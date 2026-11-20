// ==WindhawkMod==
// @id              click-click-boom
// @name            ClickClickBoom
// @description     Double-click empty taskbar space to hide/unhide the taskbar
// @version         1.0
// @author          TerminateNow
// @github          https://github.com/TerminateNow
// @include         explorer.exe
// @compilerOptions -lole32 -loleaut32 -lshell32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# ClickClickBoom

Double-click an **empty** part of the taskbar to hide it. Double-click it
again to bring it back.

## How it works

The mod toggles the Windows taskbar **auto-hide** setting:

* **Double-click** → auto-hide turns on and the taskbar slides out of view.
* While hidden, move the mouse to the screen edge and the taskbar peeks out.
  **Double-click** an empty spot on it and auto-hide turns off, so the
  taskbar stays visible again.

Double-clicks on app icons, the Start button, the clock or the tray are
ignored. Only empty taskbar space counts.

Works with the Windows 10 and Windows 11 taskbars, including secondary
monitors and ExplorerPatcher.

Because the mod changes the regular Windows setting, the taskbar keeps its
current state after the mod is disabled. You can change it back in
*Settings → Personalization → Taskbar → Taskbar behaviors*, or re-enable
the mod and double-click.
*/
// ==/WindhawkModReadme==

#include <windows.h>
#include <shellapi.h>
#include <uiautomation.h>

#include <cstdlib>

namespace {

constexpr UINT WM_APP_DOUBLE_CLICK = WM_APP + 1;

// {FF48DBA4-60EF-4201-AA87-54103EEF594E}
constexpr CLSID kClsidCUIAutomation = {
    0xff48dba4,
    0x60ef,
    0x4201,
    {0xaa, 0x87, 0x54, 0x10, 0x3e, 0xef, 0x59, 0x4e}};

HANDLE g_hookThread;
DWORD g_hookThreadId;
HANDLE g_workerThread;
DWORD g_workerThreadId;
HHOOK g_mouseHook;

// Only touched from the hook thread.
bool g_hasPendingClick;
DWORD g_lastClickTime;
POINT g_lastClickPt;

bool IsTaskbarWindow(HWND hWnd) {
    WCHAR className[64];
    if (!GetClassNameW(hWnd, className, ARRAYSIZE(className))) {
        return false;
    }

    return _wcsicmp(className, L"Shell_TrayWnd") == 0 ||
           _wcsicmp(className, L"Shell_SecondaryTrayWnd") == 0;
}

// Uses UI Automation to tell empty taskbar space apart from buttons, icons
// and the tray. Win32 hit-testing reports the taskbar window for all of them.
bool IsEmptyTaskbarSpace(IUIAutomation* automation, POINT pt) {
    if (!automation) {
        return false;
    }

    IUIAutomationElement* element = nullptr;
    if (FAILED(automation->ElementFromPoint(pt, &element)) || !element) {
        return false;
    }

    BSTR className = nullptr;
    HRESULT hr = element->get_CurrentClassName(&className);
    element->Release();
    if (FAILED(hr) || !className) {
        return false;
    }

    Wh_Log(L"Double-click on element class: %s", className);

    bool isEmpty =
        // Windows 10 taskbar (and ExplorerPatcher), primary and secondary.
        wcscmp(className, L"Shell_TrayWnd") == 0 ||
        wcscmp(className, L"Shell_SecondaryTrayWnd") == 0 ||
        // Windows 11 taskbar.
        wcscmp(className, L"Taskbar.TaskbarFrameAutomationPeer") == 0 ||
        // Windows 11 21H2 taskbar.
        wcscmp(className, L"Windows.UI.Input.InputSite.WindowClass") == 0;

    SysFreeString(className);
    return isEmpty;
}

void ToggleTaskbarAutoHide() {
    APPBARDATA abd{};
    abd.cbSize = sizeof(abd);
    abd.hWnd = FindWindowW(L"Shell_TrayWnd", nullptr);

    UINT state = (UINT)SHAppBarMessage(ABM_GETSTATE, &abd);
    bool hide = !(state & ABS_AUTOHIDE);

    abd.lParam = hide ? ABS_AUTOHIDE : ABS_ALWAYSONTOP;
    SHAppBarMessage(ABM_SETSTATE, &abd);

    Wh_Log(L"Taskbar auto-hide %s", hide ? L"enabled" : L"disabled");
}

void HandleDoubleClick(IUIAutomation* automation, POINT pt) {
    HWND hWnd = WindowFromPoint(pt);
    HWND hRoot = hWnd ? GetAncestor(hWnd, GA_ROOT) : nullptr;
    if (!hRoot || !IsTaskbarWindow(hRoot)) {
        return;
    }

    // Folder windows can run in their own explorer.exe processes. Only the
    // process that owns the taskbar should react.
    DWORD processId = 0;
    GetWindowThreadProcessId(hRoot, &processId);
    if (processId != GetCurrentProcessId()) {
        return;
    }

    if (!IsEmptyTaskbarSpace(automation, pt)) {
        return;
    }

    ToggleTaskbarAutoHide();
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        const auto* info = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);

