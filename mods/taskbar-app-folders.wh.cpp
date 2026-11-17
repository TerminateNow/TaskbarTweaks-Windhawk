// ==WindhawkMod==
// @id              taskbar-app-folders
// @name            Taskbar App Folders
// @description     Group pinned taskbar apps into folders that open a menu of their apps when clicked (Windows 11)
// @version         1.4
// @author          TerminateNow
// @github          https://github.com/TerminateNow
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -ldwmapi -lgdi32 -lole32 -loleaut32 -lruntimeobject -lshell32 -lshlwapi
// ==/WindhawkMod==

// Source code is published under The GNU General Public License v3.0.

// ==WindhawkModReadme==
/*
# Taskbar App Folders

Group your pinned taskbar apps into folders. A folder takes up one spot on the
taskbar and shows a folder icon. Click it to open a menu that lists the apps
inside it.

## Setting up folders

Folders are defined in the mod's settings.

If you keep your folder icons together, set **Icon folder** to that folder.
Icons in it can then be given by file name alone, such as `games.ico`, or even
just `games`. Full paths work too, with or without an icon folder.

For each folder, set:

* **Folder name**: shown at the top of the folder's menu.
* **Folder icon**: an `.ico`, `.png`, `.jpg`, `.exe` or `.dll` file, either a
  file name from the icon folder or a full path. For an `.exe` or `.dll` you
  can pick an icon by adding its index after a comma, for example
  `C:\Windows\System32\shell32.dll,4`. Leave it empty to use the default
  folder icon.
* **Apps**: the pinned apps that go in the folder. Each entry can be any of:
  * the app's name, as shown when you hover over its taskbar icon or in the
    Start menu, for example `Paint` or `Google Chrome`
  * the program's file name, for example `chrome.exe`
  * the app's AppUserModelID, for example
    `Microsoft.WindowsTerminal_8wekyb3d8bbwe!App`

An app has to be pinned to the taskbar to be shown in a folder. If an app is
listed in more than one folder, the first folder wins.

## How folders behave

* The folder sits where its first closed app is pinned. The apps listed after
  it are hidden from the taskbar.
* Clicking the folder opens its menu. Click an app to open it.
* Drag the folder to move it, like any other pinned icon. This moves the app
  the folder sits on, so the folder stays in its new spot.
* Click ❌ next to an app to take it out of the folder and put it back on the
  taskbar. Removed apps are listed at the bottom of the folder's menu, under
  "Removed from this folder". Click ↩ Restore to put one back in the folder,
  or Clear to empty the list. Cleared apps stay out of the folder.
* ❌ doesn't change the mod's settings: Windhawk doesn't let a mod edit its own
  settings, so the mod remembers removed apps separately. Any change to a
  folder's app list in the settings brings back every app removed from that
  folder, including cleared ones. To remove an app for good, delete it from
  the list in the settings.
  If you remove every app in a folder, the folder disappears, so its menu
  can't restore them. Change its app list in the settings to bring them back.
* An open app gets its own taskbar icon and also stays in the folder menu,
  marked "Open", or with its window count when it has more than one, for
  example "Open | 2". Clicking it in the menu switches to it. An app that's only
  running in the background or the notification area (system tray) is also
  marked "Open", and clicking it brings up its window.
* Some apps, such as Steam and Chrome, can show their windows under a separate
  taskbar icon from their pinned one. The folder menu still lists them once,
  under the pinned app.

## Limitations

* Windows 11 taskbar only. The Windows 10 taskbar, and the Windows 10 taskbar
  restored by ExplorerPatcher, aren't supported.
* The folder needs at least one of its apps to be closed, since it sits on that
  app's taskbar spot. While every app in a folder is open, the folder is hidden
  and each app shows on its own.
* Keyboard shortcuts such as Win+1 still launch the app the folder sits on.
* Dragging the folder and opening its menu with a left click rely on Windows'
  debugging symbols for the taskbar, which Windhawk downloads the first time
  the mod loads. If that fails, the folder still opens its menu when clicked
  but can't be dragged.

## If an app isn't added to its folder

Open Windhawk's log for this mod. Every taskbar icon is logged with its app ID,
name, program file and pinned shortcut name, in this form:

`App <app ID>: name '<name>', exe '<program file>', shortcut '<shortcut name>'`

Any of those values can be used in the folder's app list. Lines like
`<app ID>: in folder <folder name>` show which folder each app was put in.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- iconFolder: ""
  $name: Icon folder
  $description: >-
    A folder that holds your folder icons, for example
    C:\Users\Me\Pictures\Folder icons. Icons in it can then be given by file
    name alone, such as games.ico, or even just games. Full paths still work
    too.
- defaultIcon: ""
  $name: Default folder icon
  $description: >-
    Used for folders that don't have their own icon. Same format as the
    folder icon. Leave empty to use the standard Windows folder icon.
- folders:
  - - name: Folder 1
      $name: Folder name
      $description: Shown at the top of the folder's menu
    - icon: ""
      $name: Folder icon
      $description: >-
        An .ico, .png, .jpg, .exe or .dll file: either a file name from the
        icon folder (games.ico, or just games) or a full path. For an .exe or
        .dll, add the icon index after a comma, for example
        C:\Windows\System32\shell32.dll,4. Leave empty to use the default
        folder icon.
    - apps: [""]
      $name: Apps
      $description: >-
        Pinned apps to put in this folder, in order. Use the app's name as
        shown when you hover over its taskbar icon (for example Paint), its
        program file name (for example chrome.exe), or its AppUserModelID.
  $name: Folders
- menuStyle: modern
  $name: Folder menu style
  $options:
  - modern: Modern (Windows 11 style)
  - classic: Classic context menu
*/
// ==/WindhawkModSettings==

#include <windhawk_utils.h>

#include <windows.h>

#include <dwmapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <tlhelp32.h>
#include <wincodec.h>
#include <xamlom.h>

#include <algorithm>
#include <cstdlib>
#include <atomic>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#undef GetCurrentTime

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.Text.h>
#include <winrt/Windows.UI.Xaml.Automation.Peers.h>
#include <winrt/Windows.UI.Xaml.Automation.Provider.h>
#include <winrt/Windows.UI.Xaml.Automation.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.h>
#include <winrt/base.h>

namespace wf = winrt::Windows::Foundation;
namespace wux = winrt::Windows::UI::Xaml;
namespace wuxc = winrt::Windows::UI::Xaml::Controls;

////////////////////////////////////////////////////////////////////////////////
// Constants

constexpr int kMaxFolders = 64;
constexpr int kMaxAppsPerFolder = 128;
constexpr UINT kFolderIconPixels = 64;
constexpr UINT kMenuIconPixels = 32;

constexpr UINT kMsgUpdate = WM_APP + 1;
constexpr UINT kMsgShowClassicMenu = WM_APP + 2;
constexpr UINT kMsgShowFolderMenu = WM_APP + 3;

// How far the pointer can move between pressing and releasing on a folder for
// it to still count as a click rather than a drag.
constexpr int kClickMoveThreshold = 12;

constexpr UINT kCmdLaunchBase = 1000;
constexpr UINT kCmdRemoveBase = 2000;
constexpr UINT kCmdRestoreBase = 3000;
constexpr UINT kCmdClearRemoved = 4000;
constexpr size_t kMaxClassicMenuEntries = 900;

constexpr WCHAR kWindowClassName[] = L"WindhawkTaskbarAppFolders_" WH_MOD_ID;

// {6B0F5D0E-3C1A-4E59-9F0B-7A2D41C8E913}
constexpr CLSID CLSID_AppFoldersTAP = {
    0x6b0f5d0e,
    0x3c1a,
    0x4e59,
    {0x9f, 0x0b, 0x7a, 0x2d, 0x41, 0xc8, 0xe9, 0x13}};

constexpr CLSID kCLSID_WICImagingFactory = {
    0xcacaf262,
    0x9370,
    0x4615,
    {0xa1, 0x3b, 0x9f, 0x55, 0x39, 0xda, 0x4c, 0x0a}};

constexpr GUID kGUID_WICPixelFormat32bppPBGRA = {
    0x6fddc324,
    0x4e03,
    0x4bfe,
    {0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x10}};

// PKEY_Link_TargetParsingPath
constexpr PROPERTYKEY kPKEY_Link_TargetParsingPath = {
    {0xb9b4b3fc,
     0x2b51,
     0x4a42,
     {0xb5, 0xd8, 0x32, 0x41, 0x46, 0xaf, 0xcf, 0x25}},
    2};

// PKEY_AppUserModel_ID
constexpr PROPERTYKEY kPKEY_AppUserModel_ID = {
    {0x9f4c2855,
     0x9f79,
     0x4b39,
     {0xa8, 0xd0, 0xe1, 0xd4, 0x2d, 0xe1, 0xd5, 0xf3}},
    5};

constexpr CLSID kCLSID_ShellLink = {
    0x00021401,
    0x0000,
    0x0000,
    {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

////////////////////////////////////////////////////////////////////////////////
// Global state. Everything below the settings is only touched on the taskbar's
// UI thread.

struct FolderConfig {
    std::wstring name;
    std::wstring icon;
    std::vector<std::wstring> apps;
    // Lowercased, normalized app entries used for matching.
    std::vector<std::wstring> matchKeys;
    // Identifies the folder in the stored list of removed apps.
    std::wstring key;
    // Changes whenever the folder's app list changes.
    std::wstring signature;
};

struct Settings {
    std::vector<FolderConfig> folders;
    std::wstring iconFolder;
    std::wstring defaultIcon;
    bool classicMenu = false;
};

Settings g_settings;

// Apps removed from a folder with the ❌ button, keyed by FolderConfig::key.
// Each removal is a group of app IDs: the app's own, then those of its other
// taskbar buttons (e.g. Steam's windows), so they're restored together. A
// cleared removal stays out of the folder but isn't offered for restoring.
struct RemovedGroup {
    std::vector<std::wstring> appIds;
    bool cleared = false;
};

struct RemovedApps {
    std::wstring signature;
    std::vector<RemovedGroup> groups;
};

std::unordered_map<std::wstring, RemovedApps> g_removedApps;

struct ButtonState {
    InstanceHandle handle = 0;
    void* key = nullptr;
    winrt::weak_ref<wux::FrameworkElement> element;

    bool setUp = false;
    int64_t automationIdToken = 0;
    winrt::event_token loadedToken{};
    // Added with handledEventsToo to see every press and release, including
    // the ones the taskbar handles itself.
    wux::Input::PointerEventHandler pointerDownHandler{nullptr};
    wux::Input::PointerEventHandler pointerUpHandler{nullptr};

    // While the pointer is down on a button (e.g. while it's being dragged),
    // the taskbar can hide its running indicator. Its running state is frozen
    // meanwhile so that an open app doesn't briefly turn into the folder.
    bool pointerDown = false;
    ULONGLONG pointerDownTime = 0;
    bool frozenRunning = false;

    bool templateAttached = false;
    winrt::weak_ref<wux::FrameworkElement> templateRoot;
    winrt::event_token pointerPressedToken{};
    winrt::event_token pointerReleasedToken{};
    winrt::event_token pointerCaptureLostToken{};
    winrt::event_token tappedToken{};
    winrt::event_token doubleTappedToken{};
    winrt::event_token rightTappedToken{};
    winrt::event_token holdingToken{};
    winrt::event_token contextRequestedToken{};
    winrt::weak_ref<wux::VisualStateGroup> runningGroup;
    winrt::event_token runningGroupToken{};
    winrt::weak_ref<wuxc::Image> icon;
    int64_t iconSourceToken = 0;

    std::wstring appId;
    std::wstring appIdLower;
    std::wstring automationName;
    void* rootKey = nullptr;
    bool running = false;
    int folder = -1;
    int order = 0;

    bool isHost = false;
    bool collapsedByUs = false;
    bool iconOverridden = false;
    bool applyingIcon = false;
    wux::Media::ImageSource originalIcon{nullptr};
};

std::vector<std::unique_ptr<ButtonState>> g_buttons;

struct AppInfo {
    std::wstring displayName;
    std::wstring exeLower;
    // Full path of the app's program, lowercase, when known.
    std::wstring exePathLower;
    // The app's shortcut in the taskbar's pinned items folder, if any.
    std::wstring shortcutName;
    std::wstring shortcutPath;
};

std::unordered_map<std::wstring, AppInfo> g_appInfoCache;
std::unordered_map<int, wux::Media::ImageSource> g_folderIcons;
std::unordered_map<std::wstring, wux::Media::ImageSource> g_appIcons;

struct PinnedShortcut {
    std::wstring path;
    std::wstring name;
    std::wstring appIdLower;
    std::wstring targetLower;
};

std::vector<PinnedShortcut> g_pinnedShortcuts;
ULONGLONG g_pinnedShortcutsLoadTime = 0;

struct MenuEntry {
    std::wstring appId;
    std::wstring name;
    // Open: it has windows, or it runs in the background (e.g. in the
    // notification area).
    bool running = false;
    int windowCount = 0;
    // The app's own running taskbar button, used to switch to it.
    void* buttonKey = nullptr;
    // Running taskbar buttons of the same program under a different app ID,
    // shown as this entry rather than separately (e.g. Steam's windows).
    std::vector<std::wstring> aliasAppIds;
};

// An app removed from a folder with ❌, which the folder menu can restore.
struct RemovedEntry {
    std::wstring appId;
    std::wstring name;
};

struct PendingClassicMenu {
    int folder = -1;
    std::vector<MenuEntry> entries;
    std::vector<RemovedEntry> removed;
    POINT pt{};
};

PendingClassicMenu g_pendingClassicMenu;
wuxc::Flyout g_menuFlyout{nullptr};

struct PressState {
    bool active = false;
    void* key = nullptr;
    uint32_t pointerId = 0;
};

PressState g_press;

// A left or middle press on a folder. When the taskbar then tries to launch
// the app the folder sits on (CTaskBand::Launch), the launch is replaced with
// the folder menu. A drag doesn't launch anything, so the folder can be moved
// like any other icon.
struct HostClickState {
    bool active = false;
    void* key = nullptr;
    POINT pressPoint{};
    ULONGLONG pressTime = 0;
    ULONGLONG releaseTime = 0;
};

HostClickState g_hostClick;
void* g_menuRequestKey = nullptr;

using CTaskBand_Launch_t = long(WINAPI*)(void* pThis,
                                         void* taskGroup,
                                         void* point,
                                         int options);
CTaskBand_Launch_t CTaskBand_Launch_Original;
bool g_launchHooked = false;

DWORD g_uiThreadId = 0;
HWND g_hwnd = nullptr;
bool g_updatePending = false;
std::atomic<bool> g_unloading = false;
std::atomic<bool> g_tapInjected = false;

////////////////////////////////////////////////////////////////////////////////
// Helpers

template <typename F>
void Guarded(F&& f) noexcept {
    try {
        f();
    } catch (...) {
        Wh_Log(L"Error %08X", winrt::to_hresult());
    }
}

std::wstring ToLower(std::wstring_view s) {
    std::wstring result(s);
    if (!result.empty()) {
        CharLowerBuffW(result.data(), static_cast<DWORD>(result.size()));
    }
    return result;
}

std::wstring Trim(std::wstring_view s) {
    constexpr std::wstring_view kSpace = L" \t\r\n";
    size_t start = s.find_first_not_of(kSpace);
    if (start == std::wstring_view::npos) {
        return {};
    }
    size_t end = s.find_last_not_of(kSpace);
    return std::wstring(s.substr(start, end - start + 1));
}

std::wstring StripQuotes(std::wstring_view s) {
    std::wstring result = Trim(s);
    if (result.size() >= 2 && result.front() == L'"' && result.back() == L'"') {
        result = Trim(std::wstring_view(result).substr(1, result.size() - 2));
    }
    return result;
}

bool StartsWithI(std::wstring_view s, std::wstring_view prefix) {
    return s.size() >= prefix.size() &&
           _wcsnicmp(s.data(), prefix.data(), prefix.size()) == 0;
}

bool EndsWithI(std::wstring_view s, std::wstring_view suffix) {
    return s.size() >= suffix.size() &&
           _wcsnicmp(s.data() + s.size() - suffix.size(), suffix.data(),
                     suffix.size()) == 0;
}

std::wstring FileNameOf(std::wstring_view path) {
    size_t pos = path.find_last_of(L"\\/");
    return std::wstring(pos == std::wstring_view::npos ? path
                                                       : path.substr(pos + 1));
}

std::wstring StripExe(std::wstring_view name) {
    if (EndsWithI(name, L".exe")) {
        return std::wstring(name.substr(0, name.size() - 4));
    }
    return std::wstring(name);
}

std::wstring ExpandEnvironment(const std::wstring& s) {
    DWORD size = ExpandEnvironmentStringsW(s.c_str(), nullptr, 0);
    if (size == 0) {
        return s;
    }
    std::wstring result(size, L'\0');
    size = ExpandEnvironmentStringsW(s.c_str(), result.data(), size);
    if (size == 0) {
        return s;
    }
    result.resize(size - 1);
    return result;
}

template <typename... Args>
std::wstring GetStringSetting(PCWSTR name, Args... args) {
    PCWSTR value = Wh_GetStringSetting(name, args...);
    std::wstring result = value ? value : L"";
    Wh_FreeStringSetting(value);
    return result;
}

// A running button's accessible name can have details after the app's name,
// e.g. "Google Chrome - 1 running window".
std::wstring ShortAutomationName(const std::wstring& name) {
    return Trim(std::wstring_view(name).substr(0, name.find(L" - ")));
}

void* CanonicalPtr(wf::IInspectable const& object) {
    return winrt::get_abi(object.as<wf::IUnknown>());
}

HMODULE GetCurrentModuleHandle() {
    HMODULE module;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            reinterpret_cast<LPCWSTR>(&GetCurrentModuleHandle),
                            &module)) {
        return nullptr;
    }
    return module;
}

