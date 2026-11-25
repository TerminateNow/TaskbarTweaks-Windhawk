// ==WindhawkMod==
// @id              taskbar-app-folders
// @name            Taskbar App Folders
// @description     Group pinned taskbar apps into folders that open a menu of their apps when clicked (Windows 11)
// @version         1.14
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
* When every app in a folder is open (for example, the only app of a folder
  with one app), the folder stays on the taskbar right in front of its first
  app, and each open app keeps its own taskbar icon. Click the folder to open
  its menu as usual.
* Hover over a folder to see all of its apps. The open ones are listed first,
  with a line between them and the rest.
* Clicking the folder opens its menu. Click an app to open it. With **Group
  open apps in the folder menu** (on by default), the menu also lists the open
  apps first, with a line after them.
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
  example "Open | 2". Clicking it in the menu switches to it, and its Close
  button (Close all, with more than one window) closes its windows. Apps
  still get to ask about unsaved work. An app that's only
  running in the background or the notification area (system tray) is also
  marked "Open", and clicking it brings up its window.
* Close also works for an app that's only running in the background, such as
  one in the notification area, like ending it in Task Manager. The app is
  asked to quit, and if it's still running 5 seconds later, its processes are
  ended. Apps running as administrator can't be closed this way. If the
  program has any window open, even on another virtual desktop, the folder
  menu shows no Close button for it, since its processes might be shared with
  that window (for example a browser's pinned web app). See "Browsers with
  pinned web apps" below.
* Some apps, such as Steam and Chrome, can show their windows under a separate
  taskbar icon from their pinned one. The folder menu still lists them once,
  under the pinned app.

## Keeping open apps apart from pinned apps

Turn on **Keep open apps apart from pinned apps** to split the taskbar into two
groups:

* Pinned apps and folders stay on the left, after the Start button.
* Open apps are a separate group in the middle of the taskbar, like the
  centered Windows 11 taskbar. That includes pinned apps while they're open:
  an open pinned app moves to the middle, and goes back to its pinned spot
  when it's closed. A folder stays with the pinned apps even while all its
  apps are open.

If the open apps don't fit in the middle, they move right as far as the
system tray, and then sit right after the pinned apps.

For the pinned apps to sit at the left edge, set **Taskbar alignment** to
**Left** in Windows' taskbar settings (Settings > Personalization > Taskbar >
Taskbar behaviors). With the default Center alignment, Windows centers
everything as one row, so the two groups are only kept a little apart.

Turn on **Separator after pinned apps** as well to draw a `|` line at the end
of the pinned apps and folders.

Drag pinned apps and folders to reorder them, as usual. A pinned app dragged
among the open apps goes back to the end of the pinned apps when it's dropped.

Open apps are dragged by the mod instead of by Windows: drag one to change its
place among the open apps. This order is the mod's own, so it never changes
where your pinned apps sit when they're closed. A newly opened app goes at
the end of the open apps, and an app keeps its place for as long as it's
open.

## Browsers with pinned web apps (PWAs)

A web app (PWA) installed from a browser such as Helium or Chrome runs in the
browser's own processes. While only the PWA is open, the browser looks like
it's running too, so its folder menu entry is marked "Open" without a Close
button, since closing it would also close the PWA.

To change that, list the browser under **Apps with PWAs** in the settings, in
the same format as a folder's apps, and choose a **PWA mode behavior**:

* **Show as closed**: while the browser is only running for its PWAs, it isn't
  marked "Open". Clicking it opens a new browser window.
* **Show as open, Close ends all**: it's marked "Open", and its Close all
  button closes the browser together with its PWAs, the same way as an app
  that's running in the background. Any unsaved work in the PWAs can be lost.

Apps not listed under Apps with PWAs aren't affected. Either way, a PWA you've
pinned keeps its own taskbar icon while it's open.

## Limitations

* Windows 11 taskbar only. The Windows 10 taskbar, and the Windows 10 taskbar
  restored by ExplorerPatcher, aren't supported.
* A folder sits on one of its closed apps' taskbar spots. While every app in a
  folder is open, there's no such spot, so the mod draws the folder icon
  itself, in space it makes in front of the folder's first app. That folder
  can't be dragged, and looks slightly different from real taskbar icons when
  hovered.
* Keyboard shortcuts such as Win+1 still launch the app the folder sits on.
* With **Keep open apps apart from pinned apps**, an open app is only drawn in
  its place among the open apps. Its jump list (right-click menu) is moved
  over it using Windows' debugging symbols for the taskbar; if those can't be
  found, it opens where the app would be without the option, such as next to
  its folder.
  While a pinned app or folder is being dragged, the groups aren't rearranged
  until it's dropped. The option is ignored on a taskbar laid out right to
  left.
* Dragging open apps relies on Windows' debugging symbols for the taskbar's
  buttons. If those can't be found, Windows drags open apps itself, which
  moves an open pinned app among the pinned apps.
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
- groupOpenApps: true
  $name: Group open apps in the folder menu
  $description: >-
    List a folder's open apps first in its menu, with a line between them and
    the rest of its apps.
- separateRunning: false
  $name: Keep open apps apart from pinned apps
  $description: >-
    Pinned apps and folders stay on the left, and open apps, including open
    pinned apps, are a separate group in the middle of the taskbar. For the
    pinned apps to sit at the left edge, set Taskbar alignment to Left in
    Windows' taskbar settings.
- pinnedSeparator: false
  $name: Separator after pinned apps
  $description: >-
    Draws a | line at the end of the pinned apps and folders. Needs Keep open
    apps apart from pinned apps.
- pwaMode: closed
  $name: PWA mode behavior
  $description: >-
    What the folder menu shows for an app listed under Apps with PWAs while
    it's only running for its PWAs' windows. Show as closed: it isn't marked
    Open, and clicking it opens a new window. Show as open, Close ends all: it's
    marked Open, and its Close button closes it together with its PWAs.
  $options:
  - closed: Show as closed
  - closeAll: Show as open, Close ends all
- pwaApps: [""]
  $name: Apps with PWAs
  $description: >-
    Browsers whose installed web apps (PWAs) you've pinned to the taskbar, for
    example Helium or chrome.exe. Same format as a folder's apps. PWA mode
    behavior applies only to these.
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

// Background apps closed from the folder menu are asked to quit, and ended if
// they're still running this long after.
constexpr UINT_PTR kTimerEndBackgroundApps = 1;
constexpr UINT kEndBackgroundAppsDelayMs = 5000;

// How far the pointer can move between pressing and releasing on a folder for
// it to still count as a click rather than a drag.
constexpr int kClickMoveThreshold = 12;

// Open apps kept apart from pinned apps (separateRunning), in DIPs: the
// smallest space between the two groups, and the separator line drawn after
// the pinned apps (pinnedSeparator).
constexpr double kMinSectionGap = 8;
constexpr double kSeparatorWidth = 2;
constexpr double kSeparatorPadding = 8;
constexpr double kSeparatorHeightRatio = 0.45;
constexpr double kSeparatorOpacity = 0.55;

constexpr UINT kCmdLaunchBase = 1000;
constexpr UINT kCmdRemoveBase = 2000;
constexpr UINT kCmdRestoreBase = 3000;
constexpr UINT kCmdClearRemoved = 4000;
constexpr UINT kCmdCloseBase = 5000;
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

// Microsoft.UI.Xaml.Controls.IItemsRepeater (WinUI 2), the interface of the
// taskbar's TaskbarFrameRepeater. Its GetElementIndex is vtable entry 19.
constexpr GUID kIID_IItemsRepeater = {
    0x0bd894f2,
    0xedfc,
    0x5ddf,
    {0xa1, 0x66, 0x2d, 0xb1, 0x4b, 0xbf, 0xdf, 0x35}};

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
    // Open apps first in the folder menu, with a separator after them.
    bool groupOpenApps = true;
    // Apps listed under Apps with PWAs, normalized like FolderConfig::matchKeys.
    std::vector<std::wstring> pwaMatchKeys;
    // PWA mode behavior: true for "Show as open, Close ends all".
    bool pwaCloseAll = false;
    // Keep open apps apart from pinned apps, and draw a separator after the
    // pinned apps.
    bool separateRunning = false;
    bool pinnedSeparator = false;
};

Settings g_settings;

// Set while folder entries are collected for a tooltip, which happens on every
// hover, so that they aren't logged each time.
bool g_quietMenuLog = false;
#define MENU_LOG(...)                \
    do {                             \
        if (!g_quietMenuLog) {       \
            Wh_Log(__VA_ARGS__);     \
        }                            \
    } while (0)

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
    // The taskbar keeps the buttons of closed windows for reuse, parked out of
    // sight with their last app ID. Those are ignored.
    bool recycled = false;
    int folder = -1;
    int order = 0;

    bool isHost = false;
    // Every app of the folder is open, and this is the first one: the mod
    // draws the folder in front of it.
    bool carriesFolder = false;
    bool collapsedByUs = false;
    bool iconOverridden = false;
    bool applyingIcon = false;
    wux::Media::ImageSource originalIcon{nullptr};

    // A host's tooltip lists the folder's apps: the taskbar's own tooltip,
    // whose content is replaced when it opens, or one the mod set if the
    // button had none (ownToolTipSet, with the button's tooltip value kept).
    winrt::weak_ref<wuxc::ToolTip> hostToolTip;
    winrt::event_token hostToolTipOpenedToken{};
    wf::IInspectable originalToolTipContent{nullptr};
    wf::IInspectable folderToolTipContent{nullptr};
    bool ownToolTipSet = false;
    wf::IInspectable originalToolTip{nullptr};

    // Open apps kept apart from pinned apps. The repeater the button is laid
    // out in, the margin the mod set on top of the button's own (baseMargin),
    // and the shift that draws an open pinned app among the open apps.
    void* repeaterKey = nullptr;
    bool marginOverridden = false;
    wux::Thickness baseMargin{};
    wux::Thickness appliedMargin{};
    // The open app's place among the open apps (separateRunning), and the
    // app it was given for. 0 when it isn't open.
    uint64_t runningRank = 0;
    std::wstring rankAppIdLower;
    // Where the open app was last placed, in the repeater's coordinates.
    double plannedLeft = 0;
    bool zIndexRaised = false;
    int originalZIndex = 0;
    bool transformOverridden = false;
    wux::Media::Transform originalTransform{nullptr};
    wux::Media::TranslateTransform translate{nullptr};
    wux::Media::TransformGroup transformGroup{nullptr};
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
    // The windows counted in windowCount, for closing them.
    std::vector<HWND> windows;
    // Set when the app is open without any windows (e.g. only in the
    // notification area): the program whose processes Close ends.
    std::wstring backgroundExe;
    // Listed under Apps with PWAs: a browser whose pinned web apps run in its
    // processes.
    bool pwaHost = false;
    // Close ends backgroundExe even though another app's windows (its PWAs)
    // run in its processes. Set by PWA mode behavior "Close ends all".
    bool closeEndsOtherApps = false;
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

// Where the taskbar opens a button's jump list (right-click menu). It goes by
// where the button is laid out, so for an app the mod draws elsewhere (an open
// app moved among the open apps), it's moved over the app as drawn.
using CTaskListWnd_ComputeJumpViewPosition_t =
    HRESULT(WINAPI*)(void* pThis,
                     void* taskBtnGroup,
                     int param2,
                     wf::Point* point,
                     wux::HorizontalAlignment* horizontalAlignment,
                     wux::VerticalAlignment* verticalAlignment);
CTaskListWnd_ComputeJumpViewPosition_t
    CTaskListWnd_ComputeJumpViewPosition_Original;

// Set by a right press on a button the mod moved: the middle of the button as
// drawn, in screen pixels, used by the next jump list opened soon after.
std::atomic<ULONGLONG> g_jumpListAnchorTime = 0;
std::atomic<LONG> g_jumpListAnchorX = 0;
constexpr ULONGLONG kJumpListAnchorTimeoutMs = 3000;
bool g_launchHooked = false;