        switch (wParam) {
            case WM_LBUTTONDOWN: {
                bool isDoubleClick =
                    g_hasPendingClick &&
                    info->time - g_lastClickTime <= GetDoubleClickTime() &&
                    std::abs(info->pt.x - g_lastClickPt.x) <=
                        GetSystemMetrics(SM_CXDOUBLECLK) / 2 &&
                    std::abs(info->pt.y - g_lastClickPt.y) <=
                        GetSystemMetrics(SM_CYDOUBLECLK) / 2;

                if (isDoubleClick) {
                    g_hasPendingClick = false;
                    // Keep the hook fast: the slow checks run on the worker.
                    PostThreadMessageW(g_workerThreadId, WM_APP_DOUBLE_CLICK,
                                       (WPARAM)(INT_PTR)info->pt.x,
                                       (LPARAM)(INT_PTR)info->pt.y);
                } else {
                    g_hasPendingClick = true;
                    g_lastClickTime = info->time;
                    g_lastClickPt = info->pt;
                }
                break;
            }

            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN:
                g_hasPendingClick = false;
                break;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

struct ThreadStartup {
    HANDLE readyEvent;
    bool succeeded;
};

DWORD WINAPI HookThreadProc(LPVOID param) {
    auto* startup = static_cast<ThreadStartup*>(param);

    MSG msg;
    PeekMessageW(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    HMODULE module = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCWSTR>(&LowLevelMouseProc), &module);

    g_mouseHook =
        SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, module, 0);
    startup->succeeded = g_mouseHook != nullptr;
    if (!g_mouseHook) {
        Wh_Log(L"SetWindowsHookEx failed: %u", GetLastError());
    }
    SetEvent(startup->readyEvent);
    if (!g_mouseHook) {
        return 1;
    }

    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnhookWindowsHookEx(g_mouseHook);
    g_mouseHook = nullptr;
    return 0;
}

DWORD WINAPI WorkerThreadProc(LPVOID param) {
    auto* startup = static_cast<ThreadStartup*>(param);

    MSG msg;
    PeekMessageW(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    HRESULT hrCo = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IUIAutomation* automation = nullptr;
    if (SUCCEEDED(hrCo)) {
        HRESULT hr = CoCreateInstance(kClsidCUIAutomation, nullptr,
                                      CLSCTX_INPROC_SERVER,
                                      __uuidof(IUIAutomation),
                                      reinterpret_cast<void**>(&automation));
        if (FAILED(hr)) {
            Wh_Log(L"Creating UI Automation failed: 0x%08X", hr);
            automation = nullptr;
        }
    } else {
        Wh_Log(L"CoInitializeEx failed: 0x%08X", hrCo);
    }

    startup->succeeded = automation != nullptr;
    SetEvent(startup->readyEvent);

    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (msg.message == WM_APP_DOUBLE_CLICK) {
            POINT pt{(LONG)(INT_PTR)msg.wParam, (LONG)(INT_PTR)msg.lParam};
            HandleDoubleClick(automation, pt);
        }
    }

    if (automation) {
        automation->Release();
    }
    if (SUCCEEDED(hrCo)) {
        CoUninitialize();
    }
    return 0;
}

bool StartThread(LPTHREAD_START_ROUTINE proc, HANDLE* thread, DWORD* threadId) {
    ThreadStartup startup{CreateEventW(nullptr, TRUE, FALSE, nullptr), false};
    if (!startup.readyEvent) {
        return false;
    }

    *thread = CreateThread(nullptr, 0, proc, &startup, 0, threadId);
    if (*thread) {
        WaitForSingleObject(startup.readyEvent, INFINITE);
    }
    CloseHandle(startup.readyEvent);

    return *thread && startup.succeeded;
}

void StopThread(HANDLE* thread, DWORD threadId) {
    if (!*thread) {
        return;
    }

    PostThreadMessageW(threadId, WM_QUIT, 0, 0);
    WaitForSingleObject(*thread, INFINITE);
    CloseHandle(*thread);
    *thread = nullptr;
}

void StopAllThreads() {
    StopThread(&g_hookThread, g_hookThreadId);
    StopThread(&g_workerThread, g_workerThreadId);
}

}  // namespace

BOOL Wh_ModInit() {
    Wh_Log(L">");

    // The worker must exist before the hook can post to it.
    if (!StartThread(WorkerThreadProc, &g_workerThread, &g_workerThreadId) ||
        !StartThread(HookThreadProc, &g_hookThread, &g_hookThreadId)) {
        StopAllThreads();
        return FALSE;
    }

    return TRUE;
}

void Wh_ModUninit() {
    Wh_Log(L">");

    StopAllThreads();
}