std::wstring FolderDisplayName(int folder) {
    if (folder >= 0 && folder < static_cast<int>(g_settings.folders.size())) {
        return g_settings.folders[folder].name;
    }
    return L"Folder";
}

////////////////////////////////////////////////////////////////////////////////
// Settings and stored state

std::wstring NormalizeAppEntry(const std::wstring& entry) {
    std::wstring result = StripQuotes(entry);
    // A full path to a program matches by its file name, since that's what the
    // taskbar can be compared against for every kind of app ID.
    if (EndsWithI(result, L".exe") &&
        result.find_first_of(L"\\/") != std::wstring::npos) {
        result = FileNameOf(result);
    }
    return ToLower(result);
}

Settings LoadSettings() {
    Settings settings;

    for (int i = 0; i < kMaxFolders; i++) {
        FolderConfig folder;
        folder.name = Trim(GetStringSetting(L"folders[%d].name", i));
        folder.icon = Trim(GetStringSetting(L"folders[%d].icon", i));

        for (int j = 0; j < kMaxAppsPerFolder; j++) {
            std::wstring app =
                Trim(GetStringSetting(L"folders[%d].apps[%d]", i, j));
            if (app.empty()) {
                break;
            }
            folder.apps.push_back(std::move(app));
        }

        if (folder.name.empty() && folder.icon.empty() && folder.apps.empty()) {
            break;
        }

        if (folder.name.empty()) {
            folder.name = L"Folder " + std::to_wstring(i + 1);
        }

        folder.key = ToLower(folder.name);
        for (const auto& app : folder.apps) {
            std::wstring matchKey = NormalizeAppEntry(app);
            folder.signature += matchKey;
            folder.signature += L'|';
            folder.matchKeys.push_back(std::move(matchKey));
        }

        settings.folders.push_back(std::move(folder));
    }

    settings.iconFolder = StripQuotes(GetStringSetting(L"iconFolder"));
    settings.defaultIcon = Trim(GetStringSetting(L"defaultIcon"));
    settings.classicMenu = GetStringSetting(L"menuStyle") == L"classic";

    return settings;
}

std::vector<std::wstring> Split(std::wstring_view s, WCHAR separator) {
    std::vector<std::wstring> parts;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(separator, start);
        parts.emplace_back(s.substr(start, pos == std::wstring_view::npos
                                               ? std::wstring_view::npos
                                               : pos - start));
        if (pos == std::wstring_view::npos) {
            break;
        }
        start = pos + 1;
    }
    return parts;
}

// Stored as one line per folder: key, signature, then the removals, all
// separated by tabs. A removal's app IDs are separated by '|', and a cleared
// removal starts with '~'.
void LoadRemovedApps() {
    g_removedApps.clear();

    std::vector<WCHAR> buffer(65536);
    Wh_GetStringValue(L"removedApps", buffer.data(), buffer.size());

    for (const auto& line : Split(buffer.data(), L'\n')) {
        auto fields = Split(line, L'\t');
        if (fields.size() < 3 || fields[0].empty()) {
            continue;
        }

        RemovedApps removed;
        removed.signature = fields[1];
        for (size_t i = 2; i < fields.size(); i++) {
            RemovedGroup group;
            std::wstring_view field = fields[i];
            if (!field.empty() && field.front() == L'~') {
                group.cleared = true;
                field.remove_prefix(1);
            }
            for (auto& appId : Split(field, L'|')) {
                if (!appId.empty()) {
                    group.appIds.push_back(std::move(appId));
                }
            }
            if (!group.appIds.empty()) {
                removed.groups.push_back(std::move(group));
            }
        }

        if (!removed.groups.empty()) {
            g_removedApps[fields[0]] = std::move(removed);
        }
    }
}

void SaveRemovedApps() {
    std::wstring data;
    for (const auto& [key, removed] : g_removedApps) {
        if (removed.groups.empty()) {
            continue;
        }
        data += key;
        data += L'\t';
        data += removed.signature;
        for (const auto& group : removed.groups) {
            data += L'\t';
            if (group.cleared) {
                data += L'~';
            }
            for (size_t i = 0; i < group.appIds.size(); i++) {
                if (i > 0) {
                    data += L'|';
                }
                data += group.appIds[i];
            }
        }
        data += L'\n';
    }

    Wh_SetStringValue(L"removedApps", data.c_str());
}

// Forget removals for folders that were deleted or whose app list changed.
void PruneRemovedApps() {
    bool changed = false;

    for (auto it = g_removedApps.begin(); it != g_removedApps.end();) {
        const FolderConfig* folder = nullptr;
        for (const auto& f : g_settings.folders) {
            if (f.key == it->first) {
                folder = &f;
                break;
            }
        }

        if (!folder || folder->signature != it->second.signature) {
            it = g_removedApps.erase(it);
            changed = true;
        } else {
            ++it;
        }
    }

    if (changed) {
        SaveRemovedApps();
    }
}