// Open apps are dragged by the mod while they're kept apart from pinned apps
// (separateRunning): the taskbar's own drag would move them among the pinned
// apps, where they're laid out. These are the taskbar button's own pointer
// handlers, in Taskbar.View.dll.
using TaskListButton_OnPointer_t = int(WINAPI*)(void* pThis, void* pArgs);
TaskListButton_OnPointer_t TaskListButton_OnPointerPressed_Original;
TaskListButton_OnPointer_t TaskListButton_OnPointerMoved_Original;
TaskListButton_OnPointer_t TaskListButton_OnPointerReleased_Original;
TaskListButton_OnPointer_t TaskListButton_OnPointerCaptureLost_Original;
std::atomic<bool> g_dragHooked = false;
std::atomic<bool> g_taskbarViewHooksDone = false;

// A left press on an open app, which becomes a drag once the pointer moves
// far enough.
struct RunningDrag {
    bool armed = false;
    bool dragging = false;
    void* key = nullptr;
    uint32_t pointerId = 0;
    // The pointer's x when pressed and now, and where the app was placed when
    // the drag started, in the repeater's coordinates.
    double startX = 0;
    double currentX = 0;
    double startLeft = 0;
};

RunningDrag g_runningDrag;
uint64_t g_lastRunningRank = 0;

// How far the pointer has to move for a press on an open app to become a drag.
constexpr double kDragThreshold = 6;

// Apps seen with a closed taskbar button, which means they're pinned. A folder
// whose apps are all open is only drawn in front of a pinned app.
std::vector<std::wstring> g_closedPinnedAppIds;

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
    settings.groupOpenApps = Wh_GetIntSetting(L"groupOpenApps") != 0;

    for (int i = 0; i < kMaxAppsPerFolder; i++) {
        std::wstring app = Trim(GetStringSetting(L"pwaApps[%d]", i));
        if (app.empty()) {
            break;
        }
        settings.pwaMatchKeys.push_back(NormalizeAppEntry(app));
    }
    settings.pwaCloseAll = GetStringSetting(L"pwaMode") == L"closeAll";

    settings.separateRunning = Wh_GetIntSetting(L"separateRunning") != 0;
    settings.pinnedSeparator = Wh_GetIntSetting(L"pinnedSeparator") != 0;

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

// Like the upstream taskbar mods, a button laid out left of or above the
// taskbar is taken as one kept for reuse. A button the mod collapsed isn't laid
// out at all, so its position says nothing.
bool IsRecycled(ButtonState* button, wux::FrameworkElement const& element) {
    if (button->collapsedByUs) {
        return false;
    }

    auto offset = element.ActualOffset();
    return offset.x + element.ActualWidth() < 0 ||
           offset.y + element.ActualHeight() < 0;
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
    HWND hWnd = nullptr;
    std::wstring appIdLower;   // The window's own app ID, if it sets one.
    std::wstring exePathLower;
};

struct RunningProcess {
    DWORD id = 0;
    std::wstring pathLower;
};

struct OpenAppsSnapshot {
    // Windows the taskbar shows on the current desktop.
    std::vector<OpenWindow> windows;
    // Windows that the taskbar shows only on another virtual desktop.
    std::vector<OpenWindow> otherDesktopWindows;
    // Running processes in this session.
    std::vector<RunningProcess> processes;
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

// Roughly the windows that get a taskbar button: visible, titled, and either
// unowned or marked as app windows. Cloaked ones aren't shown on the taskbar
// right now. cloak gets the reasons, as DWM_CLOAKED_* flags.
bool IsTaskbarStyleWindow(HWND hWnd, DWORD* cloak) {
    *cloak = 0;
    if (!IsWindowVisible(hWnd) || GetWindowTextLengthW(hWnd) == 0) {
        return false;
    }

    LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
    bool appWindow = exStyle & WS_EX_APPWINDOW;
    if (!appWindow &&
        ((exStyle & WS_EX_TOOLWINDOW) || GetWindow(hWnd, GW_OWNER))) {
        return false;
    }

    if (FAILED(DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, cloak,
                                     sizeof(*cloak)))) {
        *cloak = 0;
    }
    return true;
}