bool IsRemovedFromFolder(const FolderConfig& folder,
                         const std::wstring& appIdLower) {
    auto it = g_removedApps.find(folder.key);
    if (it == g_removedApps.end() ||
        it->second.signature != folder.signature) {
        return false;
    }

    for (const auto& group : it->second.groups) {
        for (const auto& appId : group.appIds) {
            if (ToLower(appId) == appIdLower) {
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// App information

// Desktop apps without an explicit AppUserModelID get an ID made of a known
// folder GUID and the path relative to it, e.g.
// {6D809377-6AF0-444B-8957-A3773F02200E}\Notepad++\notepad++.exe.
std::wstring AppIdToPath(const std::wstring& appId) {
    if (appId.size() > 2 && appId[0] == L'{') {
        size_t close = appId.find(L'}');
        if (close != std::wstring::npos && close + 1 < appId.size() &&
            appId[close + 1] == L'\\') {
            GUID folderId;
            std::wstring guidString = appId.substr(0, close + 1);
            if (SUCCEEDED(CLSIDFromString(guidString.c_str(), &folderId))) {
                PWSTR folderPath = nullptr;
                HRESULT hr = SHGetKnownFolderPath(folderId, KF_FLAG_DEFAULT,
                                                  nullptr, &folderPath);
                std::wstring result;
                if (SUCCEEDED(hr) && folderPath) {
                    result = folderPath;
                    result += appId.substr(close + 1);
                }
                CoTaskMemFree(folderPath);
                return result;
            }
        }
    }

    if (appId.size() > 2 && appId[1] == L':') {
        return appId;
    }

    return {};
}

winrt::com_ptr<IShellItem> CreateAppShellItem(const std::wstring& appId) {
    winrt::com_ptr<IShellItem> item;

    std::wstring parsingName = L"shell:AppsFolder\\" + appId;
    if (SUCCEEDED(SHCreateItemFromParsingName(parsingName.c_str(), nullptr,
                                              IID_PPV_ARGS(item.put())))) {
        return item;
    }

    item = nullptr;
    std::wstring path = AppIdToPath(appId);
    if (!path.empty() &&
        SUCCEEDED(SHCreateItemFromParsingName(path.c_str(), nullptr,
                                              IID_PPV_ARGS(item.put())))) {
        return item;
    }

    return nullptr;
}

// Taskbar pins are stored as shortcuts in this folder. They're the most
// reliable way to identify and launch a pinned app: some apps, such as Steam,
// use an app ID that the Start menu's app list doesn't know.
void LoadPinnedShortcuts() {
    g_pinnedShortcuts.clear();
    g_pinnedShortcutsLoadTime = GetTickCount64();

    WCHAR appData[MAX_PATH];
    if (FAILED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr,
                                SHGFP_TYPE_CURRENT, appData))) {
        return;
    }

    std::wstring folder = appData;
    folder += L"\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar\\";

    WIN32_FIND_DATAW findData;
    HANDLE find = FindFirstFileW((folder + L"*.lnk").c_str(), &findData);
    if (find == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        PinnedShortcut shortcut;
        shortcut.path = folder + findData.cFileName;
        shortcut.name = findData.cFileName;
        if (EndsWithI(shortcut.name, L".lnk")) {
            shortcut.name.resize(shortcut.name.size() - 4);
        }

        winrt::com_ptr<IShellLinkW> link;
        if (FAILED(CoCreateInstance(kCLSID_ShellLink, nullptr,
                                    CLSCTX_INPROC_SERVER,
                                    IID_PPV_ARGS(link.put())))) {
            continue;
        }

        auto persistFile = link.try_as<IPersistFile>();
        if (!persistFile ||
            FAILED(persistFile->Load(shortcut.path.c_str(), STGM_READ))) {
            continue;
        }

        WCHAR target[MAX_PATH * 2]{};
        if (SUCCEEDED(link->GetPath(target, ARRAYSIZE(target), nullptr,
                                    SLGP_RAWPATH)) &&
            target[0]) {
            shortcut.targetLower = ToLower(ExpandEnvironment(target));
        }

        if (auto store = link.try_as<IPropertyStore>()) {
            PROPVARIANT value;
            PropVariantInit(&value);
            if (SUCCEEDED(store->GetValue(kPKEY_AppUserModel_ID, &value)) &&
                value.vt == VT_LPWSTR && value.pwszVal) {
                shortcut.appIdLower = ToLower(value.pwszVal);
            }
            PropVariantClear(&value);
        }

        g_pinnedShortcuts.push_back(std::move(shortcut));
    } while (FindNextFileW(find, &findData));

    FindClose(find);
}

std::optional<PinnedShortcut> FindPinnedShortcut(const std::wstring& appId) {
    if (g_pinnedShortcutsLoadTime == 0 ||
        GetTickCount64() - g_pinnedShortcutsLoadTime > 5000) {
        LoadPinnedShortcuts();
    }

    // A shortcut with an explicit app ID matches by it. Otherwise the taskbar
    // derives the app ID from the program's path.
    std::wstring appIdLower = ToLower(appId);
    for (const auto& shortcut : g_pinnedShortcuts) {
        if (!shortcut.appIdLower.empty() && shortcut.appIdLower == appIdLower) {
            return shortcut;
        }
    }

    std::wstring pathLower = ToLower(AppIdToPath(appId));
    if (!pathLower.empty()) {
        for (const auto& shortcut : g_pinnedShortcuts) {
            if (shortcut.appIdLower.empty() &&
                shortcut.targetLower == pathLower) {
                return shortcut;
            }
        }
    }

    return std::nullopt;
}

const AppInfo& GetAppInfo(const std::wstring& appId) {
    std::wstring key = ToLower(appId);
    auto it = g_appInfoCache.find(key);
    if (it != g_appInfoCache.end()) {
        return it->second;
    }

    AppInfo info;

    if (auto item = CreateAppShellItem(appId)) {
        PWSTR name = nullptr;
        if (SUCCEEDED(item->GetDisplayName(SIGDN_NORMALDISPLAY, &name)) &&
            name) {
            info.displayName = name;
        }
        CoTaskMemFree(name);

        if (auto item2 = item.try_as<IShellItem2>()) {
            PWSTR target = nullptr;
            if (SUCCEEDED(item2->GetString(kPKEY_Link_TargetParsingPath,
                                           &target)) &&
                target) {
                if (EndsWithI(target, L".exe")) {
                    info.exeLower = ToLower(FileNameOf(target));
                    info.exePathLower = ToLower(ExpandEnvironment(target));
                }
            }
            CoTaskMemFree(target);
        }
    }

    if (auto shortcut = FindPinnedShortcut(appId)) {
        info.shortcutName = shortcut->name;
        info.shortcutPath = shortcut->path;
        if (info.exeLower.empty() && EndsWithI(shortcut->targetLower, L".exe")) {
            info.exeLower = FileNameOf(shortcut->targetLower);
            info.exePathLower = shortcut->targetLower;
        }
    }

    if (info.exeLower.empty()) {
        std::wstring path = AppIdToPath(appId);
        const std::wstring& source = path.empty() ? appId : path;
        if (EndsWithI(source, L".exe")) {
            info.exeLower = ToLower(FileNameOf(source));
            if (source.find(L'\\') != std::wstring::npos) {
                info.exePathLower = ToLower(source);
            }
        }
    }

    // A folder entry matching the app's name should work even when the Start
    // menu doesn't know the app.
    if (info.displayName.empty()) {
        info.displayName = info.shortcutName;
    }

    Wh_Log(L"App %s: name '%s', exe '%s', shortcut '%s'", appId.c_str(),
           info.displayName.c_str(), info.exeLower.c_str(),
           info.shortcutName.c_str());

    return g_appInfoCache.emplace(std::move(key), std::move(info))
        .first->second;
}

struct LaunchParams {
    std::wstring appId;
    std::wstring shortcutPath;
};

// Launches the app the way the taskbar does for a pinned app: through its
// pinned shortcut when there is one, otherwise through the Start menu's app
// list or the program's path.
void LaunchApp(const std::wstring& appId) {
    auto* params = new LaunchParams{appId, GetAppInfo(appId).shortcutPath};
    HANDLE thread = CreateThread(
        nullptr, 0,
        [](LPVOID lpParam) -> DWORD {
            std::unique_ptr<LaunchParams> params(
                static_cast<LaunchParams*>(lpParam));

            HRESULT hrCoInit = CoInitializeEx(
                nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

            std::vector<std::wstring> targets;
            if (!params->shortcutPath.empty()) {
                targets.push_back(params->shortcutPath);
            }
            targets.push_back(L"shell:AppsFolder\\" + params->appId);
            std::wstring path = AppIdToPath(params->appId);
            if (!path.empty()) {
                targets.push_back(path);
            }

            bool launched = false;
            for (const auto& target : targets) {
                SHELLEXECUTEINFOW sei{};
                sei.cbSize = sizeof(sei);
                sei.fMask = SEE_MASK_FLAG_NO_UI;
                sei.lpFile = target.c_str();
                sei.nShow = SW_SHOWNORMAL;
                if (ShellExecuteExW(&sei)) {
                    launched = true;
                    break;
                }
                Wh_Log(L"Couldn't launch %s: %u", target.c_str(),
                       GetLastError());
            }

            if (!launched) {
                Wh_Log(L"Failed to launch %s", params->appId.c_str());
            }

            if (SUCCEEDED(hrCoInit)) {
                CoUninitialize();
            }

            return 0;
        },
        params, 0, nullptr);

    if (thread) {
        CloseHandle(thread);
    } else {
        delete params;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Icons

winrt::com_ptr<IWICImagingFactory> CreateWicFactory() {
    winrt::com_ptr<IWICImagingFactory> factory;
    CoCreateInstance(kCLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                     IID_PPV_ARGS(factory.put()));
    return factory;
}

// Converts any WIC source to a size x size premultiplied BGRA buffer, keeping
// the aspect ratio and centering the image.
bool WicSourceToPixels(IWICImagingFactory* wic,
                       IWICBitmapSource* source,
                       UINT size,
                       std::vector<BYTE>& pixels) {
    winrt::com_ptr<IWICFormatConverter> converter;
    if (FAILED(wic->CreateFormatConverter(converter.put())) ||
        FAILED(converter->Initialize(
            source, kGUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
            nullptr, 0.0, WICBitmapPaletteTypeCustom))) {
        return false;
    }

    UINT width = 0;
    UINT height = 0;
    if (FAILED(converter->GetSize(&width, &height)) || !width || !height) {
        return false;
    }

    double scale = std::min(static_cast<double>(size) / width,
                            static_cast<double>(size) / height);
    UINT scaledWidth = std::clamp(static_cast<UINT>(width * scale + 0.5), 1u,
                                  size);
    UINT scaledHeight = std::clamp(static_cast<UINT>(height * scale + 0.5),
                                   1u, size);

    winrt::com_ptr<IWICBitmapScaler> scaler;
    if (FAILED(wic->CreateBitmapScaler(scaler.put())) ||
        FAILED(scaler->Initialize(converter.get(), scaledWidth, scaledHeight,
                                  WICBitmapInterpolationModeFant))) {
        return false;
    }

    std::vector<BYTE> scaled(scaledWidth * scaledHeight * 4);
    if (FAILED(scaler->CopyPixels(nullptr, scaledWidth * 4,
                                  static_cast<UINT>(scaled.size()),
                                  scaled.data()))) {
        return false;
    }

    pixels.assign(size * size * 4, 0);
    UINT offsetX = (size - scaledWidth) / 2;
    UINT offsetY = (size - scaledHeight) / 2;
    for (UINT y = 0; y < scaledHeight; y++) {
        memcpy(&pixels[((offsetY + y) * size + offsetX) * 4],
               &scaled[y * scaledWidth * 4], scaledWidth * 4);
    }

    return true;
}

HICON ExtractIconAtSize(const std::wstring& path, int index, UINT size) {
    HICON icon = nullptr;
    if (SHDefExtractIconW(path.c_str(), index, 0, &icon, nullptr, size) ==
            S_OK &&
        icon) {
        return icon;
    }

    icon = nullptr;
    UINT count = PrivateExtractIconsW(path.c_str(), index, size, size, &icon,
                                      nullptr, 1, 0);
    if (count != 0 && count != 0xFFFFFFFF && icon) {
        return icon;
    }

    return nullptr;
}

// A relative icon path is looked up in the icon folder from the settings. A
// name without an extension tries the common icon and image extensions.
std::wstring ResolveIconPath(const std::wstring& path) {
    if (path.empty() || !PathIsRelativeW(path.c_str()) ||
        g_settings.iconFolder.empty()) {
        return path;
    }

    std::wstring folder = ExpandEnvironment(g_settings.iconFolder);
    while (!folder.empty() && (folder.back() == L'\\' || folder.back() == L'/')) {
        folder.pop_back();
    }

    std::wstring fullPath = folder + L'\\' + path;
    if (GetFileAttributesW(fullPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return fullPath;
    }

    if (*PathFindExtensionW(path.c_str()) == L'\0') {
        for (PCWSTR extension : {L".ico", L".png", L".jpg", L".jpeg", L".bmp",
                                 L".gif", L".tif", L".tiff"}) {
            std::wstring candidate = fullPath + extension;
            if (GetFileAttributesW(candidate.c_str()) !=
                INVALID_FILE_ATTRIBUTES) {
                return candidate;
            }
        }
    }

    return fullPath;
}

// spec: empty for the standard folder icon, an image file, or an .exe/.dll/.ico
// path optionally followed by ",index".
bool LoadIconSpecPixels(const std::wstring& spec,
                        UINT size,
                        std::vector<BYTE>& pixels) {
    std::wstring path;
    int index = 0;

    std::wstring trimmed = StripQuotes(spec);
    if (trimmed.empty()) {
        SHSTOCKICONINFO sii{};
        sii.cbSize = sizeof(sii);
        if (FAILED(SHGetStockIconInfo(SIID_FOLDER, SHGSI_ICONLOCATION, &sii))) {
            return false;
        }
        path = sii.szPath;
        index = sii.iIcon;
    } else {
        path = trimmed;
        size_t comma = trimmed.rfind(L',');
        if (comma != std::wstring::npos) {
            std::wstring tail = Trim(std::wstring_view(trimmed).substr(comma + 1));
            bool isNumber = !tail.empty();
            for (size_t i = 0; i < tail.size(); i++) {
                if (!iswdigit(tail[i]) && !(i == 0 && tail[i] == L'-')) {
                    isNumber = false;
                    break;
                }
            }
            if (isNumber) {
                index = _wtoi(tail.c_str());
                path = StripQuotes(std::wstring_view(trimmed).substr(0, comma));
            }
        }
        path = ResolveIconPath(ExpandEnvironment(path));
    }

    auto wic = CreateWicFactory();
    if (!wic) {
        return false;
    }

    winrt::com_ptr<IWICBitmapSource> source;

    std::wstring extension = ToLower(PathFindExtensionW(path.c_str()));
    if (extension == L".png" || extension == L".jpg" || extension == L".jpeg" ||
        extension == L".bmp" || extension == L".gif" || extension == L".tif" ||
        extension == L".tiff" || extension == L".ico") {
        winrt::com_ptr<IWICBitmapDecoder> decoder;
        if (SUCCEEDED(wic->CreateDecoderFromFilename(
                path.c_str(), nullptr, GENERIC_READ,
                WICDecodeMetadataCacheOnDemand, decoder.put()))) {
            // Icon files hold several sizes, take the largest one.
            UINT frameCount = 0;
            decoder->GetFrameCount(&frameCount);
            UINT bestFrame = 0;
            UINT bestWidth = 0;
            for (UINT i = 0; i < frameCount; i++) {
                winrt::com_ptr<IWICBitmapFrameDecode> frame;
                UINT width = 0;
                UINT height = 0;
                if (SUCCEEDED(decoder->GetFrame(i, frame.put())) &&
                    SUCCEEDED(frame->GetSize(&width, &height)) &&
                    width > bestWidth) {
                    bestWidth = width;
                    bestFrame = i;
                }
            }

            winrt::com_ptr<IWICBitmapFrameDecode> frame;
            if (SUCCEEDED(decoder->GetFrame(bestFrame, frame.put()))) {
                source.copy_from(frame.get());
            }
        }
    }

    if (!source) {
        HICON icon = ExtractIconAtSize(path, index, size);
        if (icon) {
            winrt::com_ptr<IWICBitmap> bitmap;
            if (SUCCEEDED(wic->CreateBitmapFromHICON(icon, bitmap.put()))) {
                source.copy_from(bitmap.get());
            }
            DestroyIcon(icon);
        }
    }

    if (!source) {
        Wh_Log(L"Couldn't load icon '%s'", spec.c_str());
        return false;
    }

    return WicSourceToPixels(wic.get(), source.get(), size, pixels);
}

bool LoadAppIconPixels(const std::wstring& appId,
                       UINT size,
                       std::vector<BYTE>& pixels) {
    auto item = CreateAppShellItem(appId);
    if (!item) {
        return false;
    }

    auto factory = item.try_as<IShellItemImageFactory>();
    if (!factory) {
        return false;
    }

    HBITMAP bitmapHandle = nullptr;
    if (FAILED(factory->GetImage(
            SIZE{static_cast<LONG>(size), static_cast<LONG>(size)},
            SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK, &bitmapHandle)) ||
        !bitmapHandle) {
        return false;
    }

    bool result = false;
    if (auto wic = CreateWicFactory()) {
        winrt::com_ptr<IWICBitmap> bitmap;
        if (SUCCEEDED(wic->CreateBitmapFromHBITMAP(
                bitmapHandle, nullptr, WICBitmapUsePremultipliedAlpha,
                bitmap.put()))) {
            result = WicSourceToPixels(wic.get(), bitmap.get(), size, pixels);
        }
    }

    DeleteObject(bitmapHandle);
    return result;
}

wux::Media::ImageSource PixelsToImageSource(const std::vector<BYTE>& pixels,
                                            UINT size) {
    wux::Media::Imaging::WriteableBitmap bitmap(size, size);
    auto buffer = bitmap.PixelBuffer();
    size_t length =
        std::min(static_cast<size_t>(buffer.Capacity()), pixels.size());
    memcpy(buffer.data(), pixels.data(), length);
    bitmap.Invalidate();
    return bitmap;
}

wux::Media::ImageSource GetFolderIcon(int folder) {
    auto it = g_folderIcons.find(folder);
    if (it != g_folderIcons.end()) {
        return it->second;
    }

    std::wstring spec;
    if (folder >= 0 && folder < static_cast<int>(g_settings.folders.size())) {
        spec = g_settings.folders[folder].icon;
    }
    if (spec.empty()) {
        spec = g_settings.defaultIcon;
    }

    std::vector<BYTE> pixels;
    bool loaded = LoadIconSpecPixels(spec, kFolderIconPixels, pixels);
    if (!loaded && !spec.empty()) {
        // Fall back to the standard folder icon.
        loaded = LoadIconSpecPixels(L"", kFolderIconPixels, pixels);
    }

    wux::Media::ImageSource source{nullptr};
    if (loaded) {
        source = PixelsToImageSource(pixels, kFolderIconPixels);
    }

    g_folderIcons.insert_or_assign(folder, source);
    return source;
}

wux::Media::ImageSource GetAppIcon(const std::wstring& appId) {
    std::wstring key = ToLower(appId);
    auto it = g_appIcons.find(key);
    if (it != g_appIcons.end()) {
        return it->second;
    }

    wux::Media::ImageSource source{nullptr};
    std::vector<BYTE> pixels;
    if (LoadAppIconPixels(appId, kMenuIconPixels, pixels)) {
        source = PixelsToImageSource(pixels, kMenuIconPixels);
    }

    g_appIcons.insert_or_assign(std::move(key), source);
    return source;
}

HBITMAP CreateMenuBitmap(const std::wstring& appId, UINT size) {
    std::vector<BYTE> pixels;
    if (!LoadAppIconPixels(appId, size, pixels)) {
        return nullptr;
    }

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = static_cast<LONG>(size);
    bmi.bmiHeader.biHeight = -static_cast<LONG>(size);  // Top-down.
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    HBITMAP bitmap =
        CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!bitmap || !bits) {
        return nullptr;
    }

    memcpy(bits, pixels.data(), pixels.size());
    return bitmap;
}

////////////////////////////////////////////////////////////////////////////////
// Taskbar buttons

void ScheduleUpdate() {
    if (!g_hwnd || g_updatePending || g_unloading) {
        return;
    }

    g_updatePending = true;
    PostMessageW(g_hwnd, kMsgUpdate, 0, 0);
}

ButtonState* FindButton(void* key) {
    for (auto& button : g_buttons) {
        if (button->key == key) {
            return button.get();
        }
    }
    return nullptr;
}

bool IsButtonRunning(ButtonState* button) {
    auto group = button->runningGroup.get();
    if (!group) {
        return false;
    }

    auto state = group.CurrentState();
    if (!state) {
        return false;
    }

    // The states are NoRunningIndicator, ActiveRunningIndicator,
    // InactiveRunningIndicator and so on.
    std::wstring name(state.Name());
    if (name.find(L"NoRunning") != std::wstring::npos) {
        return false;
    }
    if (name.find(L"Running") != std::wstring::npos) {
        return true;
    }
    return !name.empty() && !StartsWithI(name, L"No");
}

void SetIconSource(ButtonState* button,
                   wuxc::Image const& image,
                   wux::Media::ImageSource const& source) {
    button->applyingIcon = true;
    try {
        image.Source(source);
    } catch (...) {
        button->applyingIcon = false;
        throw;
    }
    button->applyingIcon = false;
}

void ApplyHostIcon(ButtonState* button) {
    auto image = button->icon.get();
    if (!image) {
        return;
    }

    auto folderIcon = GetFolderIcon(button->folder);
    if (!folderIcon) {
        return;
    }

    if (!button->iconOverridden) {
        button->originalIcon = image.Source();
        button->iconOverridden = true;
    }

    if (image.Source() != folderIcon) {
        SetIconSource(button, image, folderIcon);
    }
}

void RestoreIcon(ButtonState* button) {
    if (!button->iconOverridden) {
        return;
    }

    if (auto image = button->icon.get()) {
        SetIconSource(button, image, button->originalIcon);
    }

    button->originalIcon = nullptr;
    button->iconOverridden = false;
}

// The taskbar can set the icon again at any time, for example when the app's
// icon changes. Remember the new icon for restoring and put the folder icon
// back.
void OnIconSourceChanged(void* key, wux::DependencyObject const& sender) {
    ButtonState* button = FindButton(key);
    if (!button || !button->iconOverridden || button->applyingIcon) {
        return;
    }

    auto image = sender.as<wuxc::Image>();
    button->originalIcon = image.Source();
    if (button->isHost) {
        ApplyHostIcon(button);
    }
}

// What's open right now: top-level windows like the ones the taskbar shows,
// and running programs. Taken each time a folder menu opens.
struct OpenWindow {
    std::wstring appIdLower;   // The window's own app ID, if it sets one.
    std::wstring exePathLower;
};

struct OpenAppsSnapshot {
    std::vector<OpenWindow> windows;
    std::vector<std::wstring> processPathsLower;
};

std::wstring GetProcessPathLower(DWORD processId) {
    HANDLE process =
        OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (!process) {
        return {};
    }

    WCHAR path[MAX_PATH * 2];
    DWORD size = ARRAYSIZE(path);
    std::wstring result;
    if (QueryFullProcessImageNameW(process, 0, path, &size)) {
        result = ToLower(std::wstring_view(path, size));
    }

    CloseHandle(process);
    return result;
}

std::wstring GetWindowAppIdLower(HWND hWnd) {
    winrt::com_ptr<IPropertyStore> store;
    if (FAILED(SHGetPropertyStoreForWindow(hWnd, IID_PPV_ARGS(store.put())))) {
        return {};
    }

    std::wstring result;
    PROPVARIANT value;
    PropVariantInit(&value);
    if (SUCCEEDED(store->GetValue(kPKEY_AppUserModel_ID, &value)) &&
        value.vt == VT_LPWSTR && value.pwszVal) {
        result = ToLower(value.pwszVal);
    }
    PropVariantClear(&value);
    return result;
}

// Roughly the windows that get a taskbar button: visible, not cloaked (e.g. on
// another virtual desktop), titled, and either unowned or marked as app
// windows.
bool IsTaskbarStyleWindow(HWND hWnd) {
    if (!IsWindowVisible(hWnd) || GetWindowTextLengthW(hWnd) == 0) {
        return false;
    }

    LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
    bool appWindow = exStyle & WS_EX_APPWINDOW;
    if (!appWindow &&
        ((exStyle & WS_EX_TOOLWINDOW) || GetWindow(hWnd, GW_OWNER))) {
        return false;
    }

    DWORD cloaked = 0;
    if (SUCCEEDED(DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, &cloaked,
                                        sizeof(cloaked))) &&
        cloaked) {
        return false;
    }

    return true;
}

OpenAppsSnapshot TakeOpenAppsSnapshot() {
    OpenAppsSnapshot snapshot;
    std::unordered_map<DWORD, std::wstring> processPaths;

    HANDLE processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processes != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W entry{};
        entry.dwSize = sizeof(entry);
        if (Process32FirstW(processes, &entry)) {
            do {
                std::wstring path = GetProcessPathLower(entry.th32ProcessID);
                if (!path.empty()) {
                    processPaths[entry.th32ProcessID] = path;
                    snapshot.processPathsLower.push_back(std::move(path));
                }
            } while (Process32NextW(processes, &entry));
        }
        CloseHandle(processes);
    }

    struct EnumParam {
        OpenAppsSnapshot* snapshot;
        std::unordered_map<DWORD, std::wstring>* processPaths;
    } param{&snapshot, &processPaths};

    EnumWindows(
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            auto* param = reinterpret_cast<EnumParam*>(lParam);
            if (!IsTaskbarStyleWindow(hWnd)) {
                return TRUE;
            }

            DWORD processId = 0;
            GetWindowThreadProcessId(hWnd, &processId);

            OpenWindow window;
            window.appIdLower = GetWindowAppIdLower(hWnd);
            auto it = param->processPaths->find(processId);
            window.exePathLower = it != param->processPaths->end()
                                      ? it->second
                                      : GetProcessPathLower(processId);
            param->snapshot->windows.push_back(std::move(window));
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&param));

    return snapshot;
}

// exe is a full path when known, otherwise just a file name.
bool IsSameProgram(const std::wstring& exeLower,
                   const std::wstring& processPathLower) {
    if (exeLower.empty() || processPathLower.empty()) {
        return false;
    }
    if (exeLower.find(L'\\') != std::wstring::npos) {
        return exeLower == processPathLower;
    }
    return exeLower == FileNameOf(processPathLower);
}

bool IsPinnedAppId(const std::wstring& appIdLower) {
    for (const auto& shortcut : g_pinnedShortcuts) {
        if (shortcut.appIdLower == appIdLower) {
            return true;
        }
    }
    return false;
}

std::wstring EntryExe(const AppInfo& info) {
    return info.exePathLower.empty() ? info.exeLower : info.exePathLower;
}

// The program behind a running button, found through its windows.
std::wstring ExeFromWindows(const OpenAppsSnapshot& snapshot,
                            const std::wstring& appIdLower) {
    for (const auto& window : snapshot.windows) {
        if (window.appIdLower == appIdLower && !window.exePathLower.empty()) {
            return window.exePathLower;
        }
    }
    return {};
}

void CountOpenWindows(MenuEntry& entry,
                      const std::wstring& exe,
                      const OpenAppsSnapshot& snapshot) {
    std::wstring appIdLower = ToLower(entry.appId);
    std::vector<std::wstring> aliasesLower;
    for (const auto& alias : entry.aliasAppIds) {
        aliasesLower.push_back(ToLower(alias));
    }

    // File Explorer windows belong to explorer.exe, which is also the shell
    // itself, so they can only be told apart by app ID.
    bool matchByProgram = !exe.empty() && FileNameOf(exe) != L"explorer.exe";

    for (const auto& window : snapshot.windows) {
        bool matches =
            window.appIdLower == appIdLower ||
            std::find(aliasesLower.begin(), aliasesLower.end(),
                      window.appIdLower) != aliasesLower.end();

        // A window of the same program counts too, unless it belongs to
        // another pinned app (e.g. a pinned web app of a browser).
        if (!matches && matchByProgram &&
            IsSameProgram(exe, window.exePathLower) &&
            (window.appIdLower.empty() || !IsPinnedAppId(window.appIdLower))) {
            matches = true;
        }

        if (matches) {
            entry.windowCount++;
        }
    }

    bool processRunning = false;
    if (matchByProgram) {
        for (const auto& path : snapshot.processPathsLower) {
            if (IsSameProgram(exe, path)) {
                processRunning = true;
                break;
            }
        }
    }

    entry.running = entry.running || entry.windowCount > 0 || processRunning;
}

std::wstring MemberDisplayName(ButtonState* member) {
    std::wstring name = GetAppInfo(member->appId).displayName;
    if (name.empty()) {
        name = ShortAutomationName(member->automationName);
    }
    if (name.empty()) {
        name = member->appId;
    }
    return name;
}

std::vector<MenuEntry> CollectMenuEntries(ButtonState* host) {
    std::vector<ButtonState*> members;
    for (auto& button : g_buttons) {
        if (button->folder == host->folder &&
            button->rootKey == host->rootKey && !button->appId.empty()) {
            members.push_back(button.get());
        }
    }

    std::stable_sort(members.begin(), members.end(),
                     [](ButtonState* a, ButtonState* b) {
                         return a->order < b->order;
                     });

    OpenAppsSnapshot snapshot = TakeOpenAppsSnapshot();

    std::vector<MenuEntry> entries;
    std::vector<std::wstring> entryExes;
    // The position in the folder's app list that each entry matched.
    std::vector<int> entryOrders;
    std::vector<std::wstring> seen;
    std::vector<ButtonState*> unpinnedRunning;

    // Pinned apps first: those with a pinned shortcut or a closed button.
    for (ButtonState* member : members) {
        if (std::find(seen.begin(), seen.end(), member->appIdLower) !=
            seen.end()) {
            continue;
        }

        const AppInfo& info = GetAppInfo(member->appId);
        if (member->running && info.shortcutPath.empty()) {
            unpinnedRunning.push_back(member);
            continue;
        }
        seen.push_back(member->appIdLower);

        MenuEntry entry;
        entry.appId = member->appId;
        entry.name = MemberDisplayName(member);
        if (member->running) {
            entry.running = true;
            entry.buttonKey = member->key;
        }
        entries.push_back(std::move(entry));
        entryExes.push_back(EntryExe(info));
        entryOrders.push_back(member->order);
    }

    size_t pinnedCount = entries.size();

    // Running buttons without a pinned shortcut. Some apps, such as Steam and
    // Chrome, open windows under a different app ID than their pinned
    // shortcut, which gives them a separate taskbar button. Those are shown as
    // the pinned app: the one that matched the same entry in the folder's app
    // list, or else the one with the same program. Matching by the entry comes
    // first since the program can't always be found: Chrome sets its app ID
    // for the whole process, not on its windows.
    for (ButtonState* member : unpinnedRunning) {
        if (std::find(seen.begin(), seen.end(), member->appIdLower) !=
            seen.end()) {
            continue;
        }
        seen.push_back(member->appIdLower);

        const AppInfo& info = GetAppInfo(member->appId);
        std::wstring exe = EntryExe(info);
        if (exe.empty()) {
            exe = ExeFromWindows(snapshot, member->appIdLower);
        }

        std::optional<size_t> target;
        for (size_t i = 0; i < pinnedCount; i++) {
            if (entryOrders[i] == member->order) {
                target = i;
                break;
            }
        }

        if (!target && !exe.empty() &&
            FileNameOf(exe) != L"applicationframehost.exe") {
            for (size_t i = 0; i < pinnedCount; i++) {
                if (IsSameProgram(entryExes[i], exe) ||
                    IsSameProgram(exe, entryExes[i])) {
                    target = i;
                    break;
                }
            }
        }

        if (target) {
            auto& entry = entries[*target];
            entry.aliasAppIds.push_back(member->appId);
            entry.running = true;
            if (!entry.buttonKey) {
                entry.buttonKey = member->key;
            }
            continue;
        }

        MenuEntry entry;
        entry.appId = member->appId;
        entry.name = MemberDisplayName(member);
        entry.running = true;
        entry.buttonKey = member->key;
        entries.push_back(std::move(entry));
        entryExes.push_back(exe);
        entryOrders.push_back(member->order);
    }

    for (size_t i = 0; i < entries.size(); i++) {
        CountOpenWindows(entries[i], entryExes[i], snapshot);
    }

    return entries;
}

// "Open" for one window, or for an app running without windows (e.g. only in
// the notification area). The count is shown from 2 windows up: "Open | 2".
std::wstring OpenLabel(const MenuEntry& entry) {
    if (entry.windowCount >= 2) {
        return L"Open | " + std::to_wstring(entry.windowCount);
    }
    return L"Open";
}

// Clicks the app's own taskbar button, which switches to the app (or shows its
// windows) exactly like clicking it on the taskbar.
bool InvokeTaskbarButton(void* buttonKey) {
    ButtonState* button = FindButton(buttonKey);
    if (!button) {
        return false;
    }

    auto element = button->element.get();
    if (!element || element.Visibility() != wux::Visibility::Visible) {
        return false;
    }

    namespace peers = wux::Automation::Peers;
    auto peer = peers::FrameworkElementAutomationPeer::CreatePeerForElement(
        element);
    if (!peer) {
        return false;
    }

    auto invoke = peer.GetPattern(peers::PatternInterface::Invoke)
                      .try_as<wux::Automation::Provider::IInvokeProvider>();
    if (!invoke) {
        return false;
    }

    invoke.Invoke();
    return true;
}

void OpenMenuEntry(const MenuEntry& entry) {
    // An app that's open in the notification area only has no button to
    // switch with. Launching it again brings up its window instead.
    if (entry.buttonKey) {
        bool invoked = false;
        try {
            invoked = InvokeTaskbarButton(entry.buttonKey);
        } catch (...) {
            Wh_Log(L"Invoking the taskbar button failed: %08X",
                   winrt::to_hresult());
        }
        if (invoked) {
            return;
        }
    }

    LaunchApp(entry.appId);
}

void HideFolderMenu() {
    if (g_menuFlyout) {
        auto flyout = g_menuFlyout;
        g_menuFlyout = nullptr;
        flyout.Hide();
    }
}

// Takes a folder menu entry out of its folder, together with its other app IDs
// (e.g. Steam's windows), so that those don't show up in the folder on their
// own afterwards.
void RemoveEntryFromFolder(int folder, const MenuEntry& entry) {
    if (folder < 0 || folder >= static_cast<int>(g_settings.folders.size())) {
        return;
    }

    const auto& config = g_settings.folders[folder];
    auto& removed = g_removedApps[config.key];
    if (removed.signature != config.signature) {
        removed.signature = config.signature;
        removed.groups.clear();
    }

    if (IsRemovedFromFolder(config, ToLower(entry.appId))) {
        return;
    }

    RemovedGroup group;
    group.appIds.push_back(entry.appId);
    group.appIds.insert(group.appIds.end(), entry.aliasAppIds.begin(),
                        entry.aliasAppIds.end());
    removed.groups.push_back(std::move(group));

    Wh_Log(L"Removed %s from folder %s", entry.appId.c_str(),
           config.name.c_str());

    SaveRemovedApps();
    ScheduleUpdate();
}

// The apps removed from a folder with ❌, which its menu offers to restore.
std::vector<RemovedEntry> GetRemovedEntries(int folder) {
    std::vector<RemovedEntry> result;
    if (folder < 0 || folder >= static_cast<int>(g_settings.folders.size())) {
        return result;
    }

    const auto& config = g_settings.folders[folder];
    auto it = g_removedApps.find(config.key);
    if (it == g_removedApps.end() ||
        it->second.signature != config.signature) {
        return result;
    }

    for (const auto& group : it->second.groups) {
        if (group.cleared) {
            continue;
        }

        RemovedEntry entry;
        entry.appId = group.appIds.front();
        entry.name = GetAppInfo(entry.appId).displayName;
        if (entry.name.empty()) {
            std::wstring appIdLower = ToLower(entry.appId);
            for (const auto& button : g_buttons) {
                if (button->appIdLower == appIdLower &&
                    !button->automationName.empty()) {
                    entry.name = ShortAutomationName(button->automationName);
                    break;
                }
            }
        }
        if (entry.name.empty()) {
            entry.name = entry.appId;
        }
        result.push_back(std::move(entry));
    }

    return result;
}

void RestoreToFolder(int folder, const std::wstring& appId) {
    if (folder < 0 || folder >= static_cast<int>(g_settings.folders.size())) {
        return;
    }

    const auto& config = g_settings.folders[folder];
    auto it = g_removedApps.find(config.key);
    if (it == g_removedApps.end()) {
        return;
    }

    std::wstring appIdLower = ToLower(appId);
    auto& groups = it->second.groups;
    groups.erase(std::remove_if(groups.begin(), groups.end(),
                                [&](const RemovedGroup& group) {
                                    return !group.cleared &&
                                           ToLower(group.appIds.front()) ==
                                               appIdLower;
                                }),
                 groups.end());
    if (groups.empty()) {
        g_removedApps.erase(it);
    }

    Wh_Log(L"Restored %s to folder %s", appId.c_str(), config.name.c_str());

    SaveRemovedApps();
    ScheduleUpdate();
}

// Empties the folder menu's list of removed apps. The apps stay out of the
// folder until its app list in the settings changes.
void ClearRemovedList(int folder) {
    if (folder < 0 || folder >= static_cast<int>(g_settings.folders.size())) {
        return;
    }

    const auto& config = g_settings.folders[folder];
    auto it = g_removedApps.find(config.key);
    if (it == g_removedApps.end()) {
        return;
    }

    for (auto& group : it->second.groups) {
        group.cleared = true;
    }

    Wh_Log(L"Cleared the removed apps list of folder %s", config.name.c_str());

    SaveRemovedApps();
}

wux::Media::SolidColorBrush TransparentBrush() {
    return wux::Media::SolidColorBrush(winrt::Windows::UI::Color{0, 0, 0, 0});
}

void ShowModernMenu(wux::FrameworkElement const& anchor,
                    int folder,
                    const std::vector<MenuEntry>& entries,
                    const std::vector<RemovedEntry>& removed) {
    HideFolderMenu();

    wuxc::StackPanel panel;
    panel.MinWidth(220);
    panel.MaxWidth(380);

    wuxc::TextBlock title;
    title.Text(winrt::hstring(FolderDisplayName(folder)));
    title.FontWeight(winrt::Windows::UI::Text::FontWeight{600});
    title.FontSize(14);
    title.Margin(wux::Thickness{8, 2, 8, 8});
    title.TextTrimming(wux::TextTrimming::CharacterEllipsis);
    panel.Children().Append(title);

    for (const auto& entry : entries) {
        std::wstring appId = entry.appId;

        wuxc::Grid row;
        wuxc::ColumnDefinition nameColumn;
        nameColumn.Width(wux::GridLength{1, wux::GridUnitType::Star});
        wuxc::ColumnDefinition removeColumn;
        removeColumn.Width(wux::GridLength{1, wux::GridUnitType::Auto});
        row.ColumnDefinitions().Append(nameColumn);
        row.ColumnDefinitions().Append(removeColumn);

        wuxc::StackPanel content;
        content.Orientation(wuxc::Orientation::Horizontal);
        content.Spacing(10);

        if (auto icon = GetAppIcon(appId)) {
            wuxc::Image image;
            image.Source(icon);
            image.Width(20);
            image.Height(20);
            content.Children().Append(image);
        }

        wuxc::TextBlock label;
        label.Text(winrt::hstring(entry.name));
        label.VerticalAlignment(wux::VerticalAlignment::Center);
        label.TextTrimming(wux::TextTrimming::CharacterEllipsis);
        content.Children().Append(label);

        if (entry.running) {
            wuxc::TextBlock openLabel;
            openLabel.Text(winrt::hstring(OpenLabel(entry)));
            openLabel.FontSize(11);
            openLabel.Opacity(0.6);
            openLabel.VerticalAlignment(wux::VerticalAlignment::Center);
            content.Children().Append(openLabel);
        }

        wuxc::Button openButton;
        openButton.Content(content);
        openButton.HorizontalAlignment(wux::HorizontalAlignment::Stretch);
        openButton.HorizontalContentAlignment(wux::HorizontalAlignment::Left);
        openButton.Background(TransparentBrush());
        openButton.BorderThickness(wux::Thickness{0, 0, 0, 0});
        openButton.Padding(wux::Thickness{8, 6, 8, 6});
        wux::Automation::AutomationProperties::SetName(
            openButton,
            winrt::hstring((entry.running ? L"Switch to " : L"Open ") +
                           entry.name));
        openButton.Click([entry](auto const&, auto const&) {
            Guarded([&] {
                HideFolderMenu();
                OpenMenuEntry(entry);
            });
        });

        wuxc::TextBlock removeGlyph;
        removeGlyph.Text(L"\u274C");
        removeGlyph.FontSize(11);

        wuxc::Button removeButton;
        removeButton.Content(removeGlyph);
        removeButton.VerticalAlignment(wux::VerticalAlignment::Stretch);
        removeButton.Background(TransparentBrush());
        removeButton.BorderThickness(wux::Thickness{0, 0, 0, 0});
        removeButton.Padding(wux::Thickness{8, 6, 8, 6});
        wuxc::ToolTipService::SetToolTip(
            removeButton, winrt::box_value(winrt::hstring(L"Remove from folder")));
        wux::Automation::AutomationProperties::SetName(
            removeButton,
            winrt::hstring(L"Remove " + entry.name + L" from folder"));
        removeButton.Click([folder, entry](auto const&, auto const&) {
            Guarded([&] {
                HideFolderMenu();
                RemoveEntryFromFolder(folder, entry);
            });
        });

        wuxc::Grid::SetColumn(removeButton, 1);
        row.Children().Append(openButton);
        row.Children().Append(removeButton);
        panel.Children().Append(row);
    }

    if (!removed.empty()) {
        wuxc::Border separator;
        separator.Height(1);
        separator.Margin(wux::Thickness{8, 6, 8, 6});
        separator.Background(wux::Media::SolidColorBrush(
            winrt::Windows::UI::Color{0x40, 0x80, 0x80, 0x80}));
        panel.Children().Append(separator);

        wuxc::Grid captionRow;
        wuxc::ColumnDefinition captionColumn;
        captionColumn.Width(wux::GridLength{1, wux::GridUnitType::Star});
        wuxc::ColumnDefinition clearColumn;
        clearColumn.Width(wux::GridLength{1, wux::GridUnitType::Auto});
        captionRow.ColumnDefinitions().Append(captionColumn);
        captionRow.ColumnDefinitions().Append(clearColumn);

        wuxc::TextBlock caption;
        caption.Text(L"Removed from this folder");
        caption.FontSize(12);
        caption.Opacity(0.6);
        caption.VerticalAlignment(wux::VerticalAlignment::Center);
        caption.Margin(wux::Thickness{8, 0, 8, 0});

        wuxc::TextBlock clearLabel;
        clearLabel.Text(L"Clear");
        clearLabel.FontSize(12);

        wuxc::Button clearButton;
        clearButton.Content(clearLabel);
        clearButton.Background(TransparentBrush());
        clearButton.BorderThickness(wux::Thickness{0, 0, 0, 0});
        clearButton.Padding(wux::Thickness{8, 4, 8, 4});
        wuxc::ToolTipService::SetToolTip(
            clearButton,
            winrt::box_value(winrt::hstring(
                L"Clear this list. The apps stay out of the folder.")));
        wux::Automation::AutomationProperties::SetName(
            clearButton, winrt::hstring(L"Clear the removed apps list"));
        clearButton.Click([folder](auto const&, auto const&) {
            Guarded([&] {
                HideFolderMenu();
                ClearRemovedList(folder);
            });
        });

        wuxc::Grid::SetColumn(clearButton, 1);
        captionRow.Children().Append(caption);
        captionRow.Children().Append(clearButton);
        panel.Children().Append(captionRow);
    }

    for (const auto& entry : removed) {
        std::wstring appId = entry.appId;

        wuxc::Grid row;
        wuxc::ColumnDefinition nameColumn;
        nameColumn.Width(wux::GridLength{1, wux::GridUnitType::Star});
        wuxc::ColumnDefinition restoreColumn;
        restoreColumn.Width(wux::GridLength{1, wux::GridUnitType::Auto});
        row.ColumnDefinitions().Append(nameColumn);
        row.ColumnDefinitions().Append(restoreColumn);

        wuxc::StackPanel content;
        content.Orientation(wuxc::Orientation::Horizontal);
        content.Spacing(10);
        content.Margin(wux::Thickness{8, 6, 8, 6});
        content.Opacity(0.6);

        if (auto icon = GetAppIcon(appId)) {
            wuxc::Image image;
            image.Source(icon);
            image.Width(20);
            image.Height(20);
            content.Children().Append(image);
        }

        wuxc::TextBlock label;
        label.Text(winrt::hstring(entry.name));
        label.VerticalAlignment(wux::VerticalAlignment::Center);
        label.TextTrimming(wux::TextTrimming::CharacterEllipsis);
        content.Children().Append(label);

        wuxc::TextBlock restoreLabel;
        restoreLabel.Text(L"\u21A9 Restore");
        restoreLabel.FontSize(12);

        wuxc::Button restoreButton;
        restoreButton.Content(restoreLabel);
        restoreButton.VerticalAlignment(wux::VerticalAlignment::Center);
        restoreButton.Background(TransparentBrush());
        restoreButton.BorderThickness(wux::Thickness{0, 0, 0, 0});
        restoreButton.Padding(wux::Thickness{8, 6, 8, 6});
        wuxc::ToolTipService::SetToolTip(
            restoreButton,
            winrt::box_value(winrt::hstring(L"Put back in this folder")));
        wux::Automation::AutomationProperties::SetName(
            restoreButton,
            winrt::hstring(L"Restore " + entry.name + L" to folder"));
        restoreButton.Click([folder, appId](auto const&, auto const&) {
            Guarded([&] {
                HideFolderMenu();
                RestoreToFolder(folder, appId);
            });
        });

        wuxc::Grid::SetColumn(restoreButton, 1);
        row.Children().Append(content);
        row.Children().Append(restoreButton);
        panel.Children().Append(row);
    }

    wuxc::Flyout flyout;
    flyout.Content(panel);
    // The taskbar's XAML island is only as tall as the taskbar, the menu has to
    // be allowed outside of it.
    flyout.ShouldConstrainToRootBounds(false);
    flyout.Closed([](wf::IInspectable const& sender, auto const&) {
        if (g_menuFlyout && sender == g_menuFlyout) {
            g_menuFlyout = nullptr;
        }
    });

    g_menuFlyout = flyout;

    wuxc::Primitives::FlyoutShowOptions options;
    options.Placement(wuxc::Primitives::FlyoutPlacementMode::Top);
    flyout.ShowAt(anchor, options);
}

void QueueClassicMenu(int folder,
                      std::vector<MenuEntry> entries,
                      std::vector<RemovedEntry> removed) {
    if (!g_hwnd) {
        return;
    }

    g_pendingClassicMenu.folder = folder;
    g_pendingClassicMenu.entries = std::move(entries);
    g_pendingClassicMenu.removed = std::move(removed);
    GetCursorPos(&g_pendingClassicMenu.pt);

    // Run the menu's modal loop outside of the XAML input handler.
    PostMessageW(g_hwnd, kMsgShowClassicMenu, 0, 0);
}

void ShowClassicMenuNow() {
    PendingClassicMenu pending = std::move(g_pendingClassicMenu);
    g_pendingClassicMenu = PendingClassicMenu{};

    if (pending.entries.empty() || !g_hwnd) {
        return;
    }

    if (pending.entries.size() > kMaxClassicMenuEntries) {
        pending.entries.resize(kMaxClassicMenuEntries);
    }
    if (pending.removed.size() > kMaxClassicMenuEntries) {
        pending.removed.resize(kMaxClassicMenuEntries);
    }

    HMENU menu = CreatePopupMenu();
    if (!menu) {
        return;
    }

    std::vector<HBITMAP> bitmaps;
    UINT iconSize = static_cast<UINT>(GetSystemMetrics(SM_CXSMICON));

    AppendMenuW(menu, MF_STRING | MF_GRAYED, 1,
                FolderDisplayName(pending.folder).c_str());
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);

    for (size_t i = 0; i < pending.entries.size(); i++) {
        UINT id = kCmdLaunchBase + static_cast<UINT>(i);
        std::wstring text = pending.entries[i].name;
        if (pending.entries[i].running) {
            text += L"\t" + OpenLabel(pending.entries[i]);
        }
        AppendMenuW(menu, MF_STRING, id, text.c_str());

        if (HBITMAP bitmap =
                CreateMenuBitmap(pending.entries[i].appId, iconSize)) {
            MENUITEMINFOW mii{};
            mii.cbSize = sizeof(mii);
            mii.fMask = MIIM_BITMAP;
            mii.hbmpItem = bitmap;
            SetMenuItemInfoW(menu, id, FALSE, &mii);
            bitmaps.push_back(bitmap);
        }
    }

    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);

    for (size_t i = 0; i < pending.entries.size(); i++) {
        std::wstring text = L"\u274C  Remove " + pending.entries[i].name;
        AppendMenuW(menu, MF_STRING, kCmdRemoveBase + static_cast<UINT>(i),
                    text.c_str());
    }

    if (!pending.removed.empty()) {
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        for (size_t i = 0; i < pending.removed.size(); i++) {
            std::wstring text = L"\u21A9  Restore " + pending.removed[i].name;
            AppendMenuW(menu, MF_STRING,
                        kCmdRestoreBase + static_cast<UINT>(i), text.c_str());
        }
        AppendMenuW(menu, MF_STRING, kCmdClearRemoved,
                    L"Clear the removed apps list");
    }

    SetForegroundWindow(g_hwnd);
    UINT command = static_cast<UINT>(TrackPopupMenuEx(
        menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_BOTTOMALIGN, pending.pt.x,
        pending.pt.y, g_hwnd, nullptr));
    PostMessageW(g_hwnd, WM_NULL, 0, 0);

    DestroyMenu(menu);
    for (HBITMAP bitmap : bitmaps) {
        DeleteObject(bitmap);
    }

    size_t count = pending.entries.size();
    size_t removedCount = pending.removed.size();
    if (command == kCmdClearRemoved) {
        ClearRemovedList(pending.folder);
    } else if (command >= kCmdRestoreBase &&
               command < kCmdRestoreBase + removedCount) {
        RestoreToFolder(pending.folder,
                        pending.removed[command - kCmdRestoreBase].appId);
    } else if (command >= kCmdRemoveBase && command < kCmdRemoveBase + count) {
        RemoveEntryFromFolder(pending.folder,
                              pending.entries[command - kCmdRemoveBase]);
    } else if (command >= kCmdLaunchBase && command < kCmdLaunchBase + count) {
        OpenMenuEntry(pending.entries[command - kCmdLaunchBase]);
    }
}

void ShowFolderMenu(ButtonState* host) {
    auto element = host->element.get();
    if (!element || host->folder < 0) {
        return;
    }

    auto entries = CollectMenuEntries(host);
    if (entries.empty()) {
        return;
    }

    auto removed = GetRemovedEntries(host->folder);

    if (g_settings.classicMenu) {
        QueueClassicMenu(host->folder, std::move(entries), std::move(removed));
        return;
    }

    try {
        ShowModernMenu(element, host->folder, entries, removed);
    } catch (...) {
        Wh_Log(L"Modern menu failed (%08X), using the classic menu",
               winrt::to_hresult());
        g_menuFlyout = nullptr;
        QueueClassicMenu(host->folder, std::move(entries), std::move(removed));
    }
}

// Handlers on the template root, which run before the button's own handling.
// With the launch hook, only right clicks on a folder are taken over here: a
// left press goes through so that the folder can be dragged, and the click is
// turned into the folder menu in CTaskBand_Launch_Hook. Without the hook, every
// press on a folder is taken over, and the folder can't be dragged.
void OnPointerPressed(void* key, wux::Input::PointerRoutedEventArgs const& e) {
    ButtonState* button = FindButton(key);
    if (!button || !button->isHost) {
        return;
    }

    auto root = button->templateRoot.get();
    if (!root) {
        return;
    }

    bool rightButton =
        e.GetCurrentPoint(root).Properties().IsRightButtonPressed();
    if (g_launchHooked && !rightButton) {
        return;
    }

    // Keep the taskbar from launching the app the folder sits on, or from
    // showing its jump list.
    e.Handled(true);

    g_press.active = true;
    g_press.key = key;
    g_press.pointerId = e.Pointer().PointerId();
    root.CapturePointer(e.Pointer());
}