bool IsInThisSession(DWORD processId) {
    static DWORD sessionId = [] {
        DWORD id = 0;
        ProcessIdToSessionId(GetCurrentProcessId(), &id);
        return id;
    }();

    DWORD id = 0;
    return ProcessIdToSessionId(processId, &id) && id == sessionId;
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
                DWORD processId = entry.th32ProcessID;
                if (!IsInThisSession(processId)) {
                    continue;
                }
                std::wstring path = GetProcessPathLower(processId);
                if (!path.empty()) {
                    processPaths[processId] = path;
                    snapshot.processes.push_back({processId, std::move(path)});
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
            DWORD cloak;
            if (!IsTaskbarStyleWindow(hWnd, &cloak)) {
                return TRUE;
            }

            // Cloaked only by its own app: not shown anywhere, so not really
            // open. Apps cloak windows they're preparing or keeping ready to
            // show.
            if (cloak && !(cloak & (DWM_CLOAKED_SHELL | DWM_CLOAKED_INHERITED))) {
                return TRUE;
            }

            DWORD processId = 0;
            GetWindowThreadProcessId(hWnd, &processId);
            auto it = param->processPaths->find(processId);
            std::wstring exePathLower = it != param->processPaths->end()
                                            ? it->second
                                            : GetProcessPathLower(processId);

            OpenWindow window;
            window.hWnd = hWnd;
            window.appIdLower = GetWindowAppIdLower(hWnd);
            window.exePathLower = std::move(exePathLower);
            // Cloaked by the shell: on another virtual desktop.
            (cloak ? param->snapshot->otherDesktopWindows
                   : param->snapshot->windows)
                .push_back(std::move(window));
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

void LogProgramWindow(const OpenWindow& window) {
    WCHAR className[256] = L"";
    GetClassNameW(window.hWnd, className, ARRAYSIZE(className));
    WCHAR title[256] = L"";
    GetWindowTextW(window.hWnd, title, ARRAYSIZE(title));
    DWORD cloak = 0;
    DwmGetWindowAttribute(window.hWnd, DWMWA_CLOAKED, &cloak, sizeof(cloak));
    MENU_LOG(L"  window %p: class '%s', title '%s', app ID '%s', cloak %u",
           window.hWnd, className, title, window.appIdLower.c_str(), cloak);
}

// File Explorer windows belong to explorer.exe, which is also the shell itself,
// so they can only be told apart by app ID. That also keeps explorer.exe from
// ever being ended as a background app.
bool MatchesByProgram(const std::wstring& exe) {
    return !exe.empty() && FileNameOf(exe) != L"explorer.exe";
}

// The windows that the taskbar shows for an entry's app and its program, by
// whose they are.
struct ProgramWindows {
    // The entry's own windows on this desktop: the ones counted and closed.
    std::vector<const OpenWindow*> own;
    // One of its own windows on another virtual desktop, if any.
    const OpenWindow* ownElsewhere = nullptr;
    // A window of the same program that belongs to another pinned app, e.g. a
    // browser's pinned web app (PWA), on any desktop.
    const OpenWindow* otherApp = nullptr;
};

ProgramWindows FindProgramWindows(const MenuEntry& entry,
                                  const std::wstring& exe,
                                  const OpenAppsSnapshot& snapshot) {
    std::wstring appIdLower = ToLower(entry.appId);
    std::vector<std::wstring> aliasesLower;
    for (const auto& alias : entry.aliasAppIds) {
        aliasesLower.push_back(ToLower(alias));
    }
    bool matchByProgram = MatchesByProgram(exe);

    ProgramWindows result;
    for (const auto* list : {&snapshot.windows, &snapshot.otherDesktopWindows}) {
        bool thisDesktop = list == &snapshot.windows;
        for (const auto& window : *list) {
            bool own =
                window.appIdLower == appIdLower ||
                std::find(aliasesLower.begin(), aliasesLower.end(),
                          window.appIdLower) != aliasesLower.end();
            bool sameProgram =
                matchByProgram && IsSameProgram(exe, window.exePathLower);

            // A window of the same program is the app's too, unless it
            // belongs to another pinned app.
            if (!own && sameProgram) {
                own = window.appIdLower.empty() ||
                      !IsPinnedAppId(window.appIdLower);
                if (!own && !result.otherApp) {
                    result.otherApp = &window;
                }
            }

            if (own) {
                if (thisDesktop) {
                    result.own.push_back(&window);
                } else if (!result.ownElsewhere) {
                    result.ownElsewhere = &window;
                }
            }
        }
    }
    return result;
}

void CountOpenWindows(MenuEntry& entry,
                      const std::wstring& exe,
                      const OpenAppsSnapshot& snapshot) {
    ProgramWindows windows = FindProgramWindows(entry, exe, snapshot);
    for (const OpenWindow* window : windows.own) {
        entry.windowCount++;
        entry.windows.push_back(window->hWnd);
    }

    bool processRunning = false;
    if (MatchesByProgram(exe)) {
        for (const auto& process : snapshot.processes) {
            if (IsSameProgram(exe, process.pathLower)) {
                processRunning = true;
                break;
            }
        }
    }

    entry.running = entry.running || entry.windowCount > 0 || processRunning;

    if (!processRunning || entry.windowCount > 0) {
        return;
    }

    // Running without a window of its own, e.g. only in the notification
    // area, so Close has to end its processes.
    PCWSTR noCloseReason = nullptr;
    const OpenWindow* blocking = nullptr;
    if (entry.buttonKey) {
        noCloseReason = L"it has a taskbar button";
    } else if (windows.ownElsewhere) {
        noCloseReason = L"it has a window on another desktop";
        blocking = windows.ownElsewhere;
    } else if (!windows.otherApp) {
        entry.backgroundExe = exe;
    } else if (!entry.pwaHost) {
        // Its processes also run another app's windows, which ending them
        // would close too.
        noCloseReason = L"its program has another app's window";
        blocking = windows.otherApp;
    } else if (g_settings.pwaCloseAll) {
        entry.backgroundExe = exe;
        entry.closeEndsOtherApps = true;
        MENU_LOG(L"%s is only running for its PWAs: Close ends them too",
               entry.appId.c_str());
    } else {
        entry.running = false;
        MENU_LOG(L"%s is only running for its PWAs: shown as closed",
               entry.appId.c_str());
    }

    if (noCloseReason) {
        MENU_LOG(L"%s runs without windows of its own, but can't be closed "
               L"from the menu: %s",
               entry.appId.c_str(), noCloseReason);
        if (blocking) {
            LogProgramWindow(*blocking);
        }
    }
}

// The names a taskbar button's app goes by, to match against the app entries
// in the settings.
struct AppNames {
    explicit AppNames(ButtonState* button) {
        const AppInfo& info = GetAppInfo(button->appId);
        appIdLower = button->appIdLower;
        exeLower = info.exeLower;
        exeStem = StripExe(info.exeLower);
        displayNameLower = ToLower(info.displayName);
        shortcutNameLower = ToLower(info.shortcutName);
        automationNameLower = ToLower(button->automationName);
        // In case the accessible name has extra details after the app's name.
        automationNameShortLower = automationNameLower.substr(
            0, automationNameLower.find(L" - "));
    }

    // matchKey is an entry from the settings, normalized with
    // NormalizeAppEntry.
    bool Matches(const std::wstring& matchKey) const {
        return matchKey == appIdLower ||
               (!exeLower.empty() &&
                (matchKey == exeLower || matchKey == exeStem)) ||
               (!displayNameLower.empty() && matchKey == displayNameLower) ||
               (!shortcutNameLower.empty() && matchKey == shortcutNameLower) ||
               (!automationNameLower.empty() &&
                (matchKey == automationNameLower ||
                 matchKey == automationNameShortLower));
    }

    std::wstring appIdLower;
    std::wstring exeLower;
    std::wstring exeStem;
    std::wstring displayNameLower;
    std::wstring shortcutNameLower;
    std::wstring automationNameLower;
    std::wstring automationNameShortLower;
};

// Whether the button's app is listed under Apps with PWAs.
bool IsPwaHost(ButtonState* button) {
    if (g_settings.pwaMatchKeys.empty() || button->appId.empty()) {
        return false;
    }

    AppNames names(button);
    for (const auto& matchKey : g_settings.pwaMatchKeys) {
        if (names.Matches(matchKey)) {
            return true;
        }
    }
    return false;
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
        if (button->folder != host->folder ||
            button->rootKey != host->rootKey || button->appId.empty()) {
            continue;
        }

        // A button may have been put aside since the last update.
        auto element = button->element.get();
        if (!element || IsRecycled(button.get(), element)) {
            continue;
        }

        members.push_back(button.get());
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
        entry.pwaHost = IsPwaHost(member);
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
            entry.pwaHost = entry.pwaHost || IsPwaHost(member);
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
        entry.pwaHost = IsPwaHost(member);
        entries.push_back(std::move(entry));
        entryExes.push_back(exe);
        entryOrders.push_back(member->order);
    }

    // Entries that matched the same entry of the folder's app list are the same
    // app under different app IDs, e.g. Chrome's profile picker and its
    // windows. Show them once.
    for (size_t i = 0; i < entries.size(); i++) {
        for (size_t j = i + 1; j < entries.size();) {
            if (entryOrders[j] != entryOrders[i]) {
                j++;
                continue;
            }

            MenuEntry& kept = entries[i];
            MenuEntry& extra = entries[j];
            kept.aliasAppIds.push_back(extra.appId);
            kept.aliasAppIds.insert(kept.aliasAppIds.end(),
                                    extra.aliasAppIds.begin(),
                                    extra.aliasAppIds.end());
            kept.running = kept.running || extra.running;
            kept.pwaHost = kept.pwaHost || extra.pwaHost;
            if (!kept.buttonKey) {
                kept.buttonKey = extra.buttonKey;
            }
            if (entryExes[i].empty()) {
                entryExes[i] = entryExes[j];
            }

            entries.erase(entries.begin() + j);
            entryExes.erase(entryExes.begin() + j);
            entryOrders.erase(entryOrders.begin() + j);
        }
    }

    for (size_t i = 0; i < entries.size(); i++) {
        CountOpenWindows(entries[i], entryExes[i], snapshot);
        MENU_LOG(L"Menu entry %s: order %d, %s, %d windows, %zu other app IDs",
               entries[i].appId.c_str(), entryOrders[i],
               entries[i].running ? L"open" : L"closed",
               entries[i].windowCount, entries[i].aliasAppIds.size());
        for (const auto& alias : entries[i].aliasAppIds) {
            MENU_LOG(L"  also %s", alias.c_str());
        }
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

// Processes of background apps that were asked to quit, to be ended when
// kTimerEndBackgroundApps fires if they're still running.
std::vector<HANDLE> g_endingProcesses;

void ReleaseEndingProcesses() {
    for (HANDLE process : g_endingProcesses) {
        CloseHandle(process);
    }
    g_endingProcesses.clear();
}

void EndRemainingBackgroundApps() {
    if (g_hwnd) {
        KillTimer(g_hwnd, kTimerEndBackgroundApps);
    }

    for (HANDLE process : g_endingProcesses) {
        if (WaitForSingleObject(process, 0) != WAIT_TIMEOUT) {
            continue;
        }
        DWORD processId = GetProcessId(process);
        if (TerminateProcess(process, 1)) {
            Wh_Log(L"Ended process %u", processId);
        } else {
            Wh_Log(L"Failed to end process %u: %u", processId, GetLastError());
        }
    }

    ReleaseEndingProcesses();
}

// Closes an app that runs without windows, e.g. only in the notification area,
// like Task Manager's End task: each of its processes is asked to quit, the
// way taskkill does, by posting WM_CLOSE to its top-level windows (which are
// all hidden, or with Close ends all, its PWAs' windows). Those still running
// a few seconds later are ended.
void CloseBackgroundApp(const MenuEntry& entry) {
    const std::wstring& exe = entry.backgroundExe;

    // The menu may have been open for a while. Leave the app alone if it got a
    // window since, as ending it now could lose work in that window. Other
    // apps' windows in its processes (its PWAs) only count without Close ends
    // all.
    OpenAppsSnapshot snapshot = TakeOpenAppsSnapshot();
    ProgramWindows windows = FindProgramWindows(entry, exe, snapshot);
    const OpenWindow* blocking = windows.ownElsewhere;
    if (!windows.own.empty()) {
        blocking = windows.own.front();
    } else if (!blocking && !entry.closeEndsOtherApps) {
        blocking = windows.otherApp;
    }
    if (blocking) {
        Wh_Log(L"Not closing %s: it has windows now", exe.c_str());
        LogProgramWindow(*blocking);
        return;
    }

    std::vector<DWORD> processIds;
    for (const auto& process : snapshot.processes) {
        if (process.id != GetCurrentProcessId() &&
            IsSameProgram(exe, process.pathLower)) {
            processIds.push_back(process.id);
        }
    }

    EnumWindows(
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            const auto& processIds =
                *reinterpret_cast<std::vector<DWORD>*>(lParam);
            DWORD processId = 0;
            GetWindowThreadProcessId(hWnd, &processId);
            if (std::find(processIds.begin(), processIds.end(), processId) !=
                processIds.end()) {
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
            }
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&processIds));

    for (DWORD processId : processIds) {
        HANDLE process = OpenProcess(
            SYNCHRONIZE | PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION,
            FALSE, processId);
        if (!process) {
            Wh_Log(L"Can't end process %u: %u", processId, GetLastError());
            continue;
        }

        // Make sure the ID wasn't reused for another program in the meantime.
        WCHAR path[MAX_PATH * 2];
        DWORD size = ARRAYSIZE(path);
        if (!QueryFullProcessImageNameW(process, 0, path, &size) ||
            !IsSameProgram(exe, ToLower(std::wstring_view(path, size)))) {
            CloseHandle(process);
            continue;
        }

        g_endingProcesses.push_back(process);
    }

    Wh_Log(L"Closing %s: asked %zu processes of %s to quit", entry.appId.c_str(),
           processIds.size(), exe.c_str());

    if (!g_endingProcesses.empty()) {
        if (!g_hwnd || !SetTimer(g_hwnd, kTimerEndBackgroundApps,
                                 kEndBackgroundAppsDelayMs, nullptr)) {
            ReleaseEndingProcesses();
        }
    }
}

// Asks each of the app's windows to close, as the window's own close button
// does, so apps can still ask about unsaved work. An app running without
// windows is closed with CloseBackgroundApp.
void CloseMenuEntry(const MenuEntry& entry) {
    if (entry.windows.empty()) {
        if (!entry.backgroundExe.empty()) {
            CloseBackgroundApp(entry);
        }
        return;
    }

    for (HWND hWnd : entry.windows) {
        if (IsWindow(hWnd)) {
            PostMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        }
    }

    Wh_Log(L"Closed %zu windows of %s", entry.windows.size(),
           entry.appId.c_str());
}

bool CanClose(const MenuEntry& entry) {
    return !entry.windows.empty() || !entry.backgroundExe.empty();
}

std::wstring CloseLabel(const MenuEntry& entry) {
    return entry.windowCount >= 2 || entry.closeEndsOtherApps ? L"Close all"
                                                              : L"Close";
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

wuxc::Border MenuSeparator() {
    wuxc::Border separator;
    separator.Height(1);
    separator.Margin(wux::Thickness{8, 6, 8, 6});
    separator.Background(wux::Media::SolidColorBrush(
        winrt::Windows::UI::Color{0x40, 0x80, 0x80, 0x80}));
    return separator;
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

    for (size_t entryIndex = 0; entryIndex < entries.size(); entryIndex++) {
        const auto& entry = entries[entryIndex];
        std::wstring appId = entry.appId;

        // Between the open apps, listed first, and the rest.
        if (g_settings.groupOpenApps && entryIndex > 0 && !entry.running &&
            entries[entryIndex - 1].running) {
            panel.Children().Append(MenuSeparator());
        }

        wuxc::Grid row;
        wuxc::ColumnDefinition nameColumn;
        nameColumn.Width(wux::GridLength{1, wux::GridUnitType::Star});
        wuxc::ColumnDefinition closeColumn;
        closeColumn.Width(wux::GridLength{1, wux::GridUnitType::Auto});
        wuxc::ColumnDefinition removeColumn;
        removeColumn.Width(wux::GridLength{1, wux::GridUnitType::Auto});
        row.ColumnDefinitions().Append(nameColumn);
        row.ColumnDefinitions().Append(closeColumn);
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

        wuxc::Grid::SetColumn(removeButton, 2);
        row.Children().Append(openButton);
        row.Children().Append(removeButton);

        if (CanClose(entry)) {
            wuxc::TextBlock closeLabel;
            closeLabel.Text(winrt::hstring(CloseLabel(entry)));
            closeLabel.FontSize(12);

            wuxc::Button closeButton;
            closeButton.Content(closeLabel);
            closeButton.VerticalAlignment(wux::VerticalAlignment::Stretch);
            closeButton.Background(TransparentBrush());
            closeButton.BorderThickness(wux::Thickness{0, 0, 0, 0});
            closeButton.Padding(wux::Thickness{8, 6, 8, 6});
            std::wstring tooltip =
                entry.windowCount >= 2
                    ? L"Close all " + std::to_wstring(entry.windowCount) +
                          L" " + entry.name + L" windows"
                : entry.closeEndsOtherApps
                    ? L"Close " + entry.name + L" and its PWAs"
                : entry.windows.empty()
                    ? L"Close " + entry.name + L", running in the background"
                    : L"Close " + entry.name;
            wuxc::ToolTipService::SetToolTip(
                closeButton, winrt::box_value(winrt::hstring(tooltip)));
            wux::Automation::AutomationProperties::SetName(
                closeButton, winrt::hstring(tooltip));
            closeButton.Click([entry](auto const&, auto const&) {
                Guarded([&] {
                    HideFolderMenu();
                    CloseMenuEntry(entry);
                });
            });

            wuxc::Grid::SetColumn(closeButton, 1);
            row.Children().Append(closeButton);
        }

        panel.Children().Append(row);
    }

    if (!removed.empty()) {
        panel.Children().Append(MenuSeparator());

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
        if (g_settings.groupOpenApps && i > 0 && !pending.entries[i].running &&
            pending.entries[i - 1].running) {
            AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        }

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

    bool anyClosable = false;
    for (size_t i = 0; i < pending.entries.size(); i++) {
        if (!CanClose(pending.entries[i])) {
            continue;
        }
        anyClosable = true;
        std::wstring text =
            CloseLabel(pending.entries[i]) + L" " + pending.entries[i].name;
        AppendMenuW(menu, MF_STRING, kCmdCloseBase + static_cast<UINT>(i),
                    text.c_str());
    }

    if (anyClosable) {
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    }

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
    if (command >= kCmdCloseBase && command < kCmdCloseBase + count) {
        CloseMenuEntry(pending.entries[command - kCmdCloseBase]);
    } else if (command == kCmdClearRemoved) {
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

// Shows the folder's menu. anchor is the element it opens from: by default
// the host button, or the folder the mod draws when all its apps are open.
void ShowFolderMenu(ButtonState* host,
                    wux::FrameworkElement const& anchor = nullptr) {
    auto element = anchor ? anchor : host->element.get();
    if (!element || host->folder < 0) {
        return;
    }

    auto entries = CollectMenuEntries(host);
    if (entries.empty()) {
        return;
    }

    if (g_settings.groupOpenApps) {
        std::stable_partition(entries.begin(), entries.end(),
                              [](MenuEntry const& entry) {
                                  return entry.running;
                              });
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

void RestoreHostToolTip(ButtonState* button);

// The content of a folder's tooltip: its name and all its apps, the open
// ones first with a line after them. Built each time the tooltip opens, from
// the same entries as the folder menu.
wux::UIElement BuildFolderToolTipContent(ButtonState* host) {
    std::vector<MenuEntry> entries;
    g_quietMenuLog = true;
    try {
        entries = CollectMenuEntries(host);
    } catch (...) {
        g_quietMenuLog = false;
        throw;
    }
    g_quietMenuLog = false;

    std::stable_partition(
        entries.begin(), entries.end(),
        [](MenuEntry const& entry) { return entry.running; });

    wuxc::StackPanel panel;
    panel.Spacing(4);

    wuxc::TextBlock title;
    title.Text(winrt::hstring(FolderDisplayName(host->folder)));
    title.FontWeight(winrt::Windows::UI::Text::FontWeight{600});
    panel.Children().Append(title);

    constexpr size_t kMaxToolTipEntries = 30;
    for (size_t i = 0; i < entries.size() && i < kMaxToolTipEntries; i++) {
        const auto& entry = entries[i];

        if (i > 0 && !entry.running && entries[i - 1].running) {
            wuxc::Border separator;
            separator.Height(1);
            separator.Margin(wux::Thickness{0, 2, 0, 2});
            separator.Background(wux::Media::SolidColorBrush(
                winrt::Windows::UI::Color{0x60, 0x80, 0x80, 0x80}));
            panel.Children().Append(separator);
        }

        wuxc::StackPanel row;
        row.Orientation(wuxc::Orientation::Horizontal);
        row.Spacing(8);

        if (auto icon = GetAppIcon(entry.appId)) {
            wuxc::Image image;
            image.Source(icon);
            image.Width(16);
            image.Height(16);
            row.Children().Append(image);
        }

        wuxc::TextBlock name;
        name.Text(winrt::hstring(entry.name));
        name.VerticalAlignment(wux::VerticalAlignment::Center);
        row.Children().Append(name);

        if (entry.running) {
            wuxc::TextBlock openLabel;
            openLabel.Text(winrt::hstring(OpenLabel(entry)));
            openLabel.FontSize(11);
            openLabel.Opacity(0.6);
            openLabel.VerticalAlignment(wux::VerticalAlignment::Center);
            row.Children().Append(openLabel);
        }

        panel.Children().Append(row);
    }

    if (entries.size() > kMaxToolTipEntries) {
        wuxc::TextBlock more;
        more.Text(winrt::hstring(
            L"+" + std::to_wstring(entries.size() - kMaxToolTipEntries) +
            L" more"));
        more.Opacity(0.6);
        panel.Children().Append(more);
    }

    return panel;
}

void OnHostToolTipOpened(void* key, wf::IInspectable const& sender) {
    ButtonState* button = FindButton(key);
    if (!button || !button->isHost) {
        return;
    }

    auto toolTip = sender.as<wuxc::ToolTip>();
    auto content = toolTip.Content();
    // Whatever the taskbar put in since is what goes back later.
    if (!button->folderToolTipContent || !content ||
        CanonicalPtr(content) != CanonicalPtr(button->folderToolTipContent)) {
        button->originalToolTipContent = content;
    }

    auto folderContent = BuildFolderToolTipContent(button);
    button->folderToolTipContent = folderContent;
    toolTip.Content(folderContent);
}

// Makes a host's tooltip list its folder's apps. Called on every update, and
// does nothing while that's already set up.
void ApplyHostToolTip(ButtonState* button,
                      wux::FrameworkElement const& element) {
    auto current = wuxc::ToolTipService::GetToolTip(element);
    auto toolTip = current ? current.try_as<wuxc::ToolTip>() : nullptr;

    auto applied = button->hostToolTip.get();
    if (applied && toolTip &&
        CanonicalPtr(applied) == CanonicalPtr(toolTip)) {
        return;
    }

    // The taskbar replaced its tooltip since.
    if (applied) {
        RestoreHostToolTip(button);
        current = wuxc::ToolTipService::GetToolTip(element);
        toolTip = current ? current.try_as<wuxc::ToolTip>() : nullptr;
    }

    if (!toolTip) {
        // No tooltip object to fill in: the mod sets its own.
        button->originalToolTip = current;
        toolTip = wuxc::ToolTip();
        toolTip.Content(
            winrt::box_value(winrt::hstring(FolderDisplayName(button->folder))));
        wuxc::ToolTipService::SetToolTip(element, toolTip);
        button->ownToolTipSet = true;
    }

    void* key = button->key;
    button->hostToolTip = winrt::make_weak(toolTip);
    button->hostToolTipOpenedToken = toolTip.Opened(
        [key](wf::IInspectable const& sender, auto const&) {
            Guarded([&] { OnHostToolTipOpened(key, sender); });
        });
}

// Puts the taskbar's tooltip back as it was.
void RestoreHostToolTip(ButtonState* button) {
    Guarded([&] {
        auto toolTip = button->hostToolTip.get();
        if (!toolTip) {
            return;
        }

        if (button->hostToolTipOpenedToken) {
            toolTip.Opened(button->hostToolTipOpenedToken);
        }

        auto content = toolTip.Content();
        if (button->folderToolTipContent && content &&
            CanonicalPtr(content) ==
                CanonicalPtr(button->folderToolTipContent)) {
            toolTip.Content(button->originalToolTipContent);
        }

        auto element = button->element.get();
        if (button->ownToolTipSet && element) {
            auto current = wuxc::ToolTipService::GetToolTip(element);
            if (current && CanonicalPtr(current) == CanonicalPtr(toolTip)) {
                wuxc::ToolTipService::SetToolTip(element,
                                                 button->originalToolTip);
            }
        }
    });

    button->hostToolTip = nullptr;
    button->hostToolTipOpenedToken = {};
    button->originalToolTipContent = nullptr;
    button->folderToolTipContent = nullptr;
    button->ownToolTipSet = false;
    button->originalToolTip = nullptr;
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
// A right press: if the mod draws the button away from where it's laid out,
// the jump list it opens is moved over the button as drawn. Any other right
// press clears that, so jump lists of other buttons open as usual.
void SetJumpListAnchor(ButtonState* button,
                       wux::FrameworkElement const& element,
                       wf::Point position) {
    g_jumpListAnchorTime = 0;

    if (!button->transformOverridden || !button->translate ||
        std::abs(button->translate.X()) < 0.5) {
        return;
    }

    auto xamlRoot = element.XamlRoot();
    double scale = xamlRoot ? xamlRoot.RasterizationScale() : 1.0;
    if (!(scale > 0)) {
        scale = 1.0;
    }

    // The cursor is at position within the button as drawn.
    POINT cursor;
    if (!GetCursorPos(&cursor)) {
        return;
    }
    double centerX =
        cursor.x + (element.ActualWidth() / 2 - position.X) * scale;

    g_jumpListAnchorX = static_cast<LONG>(std::lround(centerX));
    g_jumpListAnchorTime = GetTickCount64();
}

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

    auto element = button->element.get();
    if (!element) {
        return;
    }

    auto point = e.GetCurrentPoint(element);
    if (point.Properties().IsRightButtonPressed()) {
        SetJumpListAnchor(button, element, point.Position());
        return;
    }

    if (!g_launchHooked || !button->isHost) {
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

HRESULT WINAPI CTaskListWnd_ComputeJumpViewPosition_Hook(
    void* pThis,
    void* taskBtnGroup,
    int param2,
    wf::Point* point,
    wux::HorizontalAlignment* horizontalAlignment,
    wux::VerticalAlignment* verticalAlignment) {
    HRESULT result = CTaskListWnd_ComputeJumpViewPosition_Original(
        pThis, taskBtnGroup, param2, point, horizontalAlignment,
        verticalAlignment);

    ULONGLONG anchorTime = g_jumpListAnchorTime.exchange(0);
    if (SUCCEEDED(result) && point && anchorTime && !g_unloading &&
        GetTickCount64() - anchorTime < kJumpListAnchorTimeoutMs) {
        Wh_Log(L"Moving a jump list from x=%d to x=%d over the app as drawn",
               static_cast<int>(point->X),
               static_cast<int>(g_jumpListAnchorX.load()));
        point->X = static_cast<float>(g_jumpListAnchorX.load());
    }

    return result;
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

////////////////////////////////////////////////////////////////////////////////
// Taskbar row layout: open apps kept apart from pinned apps (separateRunning),
// and folders whose apps are all open.
//
// The taskbar lays out its buttons in one row: pinned apps in their pinned
// order (an open pinned app keeps its pinned spot), then other open apps. The
// mod only adds margins to that row, so that the taskbar's own layout, and
// with it dragging, keeps working:
// * A folder whose apps are all open has no closed button to sit on. Its
//   first open pinned app (the carrier) gets a left margin as wide as itself,
//   and the mod draws a folder button of its own there, in an overlay canvas.
// * With separateRunning, the last button of the pinned section (a closed
//   pinned app, a folder, or a carrier's folder space) gets a right margin
//   that puts the open apps after it in the middle of the taskbar. An open
//   pinned app before it is collapsed to no width with a negative margin and
//   drawn among the open apps with a TranslateTransform. Its space there is
//   part of that right margin.
// Everything is recomputed after each layout pass of the taskbar, except while
// a button is pressed (e.g. being dragged), when the row's geometry is in flux.

// A folder button drawn by the mod, for a folder whose apps are all open.
struct DrawnFolder {
    int folder = -1;
    winrt::weak_ref<wuxc::Grid> element;
    winrt::weak_ref<wuxc::Border> highlight;
    winrt::weak_ref<wuxc::Image> image;
    bool hovered = false;
    bool pressed = false;
    uint32_t pointerId = 0;
    bool seen = false;
};

struct SectionsTaskbar {
    void* key = nullptr;
    winrt::weak_ref<wux::FrameworkElement> repeater;
    winrt::event_token layoutUpdatedToken{};
    winrt::weak_ref<wux::FrameworkElement> trayFrame;
    bool trayLookedUp = false;
    // A canvas added on top of the repeater, holding the separator line and
    // drawn folders.
    winrt::weak_ref<wuxc::Panel> overlayParent;
    winrt::weak_ref<wuxc::Canvas> overlay;
    winrt::weak_ref<wuxc::Border> separator;
    std::vector<DrawnFolder> drawnFolders;
    bool loggedCentered = false;
};

std::vector<std::unique_ptr<SectionsTaskbar>> g_sectionsTaskbars;
bool g_arrangingSections = false;

struct SectionItem {
    ButtonState* button = nullptr;
    wux::FrameworkElement element{nullptr};
    int index = -1;
    double x = 0;
    double width = 0;
    // The button's own margin, without what the mod added.
    wux::Thickness base{};
    // The space the button takes in the row with its own margin.
    double extent = 0;
    // Where the button's space in the row starts.
    double slotStart = 0;
};

bool IsPointerHeld(ButtonState* button) {
    return button->pointerDown &&
           GetTickCount64() - button->pointerDownTime < 60 * 1000;
}

bool SameThickness(wux::Thickness const& a,
                   wux::Thickness const& b,
                   double tolerance = 0.01) {
    return std::abs(a.Left - b.Left) < tolerance &&
           std::abs(a.Top - b.Top) < tolerance &&
           std::abs(a.Right - b.Right) < tolerance &&
           std::abs(a.Bottom - b.Bottom) < tolerance;
}

// The button's margin without what the mod added. If something else set the
// margin since, that's the button's own margin now.
wux::Thickness SectionBaseMargin(ButtonState* button,
                                 wux::FrameworkElement const& element) {
    auto margin = element.Margin();
    if (button->marginOverridden &&
        SameThickness(margin, button->appliedMargin)) {
        return button->baseMargin;
    }

    button->marginOverridden = false;
    button->baseMargin = margin;
    return margin;
}

void SetSectionMargin(ButtonState* button,
                      wux::FrameworkElement const& element,
                      wux::Thickness const& base,
                      double extraLeft,
                      double extraRight) {
    if (std::abs(extraLeft) < 0.5 && std::abs(extraRight) < 0.5) {
        if (button->marginOverridden) {
            element.Margin(base);
            button->marginOverridden = false;
        }
        return;
    }

    wux::Thickness margin = base;
    margin.Left += extraLeft;
    margin.Right += extraRight;
    // Small changes are ignored, so that layout passes settle.
    if (button->marginOverridden &&
        SameThickness(margin, button->appliedMargin, 0.5)) {
        return;
    }

    button->baseMargin = base;
    button->appliedMargin = margin;
    button->marginOverridden = true;
    element.Margin(margin);
}

bool IsSectionTransform(ButtonState* button,
                        wux::Media::Transform const& transform) {
    if (!transform) {
        return false;
    }
    void* ptr = CanonicalPtr(transform);
    return (button->translate && ptr == CanonicalPtr(button->translate)) ||
           (button->transformGroup &&
            ptr == CanonicalPtr(button->transformGroup));
}

void SetSectionTranslate(ButtonState* button,
                         wux::FrameworkElement const& element,
                         double x) {
    auto current = element.RenderTransform();
    if (!button->transformOverridden || !IsSectionTransform(button, current)) {
        // Any transform the button already has is kept, ahead of the mod's.
        button->translate = wux::Media::TranslateTransform();
        button->transformGroup = nullptr;
        button->originalTransform = current;
        if (current) {
            element.RenderTransform(nullptr);
            wux::Media::TransformGroup group;
            group.Children().Append(current);
            group.Children().Append(button->translate);
            button->transformGroup = group;
            element.RenderTransform(group);
        } else {
            element.RenderTransform(button->translate);
        }
        button->transformOverridden = true;
    }

    if (std::abs(button->translate.X() - x) >= 0.25) {
        button->translate.X(x);
    }
}

void RestoreSectionTranslate(ButtonState* button,
                             wux::FrameworkElement const& element) {
    if (!button->transformOverridden) {
        return;
    }

    // If something else replaced the transform since, it's left alone.
    if (element && IsSectionTransform(button, element.RenderTransform())) {
        if (button->transformGroup) {
            button->transformGroup.Children().Clear();
        }
        element.RenderTransform(button->originalTransform);
    }

    button->transformOverridden = false;
    button->originalTransform = nullptr;
    button->translate = nullptr;
    button->transformGroup = nullptr;
}

void RaiseDraggedButton(ButtonState* button, bool raise) {
    auto element = button->element.get();
    if (!element || raise == button->zIndexRaised) {
        return;
    }

    if (raise) {
        button->originalZIndex = wuxc::Canvas::GetZIndex(element);
        wuxc::Canvas::SetZIndex(element, button->originalZIndex + 100);
    } else {
        wuxc::Canvas::SetZIndex(element, button->originalZIndex);
    }
    button->zIndexRaised = raise;
}

// Undoes the margin and transform set for keeping open apps apart.
void RestoreSectionLayout(ButtonState* button) {
    auto element = button->element.get();

    Guarded([&] {
        if (element && button->marginOverridden &&
            SameThickness(element.Margin(), button->appliedMargin)) {
            element.Margin(button->baseMargin);
        }
    });
    button->marginOverridden = false;

    Guarded([&] { RestoreSectionTranslate(button, element); });
    Guarded([&] { RaiseDraggedButton(button, false); });
}

std::optional<int> GetRepeaterElementIndex(
    wux::FrameworkElement const& repeater,
    wux::UIElement const& element) {
    winrt::com_ptr<IUnknown> itemsRepeater;
    if (FAILED(winrt::get_unknown(repeater)->QueryInterface(
            kIID_IItemsRepeater, itemsRepeater.put_void()))) {
        return std::nullopt;
    }

    using GetElementIndex_t =
        HRESULT(STDMETHODCALLTYPE*)(void* pThis, void* element, int* index);
    void** vtable = *reinterpret_cast<void***>(itemsRepeater.get());
    auto getElementIndex = reinterpret_cast<GetElementIndex_t>(vtable[19]);

    int index = -1;
    if (FAILED(getElementIndex(itemsRepeater.get(), winrt::get_abi(element),
                               &index))) {
        return -1;
    }
    return index;
}

wux::FrameworkElement FindDescendantByClassName(
    wux::DependencyObject const& root,
    std::wstring_view className) {
    std::vector<std::pair<wux::DependencyObject, int>> queue{{root, 0}};
    for (size_t i = 0; i < queue.size(); i++) {
        auto [element, depth] = queue[i];
        if (i > 0 && winrt::get_class_name(element) == className) {
            return element.try_as<wux::FrameworkElement>();
        }

        if (depth >= 12) {
            continue;
        }

        int count = wux::Media::VisualTreeHelper::GetChildrenCount(element);
        for (int j = 0; j < count; j++) {
            queue.emplace_back(
                wux::Media::VisualTreeHelper::GetChild(element, j), depth + 1);
        }
    }
    return nullptr;
}


SectionsTaskbar* FindSectionsTaskbar(void* key) {
    for (auto& taskbar : g_sectionsTaskbars) {
        if (taskbar->key == key) {
            return taskbar.get();
        }
    }
    return nullptr;
}

DrawnFolder* FindDrawnFolder(SectionsTaskbar& taskbar, int folder) {
    for (auto& drawn : taskbar.drawnFolders) {
        if (drawn.folder == folder) {
            return &drawn;
        }
    }
    return nullptr;
}

// The open app a drawn folder is in front of.
ButtonState* FindFolderCarrier(void* taskbarKey, int folder) {
    for (auto& button : g_buttons) {
        if (button->carriesFolder && button->folder == folder &&
            button->repeaterKey == taskbarKey) {
            return button.get();
        }
    }
    return nullptr;
}

wuxc::Canvas EnsureOverlay(SectionsTaskbar& taskbar,
                           wux::FrameworkElement const& repeater) {
    auto overlay = taskbar.overlay.get();
    if (overlay && wux::Media::VisualTreeHelper::GetParent(overlay)) {
        return overlay;
    }

    auto parent = wux::Media::VisualTreeHelper::GetParent(repeater)
                      .try_as<wuxc::Panel>();
    if (!parent) {
        return nullptr;
    }

    // A canvas with no size at the top left of the repeater's parent. It
    // doesn't take part in its layout, and its children are placed in it by
    // coordinates. Having no background, only its children take input.
    wuxc::Canvas canvas;
    canvas.HorizontalAlignment(wux::HorizontalAlignment::Left);
    canvas.VerticalAlignment(wux::VerticalAlignment::Top);
    parent.Children().Append(canvas);

    taskbar.overlayParent = winrt::make_weak(parent);
    taskbar.overlay = winrt::make_weak(canvas);
    taskbar.separator = nullptr;
    taskbar.drawnFolders.clear();
    return canvas;
}

void RemoveOverlay(SectionsTaskbar& taskbar) {
    auto parent = taskbar.overlayParent.get();
    auto overlay = taskbar.overlay.get();
    if (parent && overlay) {
        uint32_t index;
        if (parent.Children().IndexOf(overlay, index)) {
            parent.Children().RemoveAt(index);
        }
    }

    taskbar.overlayParent = nullptr;
    taskbar.overlay = nullptr;
    taskbar.separator = nullptr;
    taskbar.drawnFolders.clear();
}

void HideSeparator(SectionsTaskbar& taskbar) {
    if (auto separator = taskbar.separator.get()) {
        if (separator.Visibility() != wux::Visibility::Collapsed) {
            separator.Visibility(wux::Visibility::Collapsed);
        }
    }
}

// Draws the separator line at x, in the repeater's coordinates, centered
// vertically on a button of the row.
void PlaceSeparator(SectionsTaskbar& taskbar,
                    wux::FrameworkElement const& repeater,
                    SectionItem const& reference,
                    double x) {
    auto overlay = EnsureOverlay(taskbar, repeater);
    if (!overlay) {
        return;
    }

    auto separator = taskbar.separator.get();
    if (!separator) {
        separator = wuxc::Border();
        separator.IsHitTestVisible(false);
        separator.Width(kSeparatorWidth);
        separator.CornerRadius(wux::CornerRadius{
            kSeparatorWidth / 2, kSeparatorWidth / 2, kSeparatorWidth / 2,
            kSeparatorWidth / 2});
        separator.Opacity(kSeparatorOpacity);
        overlay.Children().Append(separator);
        taskbar.separator = winrt::make_weak(separator);
    }

    double buttonHeight = reference.element.ActualHeight();
    double height =
        std::max(12.0, std::round(buttonHeight * kSeparatorHeightRatio));
    auto origin = repeater.TransformToVisual(overlay).TransformPoint(
        {static_cast<float>(x),
         static_cast<float>(reference.element.ActualOffset().y)});
    double left = std::round(origin.X);
    double top = std::round(origin.Y + (buttonHeight - height) / 2);

    if (wuxc::Canvas::GetLeft(separator) != left) {
        wuxc::Canvas::SetLeft(separator, left);
    }
    if (wuxc::Canvas::GetTop(separator) != top) {
        wuxc::Canvas::SetTop(separator, top);
    }
    if (separator.Height() != height) {
        separator.Height(height);
    }

    // The color of the taskbar's text, which follows the light or dark theme.
    if (auto control = reference.element.try_as<wuxc::Control>()) {
        auto brush = control.Foreground();
        if (brush && separator.Background() != brush) {
            separator.Background(brush);
        }
    }

    if (separator.Visibility() != wux::Visibility::Visible) {
        separator.Visibility(wux::Visibility::Visible);
    }
}

// The hover or press background of a drawn folder: the taskbar's text color,
// faint, like the taskbar's own buttons.
wux::Media::Brush DrawnFolderHighlight(ButtonState* carrier, bool pressed) {
    winrt::Windows::UI::Color color{0xFF, 0xFF, 0xFF, 0xFF};
    if (auto element = carrier ? carrier->element.get() : nullptr) {
        if (auto control = element.try_as<wuxc::Control>()) {
            if (auto brush = control.Foreground()
                                 .try_as<wux::Media::SolidColorBrush>()) {
                color = brush.Color();
            }
        }
    }
    color.A = pressed ? 0x10 : 0x18;
    return wux::Media::SolidColorBrush(color);
}

void UpdateDrawnFolderHighlight(void* taskbarKey, int folder) {
    SectionsTaskbar* taskbar = FindSectionsTaskbar(taskbarKey);
    DrawnFolder* drawn = taskbar ? FindDrawnFolder(*taskbar, folder) : nullptr;
    auto highlight = drawn ? drawn->highlight.get() : nullptr;
    if (!highlight) {
        return;
    }

    if (!drawn->hovered && !drawn->pressed) {
        highlight.Background(nullptr);
        return;
    }
    highlight.Background(DrawnFolderHighlight(
        FindFolderCarrier(taskbarKey, folder), drawn->pressed));
}

wuxc::Grid CreateDrawnFolder(void* taskbarKey, int folder) {
    wuxc::Grid button;
    // A background, even a transparent one, makes the whole area take input.
    button.Background(TransparentBrush());

    wuxc::Border highlight;
    highlight.CornerRadius(wux::CornerRadius{4, 4, 4, 4});
    highlight.Margin(wux::Thickness{2, 4, 2, 4});
    highlight.IsHitTestVisible(false);
    button.Children().Append(highlight);

    wuxc::Image image;
    image.HorizontalAlignment(wux::HorizontalAlignment::Center);
    image.VerticalAlignment(wux::VerticalAlignment::Center);
    image.IsHitTestVisible(false);
    button.Children().Append(image);

    std::wstring name = FolderDisplayName(folder);
    wux::Automation::AutomationProperties::SetName(button,
                                                   winrt::hstring(name));

    // Lists the folder's apps, like the tooltip of a folder on a closed app.
    wuxc::ToolTip toolTip;
    toolTip.Content(winrt::box_value(winrt::hstring(name)));
    toolTip.Opened([taskbarKey, folder](wf::IInspectable const& sender,
                                        auto const&) {
        Guarded([&] {
            if (ButtonState* carrier = FindFolderCarrier(taskbarKey, folder)) {
                sender.as<wuxc::ToolTip>().Content(
                    BuildFolderToolTipContent(carrier));
            }
        });
    });
    wuxc::ToolTipService::SetToolTip(button, toolTip);

    auto drawnFolder = [taskbarKey, folder]() -> DrawnFolder* {
        SectionsTaskbar* taskbar = FindSectionsTaskbar(taskbarKey);
        return taskbar ? FindDrawnFolder(*taskbar, folder) : nullptr;
    };

    button.PointerEntered([=](auto const&, auto const&) {
        Guarded([&] {
            if (DrawnFolder* drawn = drawnFolder()) {
                drawn->hovered = true;
                UpdateDrawnFolderHighlight(taskbarKey, folder);
            }
        });
    });
    button.PointerExited([=](auto const&, auto const&) {
        Guarded([&] {
            if (DrawnFolder* drawn = drawnFolder()) {
                drawn->hovered = false;
                UpdateDrawnFolderHighlight(taskbarKey, folder);
            }
        });
    });
    button.PointerPressed(
        [=](wf::IInspectable const& sender,
            wux::Input::PointerRoutedEventArgs const& e) {
            Guarded([&] {
                e.Handled(true);
                DrawnFolder* drawn = drawnFolder();
                if (!drawn) {
                    return;
                }
                drawn->pressed = true;
                drawn->pointerId = e.Pointer().PointerId();
                sender.as<wux::UIElement>().CapturePointer(e.Pointer());
                UpdateDrawnFolderHighlight(taskbarKey, folder);
            });
        });
    button.PointerReleased(
        [=](wf::IInspectable const& sender,
            wux::Input::PointerRoutedEventArgs const& e) {
            Guarded([&] {
                e.Handled(true);
                DrawnFolder* drawn = drawnFolder();
                if (!drawn || !drawn->pressed ||
                    drawn->pointerId != e.Pointer().PointerId()) {
                    return;
                }
                drawn->pressed = false;
                auto element = sender.as<wux::FrameworkElement>();
                element.ReleasePointerCapture(e.Pointer());
                UpdateDrawnFolderHighlight(taskbarKey, folder);

                // Like a regular button, releasing outside of it cancels the
                // click.
                auto position = e.GetCurrentPoint(element).Position();
                if (position.X < 0 || position.Y < 0 ||
                    position.X > element.ActualWidth() ||
                    position.Y > element.ActualHeight()) {
                    return;
                }

                if (ButtonState* carrier =
                        FindFolderCarrier(taskbarKey, folder)) {
                    ShowFolderMenu(carrier, element);
                }
            });
        });
    button.PointerCaptureLost([=](auto const&, auto const&) {
        Guarded([&] {
            if (DrawnFolder* drawn = drawnFolder()) {
                drawn->pressed = false;
                UpdateDrawnFolderHighlight(taskbarKey, folder);
            }
        });
    });
    // Keep the taskbar's own menu away, as on a folder on a closed app.
    button.RightTapped([](auto const&, wux::Input::RightTappedRoutedEventArgs
                                           const& e) { e.Handled(true); });
    button.ContextRequested(
        [](auto const&, wux::Input::ContextRequestedEventArgs const& e) {
            e.Handled(true);
        });

    return button;
}

// Draws the folder that the item carries, in the space left in front of it:
// from left (in the repeater's coordinates), the size of the item's button.
void PlaceDrawnFolder(SectionsTaskbar& taskbar,
                      wux::FrameworkElement const& repeater,
                      SectionItem const& item,
                      double left) {
    auto overlay = EnsureOverlay(taskbar, repeater);
    if (!overlay) {
        return;
    }

    int folder = item.button->folder;
    DrawnFolder* drawn = FindDrawnFolder(taskbar, folder);
    if (drawn && !drawn->element.get()) {
        std::erase_if(taskbar.drawnFolders,
                      [folder](auto const& d) { return d.folder == folder; });
        drawn = nullptr;
    }
    if (!drawn) {
        auto element = CreateDrawnFolder(taskbar.key, folder);
        overlay.Children().Append(element);

        DrawnFolder newDrawn;
        newDrawn.folder = folder;
        newDrawn.element = winrt::make_weak(element);
        newDrawn.highlight = winrt::make_weak(
            element.Children().GetAt(0).as<wuxc::Border>());
        newDrawn.image =
            winrt::make_weak(element.Children().GetAt(1).as<wuxc::Image>());
        taskbar.drawnFolders.push_back(std::move(newDrawn));
        drawn = &taskbar.drawnFolders.back();
        Wh_Log(L"Drawing folder %s in front of %s",
               FolderDisplayName(folder).c_str(),
               item.button->appId.c_str());
    }
    drawn->seen = true;

    auto element = drawn->element.get();
    auto image = drawn->image.get();
    if (!element || !image) {
        return;
    }

    auto origin = repeater.TransformToVisual(overlay).TransformPoint(
        {static_cast<float>(left),
         static_cast<float>(item.element.ActualOffset().y)});
    double x = std::round(origin.X);
    double y = std::round(origin.Y);
    double width = item.width;
    double height = item.element.ActualHeight();

    if (wuxc::Canvas::GetLeft(element) != x) {
        wuxc::Canvas::SetLeft(element, x);
    }
    if (wuxc::Canvas::GetTop(element) != y) {
        wuxc::Canvas::SetTop(element, y);
    }
    if (element.Width() != width) {
        element.Width(width);
    }
    if (element.Height() != height) {
        element.Height(height);
    }

    // The same icon size as the app's.
    double iconWidth = 24;
    double iconHeight = 24;
    if (auto appIcon = item.button->icon.get()) {
        if (appIcon.ActualWidth() > 0 && appIcon.ActualHeight() > 0) {
            iconWidth = appIcon.ActualWidth();
            iconHeight = appIcon.ActualHeight();
        }
    }
    if (image.Width() != iconWidth) {
        image.Width(iconWidth);
    }
    if (image.Height() != iconHeight) {
        image.Height(iconHeight);
    }

    auto icon = GetFolderIcon(folder);
    if (icon && image.Source() != icon) {
        image.Source(icon);
    }
}

// Removes the drawn folders that weren't placed in this pass.
void PruneDrawnFolders(SectionsTaskbar& taskbar) {
    auto overlay = taskbar.overlay.get();
    std::erase_if(taskbar.drawnFolders, [&](DrawnFolder& drawn) {
        if (drawn.seen) {
            drawn.seen = false;
            return false;
        }
        auto element = drawn.element.get();
        if (overlay && element) {
            uint32_t index;
            if (overlay.Children().IndexOf(element, index)) {
                overlay.Children().RemoveAt(index);
            }
        }
        return true;
    });
}

void ClearRowLayout(SectionsTaskbar& taskbar,
                    std::vector<SectionItem> const& items) {
    for (auto& item : items) {
        RestoreSectionLayout(item.button);
    }
    HideSeparator(taskbar);
    PruneDrawnFolders(taskbar);
}

// The open apps (indexes into items) in the mod's order. An app gets its
// place when it's first seen open, after the open apps before it, so a newly
// opened app goes last. Places are kept per button, for as long as its app
// stays open.
std::vector<int> OrderRunningItems(std::vector<SectionItem> const& items,
                                   std::vector<int> const& running) {
    for (auto& item : items) {
        if (!item.button->running) {
            item.button->runningRank = 0;
        }
    }

    for (int i : running) {
        ButtonState* button = items[i].button;
        if (!button->runningRank ||
            button->rankAppIdLower != button->appIdLower) {
            button->runningRank = ++g_lastRunningRank;
            button->rankAppIdLower = button->appIdLower;
        }
    }

    std::vector<int> order = running;
    std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
        return items[a].button->runningRank < items[b].button->runningRank;
    });
    return order;
}

// The left edge of each open app in order, from groupStart. While one is
// dragged, it follows the pointer, and it changes places with the apps it
// passes the middle of. The new order is kept.
std::vector<double> ApplyRunningDrag(std::vector<SectionItem> const& items,
                                     std::vector<int>& order,
                                     double groupStart) {
    auto layOut = [&] {
        std::vector<double> lefts;
        double cursor = groupStart;
        for (int i : order) {
            lefts.push_back(cursor + items[i].base.Left);
            cursor += items[i].extent;
        }
        return lefts;
    };

    int dragged = -1;
    if (g_runningDrag.dragging) {
        for (size_t k = 0; k < order.size(); k++) {
            if (items[order[k]].button->key == g_runningDrag.key) {
                dragged = static_cast<int>(k);
                break;
            }
        }
    }
    if (dragged < 0) {
        return layOut();
    }

    auto& item = items[order[dragged]];
    double groupWidth = 0;
    for (int i : order) {
        groupWidth += items[i].extent;
    }
    double left = g_runningDrag.startLeft +
                  (g_runningDrag.currentX - g_runningDrag.startX);
    left = std::clamp(left, groupStart + item.base.Left,
                      groupStart + groupWidth - item.extent + item.base.Left);
    double center = left + item.width / 2;

    // The dragged app's new place: after every other app whose middle it's
    // past, with the others laid out where they are now.
    std::vector<double> lefts = layOut();
    int target = 0;
    for (size_t k = 0; k < order.size(); k++) {
        if (static_cast<int>(k) == dragged) {
            continue;
        }
        if (lefts[k] + items[order[k]].width / 2 < center) {
            target++;
        }
    }

    if (target != dragged) {
        // Keep the same set of places, handed out in the new order.
        std::vector<uint64_t> ranks;
        for (int i : order) {
            ranks.push_back(items[i].button->runningRank);
        }
        std::sort(ranks.begin(), ranks.end());

        int moved = order[dragged];
        order.erase(order.begin() + dragged);
        order.insert(order.begin() + target, moved);
        for (size_t k = 0; k < order.size(); k++) {
            items[order[k]].button->runningRank = ranks[k];
        }
        lefts = layOut();
        dragged = target;
    }

    lefts[dragged] = left;
    return lefts;
}

void ArrangeSections(SectionsTaskbar& taskbar) {
    if (g_unloading || g_arrangingSections) {
        return;
    }

    auto repeater = taskbar.repeater.get();
    if (!repeater) {
        return;
    }

    auto root = wux::Media::VisualTreeHelper::GetParent(repeater)
                    .try_as<wux::FrameworkElement>();
    if (!root) {
        return;
    }

    g_arrangingSections = true;
    struct ArrangingGuard {
        ~ArrangingGuard() { g_arrangingSections = false; }
    } arrangingGuard;

    std::vector<SectionItem> items;
    bool frozen = false;
    bool haveIndexes = true;
    for (auto& button : g_buttons) {
        if (button->repeaterKey != taskbar.key) {
            continue;
        }

        auto element = button->element.get();
        if (!element) {
            continue;
        }

        if (button->collapsedByUs || button->recycled ||
            element.Visibility() != wux::Visibility::Visible) {
            RestoreSectionLayout(button.get());
            continue;
        }

        // The taskbar's own drag reorders the row live. Open apps are dragged
        // by the mod instead while they're kept apart from pinned apps.
        if (IsPointerHeld(button.get()) &&
            !(button->running && g_settings.separateRunning &&
              g_dragHooked)) {
            frozen = true;
        }

        SectionItem item;
        item.button = button.get();
        item.element = element;
        if (haveIndexes) {
            auto index = GetRepeaterElementIndex(repeater, element);
            if (index) {
                item.index = *index;
            } else {
                haveIndexes = false;
            }
        }
        items.push_back(std::move(item));
    }

    // A pressed button may be being dragged. The row is rearranged once it's
    // released.
    if (frozen) {
        return;
    }

    double rootWidth = root.ActualWidth();
    bool horizontal = rootWidth > 0 && rootWidth >= root.ActualHeight();
    if (!horizontal ||
        repeater.FlowDirection() == wux::FlowDirection::RightToLeft) {
        ClearRowLayout(taskbar, items);
        return;
    }

    for (auto& item : items) {
        item.x = item.element.ActualOffset().x;
        item.width = item.element.ActualWidth();
        item.base = SectionBaseMargin(item.button, item.element);
        item.extent = item.base.Left + item.width + item.base.Right;
        // Where the item's space in the row starts, without the mod's margin.
        item.slotStart = item.x - item.element.Margin().Left;
    }

    if (haveIndexes) {
        // Buttons that aren't in the row (e.g. kept for reuse) have no index.
        items.erase(std::remove_if(items.begin(), items.end(),
                                   [](SectionItem const& item) {
                                       if (item.index >= 0) {
                                           return false;
                                       }
                                       RestoreSectionLayout(item.button);
                                       return true;
                                   }),
                    items.end());
        std::sort(items.begin(), items.end(), [](auto const& a, auto const& b) {
            return a.index < b.index;
        });
    } else {
        // By position. A collapsed button starts where the next one does.
        std::stable_sort(items.begin(), items.end(),
                         [](auto const& a, auto const& b) {
                             if (std::abs(a.slotStart - b.slotStart) >= 0.5) {
                                 return a.slotStart < b.slotStart;
                             }
                             return a.button->transformOverridden &&
                                    !b.button->transformOverridden;
                         });
    }

    const int count = static_cast<int>(items.size());

    // The mod's margins for each item, and the shift of open pinned apps.
    std::vector<double> extraLeft(count, 0);
    std::vector<double> extraRight(count, 0);
    std::vector<std::optional<double>> translate(count);

    // A carrier's folder space in front of it is as wide as the button.
    for (int i = 0; i < count; i++) {
        if (items[i].button->carriesFolder) {
            extraLeft[i] = items[i].extent;
        }
    }

    auto finish = [&] {
        for (int i = 0; i < count; i++) {
            auto& item = items[i];
            if (translate[i]) {
                SetSectionTranslate(item.button, item.element, *translate[i]);
            } else {
                RestoreSectionTranslate(item.button, item.element);
            }
            SetSectionMargin(item.button, item.element, item.base,
                             extraLeft[i], extraRight[i]);
            if (item.button->carriesFolder) {
                PlaceDrawnFolder(taskbar, repeater, item,
                                 item.slotStart + item.base.Left);
            }
        }
        PruneDrawnFolders(taskbar);
    };

    if (!g_settings.separateRunning) {
        HideSeparator(taskbar);
        finish();
        return;
    }

    // The pinned section: closed pinned apps, folders, and the folder space
    // of carriers. Every open app goes in the middle.
    int pinnedEnd = -1;
    double runningWidth = 0;
    std::vector<int> running;
    for (int i = 0; i < count; i++) {
        if (items[i].button->running) {
            runningWidth += items[i].extent;
            running.push_back(i);
        }
        if (!items[i].button->running || items[i].button->carriesFolder) {
            pinnedEnd = i;
        }
    }

    // In the repeater's coordinates: the middle of the taskbar, and how far
    // right the open apps can go before reaching the system tray.
    double originX =
        repeater.TransformToVisual(root).TransformPoint({0, 0}).X;
    double centerX = rootWidth / 2 - originX;
    double limitX = rootWidth - originX;

    auto trayFrame = taskbar.trayFrame.get();
    if (!trayFrame && !taskbar.trayLookedUp) {
        taskbar.trayLookedUp = true;
        trayFrame =
            FindDescendantByClassName(root, L"SystemTray.SystemTrayFrame");
        taskbar.trayFrame = trayFrame ? winrt::make_weak(trayFrame) : nullptr;
    }
    if (trayFrame && trayFrame.XamlRoot() == root.XamlRoot()) {
        double trayX =
            trayFrame.TransformToVisual(root).TransformPoint({0, 0}).X;
        if (trayX > 0 && trayX <= rootWidth) {
            limitX = trayX - originX;
        }
    }

    double runningStart =
        std::min(centerX - runningWidth / 2, limitX - runningWidth);

    // Where the open apps start (groupStart), and where the ones after the
    // pinned section are laid out by the taskbar (naturalStart).
    double groupStart = 0;
    double naturalStart = 0;
    if (pinnedEnd < 0) {
        // Only open apps: the first one moves the group to the middle.
        if (running.empty()) {
            HideSeparator(taskbar);
            finish();
            return;
        }
        extraLeft[0] =
            std::max(0.0, std::round(runningStart - items[0].slotStart));
        groupStart = items[0].slotStart + extraLeft[0];
        naturalStart = groupStart;
        HideSeparator(taskbar);
    } else {
        auto& end = items[pinnedEnd];
        // The end of its space without the mod's right margin: a closed app's
        // button, or a carrier's folder space.
        double pinnedRight = end.slotStart +
                             (end.button->carriesFolder ? extraLeft[pinnedEnd]
                                                        : 0) +
                             (end.button->running ? 0 : end.extent);

        if (!taskbar.loggedCentered && originX > rootWidth / 5) {
            taskbar.loggedCentered = true;
            Wh_Log(L"The taskbar looks center-aligned. For pinned apps to sit "
                   L"at the left edge, set Taskbar alignment to Left");
        }

        double minGap = g_settings.pinnedSeparator
                            ? kSeparatorPadding * 2 + kSeparatorWidth
                            : kMinSectionGap;
        double gap = 0;
        if (!running.empty()) {
            gap = std::max(std::round(runningStart - pinnedRight), minGap);
        } else if (g_settings.pinnedSeparator) {
            gap = minGap;
        }

        // Open apps up to the end of the pinned section (open pinned apps and
        // carriers) take no space there: a negative right margin as wide as
        // the button with its own margin. Their space is reserved by the
        // margin after the pinned section instead, in front of the open apps
        // that the taskbar lays out after it.
        double movedWidth = 0;
        for (int i : running) {
            if (i <= pinnedEnd) {
                extraRight[i] = -items[i].extent;
                movedWidth += items[i].extent;
            }
        }
        extraRight[pinnedEnd] += gap + movedWidth;

        groupStart = pinnedRight + gap;
        naturalStart = groupStart + movedWidth;

        if (g_settings.pinnedSeparator) {
            PlaceSeparator(taskbar, repeater, end,
                           pinnedRight + kSeparatorPadding);
        } else {
            HideSeparator(taskbar);
        }
    }

    // Where the taskbar lays out each open app once the margins are applied:
    // the ones up to the pinned section where their space starts, the others
    // one after another from naturalStart.
    std::vector<double> natural(count, 0);
    double laidOut = naturalStart;
    for (int i : running) {
        auto& item = items[i];
        if (i <= pinnedEnd) {
            natural[i] = item.slotStart + item.base.Left + extraLeft[i];
        } else {
            natural[i] = laidOut + item.base.Left;
            laidOut += item.extent;
        }
    }

    // The open apps are shown in the mod's own order: by when they were first
    // seen open, rearranged by dragging. Each is moved from where the taskbar
    // lays it out to its place in that order.
    std::vector<int> order = OrderRunningItems(items, running);
    std::vector<double> lefts = ApplyRunningDrag(items, order, groupStart);

    for (size_t k = 0; k < order.size(); k++) {
        int i = order[k];
        items[i].button->plannedLeft = lefts[k];
        translate[i] = lefts[k] - natural[i];
        if (std::abs(*translate[i]) < 0.25) {
            translate[i] = std::nullopt;
        }
    }

    finish();
}

void TearDownSections() {
    for (auto& taskbar : g_sectionsTaskbars) {
        Guarded([&] {
            auto repeater = taskbar->repeater.get();
            if (repeater && taskbar->layoutUpdatedToken) {
                repeater.LayoutUpdated(taskbar->layoutUpdatedToken);
            }
        });
        Guarded([&] { RemoveOverlay(*taskbar); });
    }
    g_sectionsTaskbars.clear();

    for (auto& button : g_buttons) {
        RestoreSectionLayout(button.get());
        button->repeaterKey = nullptr;
    }
}

void ArrangeAllSections() {
    bool needed = g_settings.separateRunning;
    for (auto& button : g_buttons) {
        if (button->carriesFolder) {
            needed = true;
        }
    }
    if (!needed) {
        TearDownSections();
        return;
    }

    // Taskbars whose row is gone, e.g. after a display change.
    std::erase_if(g_sectionsTaskbars, [](auto const& taskbar) {
        if (taskbar->repeater.get()) {
            // Look for the system tray again if it wasn't there yet.
            if (!taskbar->trayFrame.get()) {
                taskbar->trayLookedUp = false;
            }
            return false;
        }
        Guarded([&] { RemoveOverlay(*taskbar); });
        return true;
    });

    // Only buttons in the taskbar's main row, not e.g. in the overflow menu.
    for (auto& button : g_buttons) {
        Guarded([&] {
            void* repeaterKey = nullptr;
            if (auto element = button->element.get()) {
                auto parent = wux::Media::VisualTreeHelper::GetParent(element)
                                  .try_as<wux::FrameworkElement>();
                if (parent && parent.Name() == L"TaskbarFrameRepeater") {
                    repeaterKey = CanonicalPtr(parent);
                    if (!FindSectionsTaskbar(repeaterKey)) {
                        auto taskbar = std::make_unique<SectionsTaskbar>();
                        taskbar->key = repeaterKey;
                        taskbar->repeater = winrt::make_weak(parent);
                        // Runs after every layout pass, e.g. when a button
                        // changes size or the taskbar is resized.
                        taskbar->layoutUpdatedToken = parent.LayoutUpdated(
                            [repeaterKey](auto const&, auto const&) {
                                Guarded([&] {
                                    if (auto* taskbar =
                                            FindSectionsTaskbar(repeaterKey)) {
                                        ArrangeSections(*taskbar);
                                    }
                                });
                            });
                        g_sectionsTaskbars.push_back(std::move(taskbar));
                    }
                }
            }

            if (repeaterKey != button->repeaterKey) {
                RestoreSectionLayout(button.get());
                button->repeaterKey = repeaterKey;
            }
        });
    }

    for (auto& taskbar : g_sectionsTaskbars) {
        Guarded([&] { ArrangeSections(*taskbar); });
    }
}

////////////////////////////////////////////////////////////////////////////////
// Dragging open apps (separateRunning)
//
// The taskbar lays out an open pinned app at its pinned spot, and the mod only
// shows it among the open apps. The taskbar's own drag would reorder it among
// the pinned apps, so presses on open apps are watched in the taskbar button's
// own pointer handlers: once the pointer moves, the taskbar doesn't see the
// moves, and the mod moves the button and reorders the open apps itself
// (ApplyRunningDrag). A press that doesn't move is left to the taskbar, so
// clicks work as usual.

ButtonState* ButtonFromTaskListButton(void* pThis) {
    wux::UIElement element{nullptr};
    reinterpret_cast<IUnknown*>(pThis)->QueryInterface(
        winrt::guid_of<wux::UIElement>(), winrt::put_abi(element));
    return element ? FindButton(CanonicalPtr(element)) : nullptr;
}

wux::Input::PointerRoutedEventArgs PointerArgs(void* pArgs) {
    wux::Input::PointerRoutedEventArgs args{nullptr};
    reinterpret_cast<IUnknown*>(pArgs)->QueryInterface(
        winrt::guid_of<wux::Input::PointerRoutedEventArgs>(),
        winrt::put_abi(args));
    return args;
}

wux::FrameworkElement DragRepeater(ButtonState* button) {
    SectionsTaskbar* taskbar =
        button->repeaterKey ? FindSectionsTaskbar(button->repeaterKey)
                            : nullptr;
    return taskbar ? taskbar->repeater.get() : nullptr;
}

// Ends a drag: the app settles into its new place. Pointer capture is only
// released when asked, since the end can come from losing it.
void EndRunningDrag(bool releaseCapture) {
    RunningDrag drag = g_runningDrag;
    g_runningDrag = RunningDrag{};

    ButtonState* button = FindButton(drag.key);
    if (!button || !drag.dragging) {
        return;
    }

    Wh_Log(L"Dropped %s", button->appId.c_str());
    RaiseDraggedButton(button, false);
    if (releaseCapture) {
        if (auto element = button->element.get()) {
            // Also ends the taskbar's own press, without a click.
            element.ReleasePointerCaptures();
        }
    }

    if (SectionsTaskbar* taskbar =
            button->repeaterKey ? FindSectionsTaskbar(button->repeaterKey)
                                : nullptr) {
        ArrangeSections(*taskbar);
    }
    ScheduleUpdate();
}

void OnTaskListButtonPressed(void* pThis, void* pArgs) {
    if (g_runningDrag.dragging) {
        return;
    }
    g_runningDrag = RunningDrag{};

    if (g_unloading || !g_settings.separateRunning) {
        return;
    }

    ButtonState* button = ButtonFromTaskListButton(pThis);
    if (!button || !button->running) {
        return;
    }

    auto element = button->element.get();
    auto repeater = DragRepeater(button);
    auto args = PointerArgs(pArgs);
    if (!element || !repeater || !args) {
        return;
    }

    auto point = args.GetCurrentPoint(repeater);
    auto properties = point.Properties();
    if (!properties.IsLeftButtonPressed() || properties.IsRightButtonPressed() ||
        properties.IsMiddleButtonPressed()) {
        return;
    }

    g_runningDrag.armed = true;
    g_runningDrag.key = button->key;
    g_runningDrag.pointerId = args.Pointer().PointerId();
    g_runningDrag.startX = point.Position().X;
    g_runningDrag.currentX = g_runningDrag.startX;
}

// Returns whether the move was taken from the taskbar.
bool OnTaskListButtonMoved(void* pArgs) {
    auto args = PointerArgs(pArgs);
    if (!args || args.Pointer().PointerId() != g_runningDrag.pointerId) {
        return false;
    }

    ButtonState* button = FindButton(g_runningDrag.key);
    auto element = button ? button->element.get() : nullptr;
    auto repeater = button ? DragRepeater(button) : nullptr;
    if (!element || !repeater) {
        g_runningDrag = RunningDrag{};
        return false;
    }

    auto point = args.GetCurrentPoint(repeater);
    if (!point.Properties().IsLeftButtonPressed()) {
        EndRunningDrag(true);
        return false;
    }

    g_runningDrag.currentX = point.Position().X;
    if (!g_runningDrag.dragging) {
        // Small moves are kept from the taskbar too, so that its own drag
        // doesn't start.
        if (std::abs(g_runningDrag.currentX - g_runningDrag.startX) <
            kDragThreshold) {
            return true;
        }

        Wh_Log(L"Dragging %s", button->appId.c_str());
        g_runningDrag.dragging = true;
        g_runningDrag.startLeft = button->plannedLeft;
        element.CapturePointer(args.Pointer());
        RaiseDraggedButton(button, true);
    }

    args.Handled(true);
    if (SectionsTaskbar* taskbar = FindSectionsTaskbar(button->repeaterKey)) {
        ArrangeSections(*taskbar);
    }
    return true;
}

int WINAPI TaskListButton_OnPointerPressed_Hook(void* pThis, void* pArgs) {
    int result = TaskListButton_OnPointerPressed_Original(pThis, pArgs);
    Guarded([&] { OnTaskListButtonPressed(pThis, pArgs); });
    return result;
}

int WINAPI TaskListButton_OnPointerMoved_Hook(void* pThis, void* pArgs) {
    if (g_runningDrag.armed) {
        bool taken = false;
        Guarded([&] { taken = OnTaskListButtonMoved(pArgs); });
        if (taken) {
            return S_OK;
        }
    }
    return TaskListButton_OnPointerMoved_Original(pThis, pArgs);
}

int WINAPI TaskListButton_OnPointerReleased_Hook(void* pThis, void* pArgs) {
    if (g_runningDrag.armed) {
        bool dropped = false;
        Guarded([&] {
            auto args = PointerArgs(pArgs);
            if (!args ||
                args.Pointer().PointerId() != g_runningDrag.pointerId) {
                return;
            }
            if (!g_runningDrag.dragging) {
                // A click.
                g_runningDrag = RunningDrag{};
                return;
            }
            // A drop: the taskbar doesn't get the release, so it isn't
            // taken as a click.
            args.Handled(true);
            dropped = true;
            EndRunningDrag(true);
        });
        if (dropped) {
            return S_OK;
        }
    }
    return TaskListButton_OnPointerReleased_Original(pThis, pArgs);
}

int WINAPI TaskListButton_OnPointerCaptureLost_Hook(void* pThis, void* pArgs) {
    if (g_runningDrag.armed) {
        Guarded([&] {
            ButtonState* button = ButtonFromTaskListButton(pThis);
            if (button && button->key == g_runningDrag.key) {
                EndRunningDrag(false);
            }
        });
    }
    return TaskListButton_OnPointerCaptureLost_Original(pThis, pArgs);
}

// Restores everything the mod changed on the button and removes its handlers.
void DetachButton(ButtonState* button) {
    auto element = button->element.get();

    Guarded([&] { RestoreIcon(button); });
    RestoreHostToolTip(button);
    RestoreSectionLayout(button);

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
    button->carriesFolder = false;

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

    AppNames names(button);

    for (size_t i = 0; i < g_settings.folders.size(); i++) {
        const auto& folder = g_settings.folders[i];
        for (size_t k = 0; k < folder.matchKeys.size(); k++) {
            if (!names.Matches(folder.matchKeys[k])) {
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
    button->recycled = IsRecycled(button, element);
    if (!button->running && !button->recycled && !button->appId.empty() &&
        std::find(g_closedPinnedAppIds.begin(), g_closedPinnedAppIds.end(),
                  button->appIdLower) == g_closedPinnedAppIds.end()) {
        g_closedPinnedAppIds.push_back(button->appIdLower);
    }
    if (button->recycled) {
        button->folder = -1;
        button->order = 0;
    } else {
        MatchFolder(button);
    }
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
            RestoreHostToolTip(button);
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
        ApplyHostToolTip(button, element);
    } else if (button->isHost) {
        RestoreIcon(button);
        RestoreHostToolTip(button);
        button->isHost = false;
    }
}

// Whether the button is a pinned app's: it was seen closed, or the app has a
// pinned shortcut.
bool IsPinnedButton(ButtonState* button) {
    if (std::find(g_closedPinnedAppIds.begin(), g_closedPinnedAppIds.end(),
                  button->appIdLower) != g_closedPinnedAppIds.end()) {
        return true;
    }
    return !button->appId.empty() &&
           !GetAppInfo(button->appId).shortcutPath.empty();
}

void UpdateAll() {
    if (g_unloading) {
        return;
    }

    for (auto& button : g_buttons) {
        Guarded([&] { RefreshButton(button.get()); });
    }

    // In each taskbar, a folder sits on its first member that isn't running.
    // If they're all running, the mod draws it in front of the first running
    // one that's pinned, which keeps its own button.
    std::map<std::pair<int, void*>, ButtonState*> hosts;
    std::map<std::pair<int, void*>, ButtonState*> carriers;
    for (auto& button : g_buttons) {
        if (button->folder < 0 || !button->element.get()) {
            continue;
        }

        auto key = std::make_pair(button->folder, button->rootKey);
        if (!button->running) {
            auto it = hosts.find(key);
            if (it == hosts.end() || button->order < it->second->order) {
                hosts[key] = button.get();
            }
            continue;
        }

        if (!IsPinnedButton(button.get())) {
            continue;
        }

        // The current carrier wins a tie (e.g. an app with a button per
        // window), so that the folder doesn't jump between them.
        auto it = carriers.find(key);
        if (it == carriers.end() || button->order < it->second->order ||
            (button->order == it->second->order && button->carriesFolder &&
             !it->second->carriesFolder)) {
            carriers[key] = button.get();
        }
    }

    for (auto& button : g_buttons) {
        bool carries = false;
        if (button->folder >= 0 && button->running) {
            auto key = std::make_pair(button->folder, button->rootKey);
            auto it = carriers.find(key);
            carries = !hosts.contains(key) && it != carriers.end() &&
                      it->second == button.get();
        }
        if (carries != button->carriesFolder) {
            Wh_Log(L"%s: %s", button->appId.c_str(),
                   carries ? (L"folder " + FolderDisplayName(button->folder) +
                              L" drawn in front of it")
                                 .c_str()
                           : L"no folder drawn in front of it");
            button->carriesFolder = carries;
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

    ArrangeAllSections();
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

        case WM_TIMER:
            if (wParam == kTimerEndBackgroundApps) {
                Guarded([] { EndRemainingBackgroundApps(); });
                return 0;
            }
            break;
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
    g_runningDrag = RunningDrag{};
    Guarded([] { TearDownSections(); });

    for (auto& button : g_buttons) {
        DetachButton(button.get());
    }
    g_buttons.clear();

    g_folderIcons.clear();
    g_appIcons.clear();
    g_menuFlyout = nullptr;
    g_pendingClassicMenu = PendingClassicMenu{};
    g_press = PressState{};

    // Apps already asked to quit aren't ended once the mod is gone.
    if (g_hwnd) {
        KillTimer(g_hwnd, kTimerEndBackgroundApps);
    }
    ReleaseEndingProcesses();

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

    // Hooked separately, so that each works without the other.
    WindhawkUtils::SYMBOL_HOOK jumpListHooks[] = {
        {
            {LR"(protected: long __cdecl CTaskListWnd::_ComputeJumpViewPosition(struct ITaskBtnGroup *,int,struct Windows::Foundation::Point &,enum Windows::UI::Xaml::HorizontalAlignment &,enum Windows::UI::Xaml::VerticalAlignment &)const )"},
            &CTaskListWnd_ComputeJumpViewPosition_Original,
            CTaskListWnd_ComputeJumpViewPosition_Hook,
        },
    };

    if (!WindhawkUtils::HookSymbols(taskbarModule, jumpListHooks,
                                    ARRAYSIZE(jumpListHooks))) {
        Wh_Log(L"Couldn't hook CTaskListWnd::_ComputeJumpViewPosition, jump "
               L"lists of moved open apps open where they're laid out");
    }
}

// The taskbar button's own pointer handlers, for dragging open apps. Optional:
// without them, open apps are dragged by the taskbar.
bool HookTaskbarViewSymbols(HMODULE module) {
    WindhawkUtils::SYMBOL_HOOK hooks[] = {
        {
            {LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::TaskListButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerPressed(void *))"},
            &TaskListButton_OnPointerPressed_Original,
            TaskListButton_OnPointerPressed_Hook,
        },
        {
            {LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::TaskListButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerMoved(void *))"},
            &TaskListButton_OnPointerMoved_Original,
            TaskListButton_OnPointerMoved_Hook,
        },
        {
            {LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::TaskListButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerReleased(void *))"},
            &TaskListButton_OnPointerReleased_Original,
            TaskListButton_OnPointerReleased_Hook,
        },
        {
            {LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::TaskListButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerCaptureLost(void *))"},
            &TaskListButton_OnPointerCaptureLost_Original,
            TaskListButton_OnPointerCaptureLost_Hook,
        },
    };

    return WindhawkUtils::HookSymbols(module, hooks, ARRAYSIZE(hooks));
}

HMODULE GetTaskbarViewModuleHandle() {
    HMODULE module = GetModuleHandleW(L"Taskbar.View.dll");
    if (!module) {
        module = GetModuleHandleW(L"ExplorerExtensions.dll");
    }
    return module;
}

void HandleTaskbarViewModule(HMODULE module, bool applyNow) {
    if (g_taskbarViewHooksDone.exchange(true)) {
        return;
    }

    g_dragHooked = HookTaskbarViewSymbols(module);
    if (!g_dragHooked) {
        Wh_Log(L"Couldn't hook the taskbar button's pointer handlers, open "
               L"apps are dragged by the taskbar");
    } else if (applyNow) {
        Wh_ApplyHookOperations();
    }
}

using LoadLibraryExW_t = decltype(&LoadLibraryExW);
LoadLibraryExW_t LoadLibraryExW_Original;
HMODULE WINAPI LoadLibraryExW_Hook(LPCWSTR lpLibFileName,
                                   HANDLE hFile,
                                   DWORD dwFlags) {
    HMODULE module = LoadLibraryExW_Original(lpLibFileName, hFile, dwFlags);
    if (module && !g_taskbarViewHooksDone &&
        module == GetTaskbarViewModuleHandle()) {
        HandleTaskbarViewModule(module, true);
    }
    return module;
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

    if (HMODULE taskbarViewModule = GetTaskbarViewModuleHandle()) {
        HandleTaskbarViewModule(taskbarViewModule, false);
    } else {
        HMODULE kernelBase = GetModuleHandleW(L"kernelbase.dll");
        auto loadLibraryExW = reinterpret_cast<LoadLibraryExW_t>(
            GetProcAddress(kernelBase, "LoadLibraryExW"));
        if (loadLibraryExW) {
            WindhawkUtils::SetFunctionHook(loadLibraryExW, LoadLibraryExW_Hook,
                                           &LoadLibraryExW_Original);
        }
    }

    return TRUE;
}

void Wh_ModAfterInit() {
    Wh_Log(L">");

    if (!g_taskbarViewHooksDone) {
        if (HMODULE taskbarViewModule = GetTaskbarViewModuleHandle()) {
            HandleTaskbarViewModule(taskbarViewModule, true);
        }
    }

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