void OnPointerReleased(void* key, wux::Input::PointerRoutedEventArgs const& e) {
    ButtonState* button = FindButton(key);
    if (!button) {
        return;
    }

    bool wasPressed = g_press.active && g_press.key == key &&
                      g_press.pointerId == e.Pointer().PointerId();
    if (!wasPressed) {
        if (button->isHost && !g_launchHooked) {
            e.Handled(true);
        }
        return;
    }

    e.Handled(true);

    g_press.active = false;
    if (auto root = button->templateRoot.get()) {
        root.ReleasePointerCapture(e.Pointer());
    }

    auto element = button->element.get();
    if (!element || !button->isHost) {
        return;
    }

    // Like a regular button, releasing outside of it cancels the click.
    auto position = e.GetCurrentPoint(element).Position();
    if (position.X < 0 || position.Y < 0 ||
        position.X > element.ActualWidth() ||
        position.Y > element.ActualHeight()) {
        return;
    }

    ShowFolderMenu(button);
}

// Handlers on the button itself, added with handledEventsToo, so they see
// every press and release whoever handles it.
void OnButtonPointerDown(void* key, wux::Input::PointerRoutedEventArgs const& e) {
    ButtonState* button = FindButton(key);
    if (!button) {
        return;
    }

    if (!button->pointerDown) {
        button->pointerDown = true;
        button->pointerDownTime = GetTickCount64();
        button->frozenRunning = button->running;
    }

    if (!g_launchHooked || !button->isHost) {
        return;
    }

    auto element = button->element.get();
    if (!element ||
        e.GetCurrentPoint(element).Properties().IsRightButtonPressed()) {
        return;
    }

    g_hostClick = HostClickState{};
    g_hostClick.active = true;
    g_hostClick.key = key;
    GetCursorPos(&g_hostClick.pressPoint);
    g_hostClick.pressTime = GetTickCount64();
}

void OnButtonPointerUp(void* key) {
    ButtonState* button = FindButton(key);
    if (!button) {
        return;
    }

    if (button->pointerDown) {
        button->pointerDown = false;
        ScheduleUpdate();
    }

    if (g_hostClick.active && g_hostClick.key == key &&
        !g_hostClick.releaseTime) {
        g_hostClick.releaseTime = GetTickCount64();
    }
}

// Called when the taskbar is about to launch a pinned app. Returns whether the
// launch is a click on a folder, which should show its menu instead.
bool TakeHostClick(void** key) {
    if (!g_hostClick.active || g_unloading || !g_hwnd) {
        return false;
    }

    HostClickState click = g_hostClick;
    g_hostClick = HostClickState{};

    ULONGLONG now = GetTickCount64();
    bool recent = click.releaseTime ? now - click.releaseTime < 2000
                                    : now - click.pressTime < 10000;

    POINT point;
    GetCursorPos(&point);
    bool moved = std::abs(point.x - click.pressPoint.x) > kClickMoveThreshold ||
                 std::abs(point.y - click.pressPoint.y) > kClickMoveThreshold;

    if (!recent || moved) {
        return false;
    }

    *key = click.key;
    return true;
}

long WINAPI CTaskBand_Launch_Hook(void* pThis,
                                  void* taskGroup,
                                  void* point,
                                  int options) {
    void* key = nullptr;
    if (TakeHostClick(&key)) {
        Wh_Log(L"Showing a folder menu instead of launching");
        g_menuRequestKey = key;
        PostMessageW(g_hwnd, kMsgShowFolderMenu, 0, 0);
        return 0;
    }

    return CTaskBand_Launch_Original(pThis, taskGroup, point, options);
}

bool IsHostButton(void* key) {
    ButtonState* button = FindButton(key);
    return button && button->isHost;
}

wuxc::Image FindIconImage(wux::DependencyObject const& root) {
    wuxc::Image fallback{nullptr};

    std::vector<std::pair<wux::DependencyObject, int>> queue{{root, 0}};
    for (size_t i = 0; i < queue.size(); i++) {
        auto [element, depth] = queue[i];

        if (auto image = element.try_as<wuxc::Image>()) {
            if (image.Name() == L"Icon") {
                return image;
            }
            if (!fallback) {
                fallback = image;
            }
        }

        if (depth >= 6) {
            continue;
        }

        int count = wux::Media::VisualTreeHelper::GetChildrenCount(element);
        for (int j = 0; j < count; j++) {
            queue.emplace_back(
                wux::Media::VisualTreeHelper::GetChild(element, j), depth + 1);
        }
    }

    return fallback;
}

void SetUpButton(ButtonState* button, wux::FrameworkElement const& element) {
    void* key = button->key;

    button->automationIdToken = element.RegisterPropertyChangedCallback(
        wux::Automation::AutomationProperties::AutomationIdProperty(),
        [](wux::DependencyObject const&, wux::DependencyProperty const&) {
            ScheduleUpdate();
        });

    button->loadedToken =
        element.Loaded([](auto const&, auto const&) { ScheduleUpdate(); });

    button->pointerDownHandler = wux::Input::PointerEventHandler(
        [key](auto const&, wux::Input::PointerRoutedEventArgs const& e) {
            Guarded([&] { OnButtonPointerDown(key, e); });
        });
    button->pointerUpHandler = wux::Input::PointerEventHandler(
        [key](auto const&, wux::Input::PointerRoutedEventArgs const&) {
            Guarded([&] { OnButtonPointerUp(key); });
        });

    element.AddHandler(wux::UIElement::PointerPressedEvent(),
                       winrt::box_value(button->pointerDownHandler), true);
    element.AddHandler(wux::UIElement::PointerReleasedEvent(),
                       winrt::box_value(button->pointerUpHandler), true);
    element.AddHandler(wux::UIElement::PointerCaptureLostEvent(),
                       winrt::box_value(button->pointerUpHandler), true);
    element.AddHandler(wux::UIElement::PointerCanceledEvent(),
                       winrt::box_value(button->pointerUpHandler), true);

    button->setUp = true;
}

void AttachTemplate(ButtonState* button, wux::FrameworkElement const& element) {
    using wux::Media::VisualTreeHelper;

    if (VisualTreeHelper::GetChildrenCount(element) == 0) {
        if (auto control = element.try_as<wuxc::Control>()) {
            control.ApplyTemplate();
        }
    }

    if (VisualTreeHelper::GetChildrenCount(element) == 0) {
        // Not ready yet, retried on the next update (e.g. when it's loaded).
        return;
    }

    auto root = VisualTreeHelper::GetChild(element, 0)
                    .try_as<wux::FrameworkElement>();
    if (!root) {
        return;
    }

    void* key = button->key;

    // The handlers are on the template root rather than on the button itself
    // so that they run, and can mark the events as handled, before the
    // button's own handling.
    button->pointerPressedToken = root.PointerPressed(
        [key](auto const&, wux::Input::PointerRoutedEventArgs const& e) {
            Guarded([&] { OnPointerPressed(key, e); });
        });
    button->pointerReleasedToken = root.PointerReleased(
        [key](auto const&, wux::Input::PointerRoutedEventArgs const& e) {
            Guarded([&] { OnPointerReleased(key, e); });
        });
    button->pointerCaptureLostToken = root.PointerCaptureLost(
        [key](auto const&, wux::Input::PointerRoutedEventArgs const&) {
            if (g_press.key == key) {
                g_press.active = false;
            }
        });
    button->tappedToken = root.Tapped(
        [key](auto const&, wux::Input::TappedRoutedEventArgs const& e) {
            Guarded([&] {
                // With the launch hook, a left click has to reach the taskbar
                // for the hook to turn it into the folder menu.
                if (!g_launchHooked && IsHostButton(key)) {
                    e.Handled(true);
                }
            });
        });
    button->doubleTappedToken = root.DoubleTapped(
        [key](auto const&, wux::Input::DoubleTappedRoutedEventArgs const& e) {
            Guarded([&] {
                // With the launch hook, a left click has to reach the taskbar
                // for the hook to turn it into the folder menu.
                if (!g_launchHooked && IsHostButton(key)) {
                    e.Handled(true);
                }
            });
        });
    button->rightTappedToken = root.RightTapped(
        [key](auto const&, wux::Input::RightTappedRoutedEventArgs const& e) {
            Guarded([&] {
                if (IsHostButton(key)) {
                    e.Handled(true);
                }
            });
        });
    button->holdingToken = root.Holding(
        [key](auto const&, wux::Input::HoldingRoutedEventArgs const& e) {
            Guarded([&] {
                if (IsHostButton(key)) {
                    e.Handled(true);
                }
            });
        });
    button->contextRequestedToken = root.ContextRequested(
        [key](auto const&, wux::Input::ContextRequestedEventArgs const& e) {
            Guarded([&] {
                if (IsHostButton(key)) {
                    e.Handled(true);
                }
            });
        });

    for (auto const& group : wux::VisualStateManager::GetVisualStateGroups(root)) {
        if (std::wstring_view(group.Name()).find(L"RunningIndicator") !=
            std::wstring_view::npos) {
            button->runningGroup = winrt::make_weak(group);
            button->runningGroupToken = group.CurrentStateChanged(
                [](auto const&, auto const&) { ScheduleUpdate(); });
            break;
        }
    }

    if (!button->runningGroup.get()) {
        Wh_Log(L"Running indicator states not found");
    }

    if (auto image = FindIconImage(root)) {
        button->icon = winrt::make_weak(image);
        button->iconSourceToken = image.RegisterPropertyChangedCallback(
            wuxc::Image::SourceProperty(),
            [key](wux::DependencyObject const& sender,
                  wux::DependencyProperty const&) {
                Guarded([&] { OnIconSourceChanged(key, sender); });
            });
    } else {
        Wh_Log(L"Icon element not found");
    }

    button->templateRoot = winrt::make_weak(root);
    button->templateAttached = true;
}

// Restores everything the mod changed on the button and removes its handlers.
void DetachButton(ButtonState* button) {
    auto element = button->element.get();

    Guarded([&] { RestoreIcon(button); });

    if (element) {
        Guarded([&] {
            if (button->collapsedByUs) {
                element.Visibility(wux::Visibility::Visible);
            }
        });
        Guarded([&] {
            if (button->automationIdToken) {
                element.UnregisterPropertyChangedCallback(
                    wux::Automation::AutomationProperties::
                        AutomationIdProperty(),
                    button->automationIdToken);
            }
            if (button->loadedToken) {
                element.Loaded(button->loadedToken);
            }
            if (button->pointerDownHandler) {
                element.RemoveHandler(
                    wux::UIElement::PointerPressedEvent(),
                    winrt::box_value(button->pointerDownHandler));
            }
            if (button->pointerUpHandler) {
                auto handler = winrt::box_value(button->pointerUpHandler);
                element.RemoveHandler(wux::UIElement::PointerReleasedEvent(),
                                      handler);
                element.RemoveHandler(
                    wux::UIElement::PointerCaptureLostEvent(), handler);
                element.RemoveHandler(wux::UIElement::PointerCanceledEvent(),
                                      handler);
            }
        });
    }
    button->collapsedByUs = false;
    button->isHost = false;

    Guarded([&] {
        auto image = button->icon.get();
        if (image && button->iconSourceToken) {
            image.UnregisterPropertyChangedCallback(
                wuxc::Image::SourceProperty(), button->iconSourceToken);
        }
    });

    Guarded([&] {
        auto group = button->runningGroup.get();
        if (group && button->runningGroupToken) {
            group.CurrentStateChanged(button->runningGroupToken);
        }
    });

    Guarded([&] {
        auto root = button->templateRoot.get();
        if (!root) {
            return;
        }
        if (button->pointerPressedToken) {
            root.PointerPressed(button->pointerPressedToken);
        }
        if (button->pointerReleasedToken) {
            root.PointerReleased(button->pointerReleasedToken);
        }
        if (button->pointerCaptureLostToken) {
            root.PointerCaptureLost(button->pointerCaptureLostToken);
        }
        if (button->tappedToken) {
            root.Tapped(button->tappedToken);
        }
        if (button->doubleTappedToken) {
            root.DoubleTapped(button->doubleTappedToken);
        }
        if (button->rightTappedToken) {
            root.RightTapped(button->rightTappedToken);
        }
        if (button->holdingToken) {
            root.Holding(button->holdingToken);
        }
        if (button->contextRequestedToken) {
            root.ContextRequested(button->contextRequestedToken);
        }
    });
}

void MatchFolder(ButtonState* button) {
    button->folder = -1;
    button->order = 0;

    if (button->appId.empty()) {
        return;
    }

    const AppInfo& info = GetAppInfo(button->appId);
    std::wstring displayNameLower = ToLower(info.displayName);
    std::wstring automationNameLower = ToLower(button->automationName);
    // In case the accessible name has extra details after the app's name.
    std::wstring automationNameShortLower = automationNameLower.substr(
        0, automationNameLower.find(L" - "));
    std::wstring shortcutNameLower = ToLower(info.shortcutName);
    std::wstring exeStem = StripExe(info.exeLower);

    for (size_t i = 0; i < g_settings.folders.size(); i++) {
        const auto& folder = g_settings.folders[i];
        for (size_t k = 0; k < folder.matchKeys.size(); k++) {
            const auto& entry = folder.matchKeys[k];
            bool matches =
                entry == button->appIdLower ||
                (!info.exeLower.empty() &&
                 (entry == info.exeLower || entry == exeStem)) ||
                (!displayNameLower.empty() && entry == displayNameLower) ||
                (!shortcutNameLower.empty() && entry == shortcutNameLower) ||
                (!automationNameLower.empty() &&
                 (entry == automationNameLower ||
                  entry == automationNameShortLower));
            if (!matches) {
                continue;
            }

            if (IsRemovedFromFolder(folder, button->appIdLower)) {
                break;
            }

            button->folder = static_cast<int>(i);
            button->order = static_cast<int>(k);
            return;
        }
    }
}

void RefreshButton(ButtonState* button) {
    auto element = button->element.get();
    if (!element) {
        return;
    }

    if (!button->setUp) {
        SetUpButton(button, element);
    }

    if (!button->templateAttached) {
        AttachTemplate(button, element);
    }

    // The ID looks like "Appid: Microsoft.WindowsCalculator_8wekyb3d8bbwe!App".
    std::wstring appId(
        wux::Automation::AutomationProperties::GetAutomationId(element));
    if (StartsWithI(appId, L"Appid:")) {
        appId = Trim(std::wstring_view(appId).substr(6));
    } else {
        appId.clear();
    }

    if (appId != button->appId) {
        button->appId = appId;
        button->appIdLower = ToLower(appId);
        button->automationName.clear();
    }

    bool running = IsButtonRunning(button);
    if (button->pointerDown &&
        GetTickCount64() - button->pointerDownTime < 60 * 1000) {
        running = button->frozenRunning;
    } else {
        button->pointerDown = false;
    }
    button->running = running;
    if (!button->running) {
        // While the app is closed, the accessible name is just the app's name.
        button->automationName = Trim(
            wux::Automation::AutomationProperties::GetName(element));
    }

    auto xamlRoot = element.XamlRoot();
    button->rootKey = xamlRoot ? CanonicalPtr(xamlRoot) : nullptr;

    int previousFolder = button->folder;
    MatchFolder(button);
    if (button->folder != previousFolder && !button->appId.empty()) {
        Wh_Log(L"%s: %s", button->appId.c_str(),
               button->folder >= 0
                   ? (L"in folder " + FolderDisplayName(button->folder)).c_str()
                   : L"not in a folder");
    }
}

void ApplyButtonState(ButtonState* button, bool host, bool collapse) {
    auto element = button->element.get();
    if (!element) {
        return;
    }

    if (collapse) {
        if (button->isHost) {
            RestoreIcon(button);
            button->isHost = false;
        }
        if (!button->collapsedByUs) {
            element.Visibility(wux::Visibility::Collapsed);
            button->collapsedByUs = true;
        }
        return;
    }

    if (button->collapsedByUs) {
        element.Visibility(wux::Visibility::Visible);
        button->collapsedByUs = false;
    }

    if (host) {
        button->isHost = true;
        ApplyHostIcon(button);
    } else if (button->isHost) {
        RestoreIcon(button);
        button->isHost = false;
    }
}

void UpdateAll() {
    if (g_unloading) {
        return;
    }

    for (auto& button : g_buttons) {
        Guarded([&] { RefreshButton(button.get()); });
    }

    // In each taskbar, a folder sits on its first member that isn't running.
    std::map<std::pair<int, void*>, ButtonState*> hosts;
    for (auto& button : g_buttons) {
        if (button->folder < 0 || button->running ||
            !button->element.get()) {
            continue;
        }

        auto key = std::make_pair(button->folder, button->rootKey);
        auto it = hosts.find(key);
        if (it == hosts.end() || button->order < it->second->order) {
            hosts[key] = button.get();
        }
    }

    for (auto& button : g_buttons) {
        Guarded([&] {
            bool host = false;
            bool collapse = false;
            if (button->folder >= 0 && !button->running) {
                auto it =
                    hosts.find(std::make_pair(button->folder, button->rootKey));
                host = it != hosts.end() && it->second == button.get();
                collapse = !host;
            }
            ApplyButtonState(button.get(), host, collapse);
        });
    }
}

void OnButtonAdded(wux::FrameworkElement const& element, InstanceHandle handle) {
    void* key = CanonicalPtr(element);
    if (ButtonState* existing = FindButton(key)) {
        existing->handle = handle;
        ScheduleUpdate();
        return;
    }

    auto button = std::make_unique<ButtonState>();
    button->handle = handle;
    button->key = key;
    button->element = winrt::make_weak(element);
    g_buttons.push_back(std::move(button));

    ScheduleUpdate();
}

void OnElementRemoved(InstanceHandle handle) {
    for (auto it = g_buttons.begin(); it != g_buttons.end(); ++it) {
        if ((*it)->handle == handle) {
            DetachButton(it->get());
            g_buttons.erase(it);
            ScheduleUpdate();
            return;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// UI thread setup

LRESULT CALLBACK MessageWindowProc(HWND hWnd,
                                   UINT uMsg,
                                   WPARAM wParam,
                                   LPARAM lParam) {
    switch (uMsg) {
        case kMsgUpdate:
            g_updatePending = false;
            Guarded([] { UpdateAll(); });
            return 0;

        case kMsgShowClassicMenu:
            Guarded([] { ShowClassicMenuNow(); });
            return 0;

        case kMsgShowFolderMenu:
            Guarded([] {
                ButtonState* button = FindButton(g_menuRequestKey);
                g_menuRequestKey = nullptr;
                if (button && button->isHost) {
                    ShowFolderMenu(button);
                }
            });
            return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void InitializeForCurrentThread() {
    if (g_hwnd) {
        return;
    }

    g_uiThreadId = GetCurrentThreadId();

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = MessageWindowProc;
    wc.hInstance = GetCurrentModuleHandle();
    wc.lpszClassName = kWindowClassName;
    RegisterClassExW(&wc);

    // A hidden top-level window rather than a message-only one, so that it can
    // own the classic menu.
    g_hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, kWindowClassName, L"", WS_POPUP,
                             0, 0, 0, 0, nullptr, nullptr, wc.hInstance,
                             nullptr);
    if (!g_hwnd) {
        Wh_Log(L"Failed to create the message window: %u", GetLastError());
    }
}

void UninitializeForCurrentThread() {
    Guarded([] { HideFolderMenu(); });

    for (auto& button : g_buttons) {
        DetachButton(button.get());
    }
    g_buttons.clear();

    g_folderIcons.clear();
    g_appIcons.clear();
    g_menuFlyout = nullptr;
    g_pendingClassicMenu = PendingClassicMenu{};
    g_press = PressState{};

    if (g_hwnd) {
        DestroyWindow(g_hwnd);
        g_hwnd = nullptr;
    }
    UnregisterClassW(kWindowClassName, GetCurrentModuleHandle());
    g_updatePending = false;
}

////////////////////////////////////////////////////////////////////////////////
// XAML diagnostics. Based on the approach used by Windows 11 Taskbar Styler:
// the mod DLL registers itself as a XAML diagnostics tap (TAP) to be told about
// every element that's added to or removed from the taskbar's visual tree.

class VisualTreeWatcher
    : public winrt::implements<VisualTreeWatcher,
                               IVisualTreeServiceCallback2,
                               winrt::non_agile> {
   public:
    explicit VisualTreeWatcher(winrt::com_ptr<IUnknown> site);

    VisualTreeWatcher(const VisualTreeWatcher&) = delete;
    VisualTreeWatcher& operator=(const VisualTreeWatcher&) = delete;

    void UnadviseVisualTreeChange();

   private:
    HRESULT STDMETHODCALLTYPE
    OnVisualTreeChange(ParentChildRelation relation,
                       VisualElement element,
                       VisualMutationType mutationType) override;
    HRESULT STDMETHODCALLTYPE
    OnElementStateChanged(InstanceHandle element,
                          VisualElementState elementState,
                          LPCWSTR context) noexcept override;

    winrt::com_ptr<IXamlDiagnostics> m_xamlDiagnostics;
};

VisualTreeWatcher::VisualTreeWatcher(winrt::com_ptr<IUnknown> site)
    : m_xamlDiagnostics(site.as<IXamlDiagnostics>()) {
    // Calling AdviseVisualTreeChange from the current thread can hang, so it's
    // done from a new thread.
    HANDLE thread = CreateThread(
        nullptr, 0,
        [](LPVOID lpParam) -> DWORD {
            auto watcher = static_cast<VisualTreeWatcher*>(lpParam);
            HRESULT hr = watcher->m_xamlDiagnostics.as<IVisualTreeService3>()
                             ->AdviseVisualTreeChange(watcher);
            watcher->Release();
            if (FAILED(hr)) {
                Wh_Log(L"AdviseVisualTreeChange failed: %08X", hr);
            }
            return 0;
        },
        this, 0, nullptr);
    if (thread) {
        AddRef();
        CloseHandle(thread);
    }
}

void VisualTreeWatcher::UnadviseVisualTreeChange() {
    HRESULT hr = m_xamlDiagnostics.as<IVisualTreeService3>()
                     ->UnadviseVisualTreeChange(this);
    if (FAILED(hr)) {
        Wh_Log(L"UnadviseVisualTreeChange failed: %08X", hr);
    }
}

HRESULT VisualTreeWatcher::OnVisualTreeChange(ParentChildRelation,
                                              VisualElement element,
                                              VisualMutationType mutationType) {
    if (g_unloading || !g_hwnd) {
        return S_OK;
    }

    Guarded([&] {
        if (mutationType == Add) {
            if (!element.Type ||
                wcscmp(element.Type, L"Taskbar.TaskListButton") != 0) {
                return;
            }

            wf::IInspectable inspectable;
            winrt::check_hresult(m_xamlDiagnostics->GetIInspectableFromHandle(
                element.Handle,
                reinterpret_cast<::IInspectable**>(winrt::put_abi(inspectable))));

            if (auto frameworkElement =
                    inspectable.try_as<wux::FrameworkElement>()) {
                OnButtonAdded(frameworkElement, element.Handle);
            }
        } else if (mutationType == Remove) {
            OnElementRemoved(element.Handle);
        }
    });

    // Returning an error stops further notifications.
    return S_OK;
}

HRESULT VisualTreeWatcher::OnElementStateChanged(InstanceHandle,
                                                 VisualElementState,
                                                 LPCWSTR) noexcept {
    return S_OK;
}

[[clang::no_destroy]] winrt::com_ptr<VisualTreeWatcher> g_visualTreeWatcher;
SRWLOCK g_visualTreeWatcherLock = SRWLOCK_INIT;

winrt::com_ptr<VisualTreeWatcher> ExchangeVisualTreeWatcher(
    winrt::com_ptr<VisualTreeWatcher> watcher) {
    AcquireSRWLockExclusive(&g_visualTreeWatcherLock);
    std::swap(g_visualTreeWatcher, watcher);
    ReleaseSRWLockExclusive(&g_visualTreeWatcherLock);
    return watcher;
}

class AppFoldersTAP : public winrt::implements<AppFoldersTAP,
                                               IObjectWithSite,
                                               winrt::non_agile> {
   public:
    HRESULT STDMETHODCALLTYPE SetSite(IUnknown* pUnkSite) override try {
        if (auto previous = ExchangeVisualTreeWatcher(nullptr)) {
            previous->UnadviseVisualTreeChange();
        }

        m_site.copy_from(pUnkSite);

        if (m_site) {
            // Undo the reference InitializeXamlDiagnosticsEx took on this DLL,
            // so that Windhawk can unload the mod.
            FreeLibrary(GetCurrentModuleHandle());

            ExchangeVisualTreeWatcher(
                winrt::make_self<VisualTreeWatcher>(m_site));
        }

        return S_OK;
    } catch (...) {
        HRESULT hr = winrt::to_hresult();
        Wh_Log(L"Error %08X", hr);
        return hr;
    }

    HRESULT STDMETHODCALLTYPE GetSite(REFIID riid,
                                      void** ppvSite) noexcept override {
        return m_site.as(riid, ppvSite);
    }

   private:
    winrt::com_ptr<IUnknown> m_site;
};

template <class T>
struct SimpleFactory
    : winrt::implements<SimpleFactory<T>, IClassFactory, winrt::non_agile> {
    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter,
                                             REFIID riid,
                                             void** ppvObject) override try {
        if (pUnkOuter) {
            return CLASS_E_NOAGGREGATION;
        }
        *ppvObject = nullptr;
        return winrt::make<T>().as(riid, ppvObject);
    } catch (...) {
        return winrt::to_hresult();
    }

    HRESULT STDMETHODCALLTYPE LockServer(BOOL) noexcept override {
        return S_OK;
    }
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdll-attribute-on-redeclaration"

__declspec(dllexport) STDAPI DllGetClassObject(REFCLSID rclsid,
                                               REFIID riid,
                                               LPVOID* ppv) try {
    if (rclsid == CLSID_AppFoldersTAP) {
        *ppv = nullptr;
        return winrt::make<SimpleFactory<AppFoldersTAP>>().as(riid, ppv);
    }
    return CLASS_E_CLASSNOTAVAILABLE;
} catch (...) {
    return winrt::to_hresult();
}

#pragma clang diagnostic pop

using PFN_INITIALIZE_XAML_DIAGNOSTICS_EX =
    HRESULT(WINAPI*)(LPCWSTR endPointName,
                     DWORD pid,
                     LPCWSTR wszDllXamlDiagnostics,
                     LPCWSTR wszTAPDllName,
                     CLSID tapClsid,
                     LPCWSTR wszInitializationData);

HRESULT InjectTAP() {
    HMODULE module = GetCurrentModuleHandle();
    if (!module) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    WCHAR location[MAX_PATH];
    DWORD length = GetModuleFileNameW(module, location, ARRAYSIZE(location));
    if (length == 0 || length == ARRAYSIZE(location)) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    HMODULE wux = LoadLibraryExW(L"Windows.UI.Xaml.dll", nullptr,
                                 LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!wux) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    auto initializeXamlDiagnosticsEx =
        reinterpret_cast<PFN_INITIALIZE_XAML_DIAGNOSTICS_EX>(
            GetProcAddress(wux, "InitializeXamlDiagnosticsEx"));
    if (!initializeXamlDiagnosticsEx) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Other mods may be using some of the connections, try until one works.
    HRESULT hr = E_FAIL;
    for (int i = 0; i < 10000; i++) {
        WCHAR connectionName[64];
        wsprintfW(connectionName, L"VisualDiagConnection%d", i + 1);
        hr = initializeXamlDiagnosticsEx(connectionName, GetCurrentProcessId(),
                                         L"", location, CLSID_AppFoldersTAP,
                                         nullptr);
        if (hr != HRESULT_FROM_WIN32(ERROR_NOT_FOUND)) {
            break;
        }
    }

    return hr;
}

void InjectTAPOnce() {
    if (g_tapInjected.exchange(true)) {
        return;
    }

    HRESULT hr = InjectTAP();
    if (FAILED(hr)) {
        Wh_Log(L"Failed to inject the XAML diagnostics tap: %08X", hr);
        g_tapInjected = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Windows and threads

using RunFromWindowThreadProc_t = void(WINAPI*)(PVOID parameter);

bool RunFromWindowThread(HWND hWnd,
                         RunFromWindowThreadProc_t proc,
                         PVOID procParam) {
    static const UINT runFromWindowThreadRegisteredMsg =
        RegisterWindowMessageW(L"Windhawk_RunFromWindowThread_" WH_MOD_ID);

    struct RUN_FROM_WINDOW_THREAD_PARAM {
        RunFromWindowThreadProc_t proc;
        PVOID procParam;
    };

    DWORD threadId = GetWindowThreadProcessId(hWnd, nullptr);
    if (threadId == 0) {
        return false;
    }

    if (threadId == GetCurrentThreadId()) {
        proc(procParam);
        return true;
    }

    HHOOK hook = SetWindowsHookExW(
        WH_CALLWNDPROC,
        [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (nCode == HC_ACTION) {
                const CWPSTRUCT* cwp = reinterpret_cast<const CWPSTRUCT*>(lParam);
                if (cwp->message == runFromWindowThreadRegisteredMsg) {
                    auto* param =
                        reinterpret_cast<RUN_FROM_WINDOW_THREAD_PARAM*>(
                            cwp->lParam);
                    param->proc(param->procParam);
                }
            }
            return CallNextHookEx(nullptr, nCode, wParam, lParam);
        },
        nullptr, threadId);
    if (!hook) {
        return false;
    }

    RUN_FROM_WINDOW_THREAD_PARAM param{proc, procParam};
    SendMessageW(hWnd, runFromWindowThreadRegisteredMsg, 0,
                 reinterpret_cast<LPARAM>(&param));

    UnhookWindowsHookEx(hook);
    return true;
}

HWND FindCurrentProcessTaskbarWnd() {
    HWND taskbarWnd = nullptr;

    EnumWindows(
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            DWORD processId;
            WCHAR className[32];
            if (GetWindowThreadProcessId(hWnd, &processId) &&
                processId == GetCurrentProcessId() &&
                GetClassNameW(hWnd, className, ARRAYSIZE(className)) &&
                _wcsicmp(className, L"Shell_TrayWnd") == 0) {
                *reinterpret_cast<HWND*>(lParam) = hWnd;
                return FALSE;
            }
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&taskbarWnd));

    return taskbarWnd;
}

// The window hosting the taskbar's XAML island. It only exists with the
// Windows 11 taskbar.
HWND GetTaskbarUiWnd() {
    HWND taskbarWnd = FindCurrentProcessTaskbarWnd();
    if (!taskbarWnd) {
        return nullptr;
    }

    return FindWindowExW(taskbarWnd, nullptr,
                         L"Windows.UI.Composition.DesktopWindowContentBridge",
                         nullptr);
}

using CreateWindowExW_t = decltype(&CreateWindowExW);
CreateWindowExW_t CreateWindowExW_Original;
HWND WINAPI CreateWindowExW_Hook(DWORD dwExStyle,
                                 LPCWSTR lpClassName,
                                 LPCWSTR lpWindowName,
                                 DWORD dwStyle,
                                 int X,
                                 int Y,
                                 int nWidth,
                                 int nHeight,
                                 HWND hWndParent,
                                 HMENU hMenu,
                                 HINSTANCE hInstance,
                                 PVOID lpParam) {
    HWND hWnd = CreateWindowExW_Original(dwExStyle, lpClassName, lpWindowName,
                                         dwStyle, X, Y, nWidth, nHeight,
                                         hWndParent, hMenu, hInstance, lpParam);
    if (!hWnd || !hWndParent || g_unloading) {
        return hWnd;
    }

    WCHAR className[64];
    if (GetClassNameW(hWnd, className, ARRAYSIZE(className)) &&
        _wcsicmp(className,
                 L"Windows.UI.Composition.DesktopWindowContentBridge") == 0 &&
        GetClassNameW(hWndParent, className, ARRAYSIZE(className)) &&
        _wcsicmp(className, L"Shell_TrayWnd") == 0) {
        Wh_Log(L"Taskbar XAML island created, initializing");
        InitializeForCurrentThread();
        InjectTAPOnce();
    }

    return hWnd;
}

// Lets a left click on a folder reach the taskbar, so that the folder can be
// dragged, and turns the resulting launch into the folder menu. Optional: if
// the symbols can't be found, folders work but can't be dragged.
void HookTaskbarSymbols() {
    HMODULE taskbarModule =
        LoadLibraryExW(L"taskbar.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!taskbarModule) {
        Wh_Log(L"Couldn't load taskbar.dll");
        return;
    }

    WindhawkUtils::SYMBOL_HOOK taskbarDllHooks[] = {
        {
            {LR"(public: virtual long __cdecl CTaskBand::Launch(struct ITaskGroup *,struct tagPOINT const &,enum LaunchFromTaskbarOptions))"},
            &CTaskBand_Launch_Original,
            CTaskBand_Launch_Hook,
        },
    };

    g_launchHooked = WindhawkUtils::HookSymbols(
        taskbarModule, taskbarDllHooks, ARRAYSIZE(taskbarDllHooks));
    if (!g_launchHooked) {
        Wh_Log(L"Couldn't hook CTaskBand::Launch, folders can't be dragged");
    }
}

////////////////////////////////////////////////////////////////////////////////
// Mod entry points

BOOL Wh_ModInit() {
    Wh_Log(L">");

    g_settings = LoadSettings();
    LoadRemovedApps();
    PruneRemovedApps();

    WindhawkUtils::SetFunctionHook(CreateWindowExW, CreateWindowExW_Hook,
                                   &CreateWindowExW_Original);

    HookTaskbarSymbols();

    return TRUE;
}

void Wh_ModAfterInit() {
    Wh_Log(L">");

    HWND taskbarUiWnd = GetTaskbarUiWnd();
    if (!taskbarUiWnd) {
        Wh_Log(L"Taskbar XAML island not found yet");
        return;
    }

    RunFromWindowThread(
        taskbarUiWnd, [](PVOID) { InitializeForCurrentThread(); }, nullptr);
    InjectTAPOnce();
}

void Wh_ModSettingsChanged() {
    Wh_Log(L">");

    HWND taskbarUiWnd = GetTaskbarUiWnd();
    if (!taskbarUiWnd || !g_hwnd) {
        g_settings = LoadSettings();
        PruneRemovedApps();
        return;
    }

    RunFromWindowThread(
        taskbarUiWnd,
        [](PVOID) {
            Guarded([] {
                HideFolderMenu();
                g_settings = LoadSettings();
                PruneRemovedApps();
                g_folderIcons.clear();
                // Re-read app details too, e.g. for an app pinned since.
                g_appInfoCache.clear();
                g_pinnedShortcutsLoadTime = 0;
                UpdateAll();
            });
        },
        nullptr);
}

void Wh_ModUninit() {
    Wh_Log(L">");

    g_unloading = true;

    auto watcher = ExchangeVisualTreeWatcher(nullptr);
    if (watcher) {
        watcher->UnadviseVisualTreeChange();
    }

    // Running this on the UI thread also waits for any visual tree callback
    // that's still in progress there.
    HWND taskbarUiWnd = GetTaskbarUiWnd();
    if (taskbarUiWnd) {
        RunFromWindowThread(
            taskbarUiWnd, [](PVOID) { UninitializeForCurrentThread(); },
            nullptr);
    }

    // Only now that no callback can be running, release the watcher.
    watcher = nullptr;
}
