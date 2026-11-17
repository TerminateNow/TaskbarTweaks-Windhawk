# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

[Windhawk](https://windhawk.net) mods for the Windows 11 taskbar. Each mod is a single self-contained C++ file, `mods/<mod-id>.wh.cpp`, that Windhawk compiles (clang targeting x86-64 MinGW, with C++/WinRT) and injects into `explorer.exe`. There's no build system: Windhawk reads the file's header comment blocks:

- `==WindhawkMod==`: id, name, version, `@include` process, `@compilerOptions` (the `-l` libraries to link).
- `==WindhawkModReadme==`: Markdown the user sees in Windhawk. Keep it in sync with behavior.
- `==WindhawkModSettings==`: YAML-like settings schema. Arrays of objects use `- - key:` and are read with `Wh_GetStringSetting(L"folders[%d].apps[%d]", i, j)`. Reading past the end returns an empty string.

Everything is GPL-3.0.

## Checking a mod

Mods can't be run here (they need Windows and Windhawk). The best available check is compiling and linking:

```bash
tools/check-mod.sh mods/taskbar-app-folders.wh.cpp
```

It needs `clang`, `zstd` and `g++-mingw-w64-x86-64-posix` from apt, and downloads MSYS2's MinGW headers (which include `xamlom.h` and MinGW-compatible C++/WinRT) on first use. The Windows SDK's own C++/WinRT headers don't work with MinGW. The script stubs the Windhawk API (`Wh_*`, `WindhawkUtils::SetFunctionHook`). If a mod uses other Windhawk APIs, add them to the stubs in the script. Linking uses the mod's `@compilerOptions`, so a missing `-l` shows up as an undefined symbol.

For Windhawk API usage and proven patterns, the upstream mods are the reference: https://github.com/ramensoftware/windhawk-mods (especially `mods/windows-11-taskbar-styler.wh.cpp`).

## taskbar-app-folders architecture

The mod reaches the taskbar's XAML tree through XAML diagnostics, the same approach Windows 11 Taskbar Styler uses. Its one internal symbol is optional: `CTaskBand::Launch` in `taskbar.dll`, hooked through `WindhawkUtils::HookSymbols`. Without it, folders still work but can't be dragged.

- **Injection**: `CreateWindowExW` is hooked to catch the taskbar's `Windows.UI.Composition.DesktopWindowContentBridge` window. If the taskbar already exists, `Wh_ModAfterInit` handles it instead. The mod then calls `InitializeXamlDiagnosticsEx` with its own DLL as the TAP. That requires the exported `DllGetClassObject`, and it tries `VisualDiagConnectionN` names until one is free. `VisualTreeWatcher::OnVisualTreeChange` reports every `Taskbar.TaskListButton` added to or removed from the tree.
- **Threading**: all XAML work happens on the taskbar UI thread. A hidden window (`g_hwnd`) created on that thread is used to defer work (`ScheduleUpdate` posts `kMsgUpdate`, so `UpdateAll` runs once per burst of changes) and owns the classic menu. Settings changes and uninit get onto that thread with `RunFromWindowThread`.
- **Button state**: a button's app comes from its `AutomationId` (`"Appid: <AUMID>"`). Its running state comes from the template root's `RunningIndicatorStates` visual state group. Its icon is the `Image` named `Icon` in its template. Buttons are recycled, so `UpdateAll` re-reads all of this on every pass and never caches it per element. A button whose window closed can stay in the tree with its old AUMID, laid out left of or above the taskbar. `IsRecycled` skips those, using the same negative-offset check as upstream `taskbar-labels`. `CollectMenuEntries` re-checks it when the menu opens.
- **App identity**: `GetAppInfo` gathers names to match folder entries against. They come from the Start menu's app list (`shell:AppsFolder\<AUMID>`) and from the taskbar's pinned shortcuts (`%APPDATA%\Microsoft\Internet Explorer\Quick Launch\User Pinned\TaskBar\*.lnk`, matched by the shortcut's AUMID or target path). Some apps, such as Steam, only resolve through the shortcuts. `LaunchApp` launches through the pinned shortcut first.
- **Folders**: folders come only from settings. In each taskbar (`XamlRoot`), a folder's host is its first member that isn't running. The host's icon is swapped for the folder icon and the other non-running members are collapsed. Running members keep their own buttons and are still listed in the menu. Choosing one invokes its button's UI Automation Invoke pattern, which switches to the app.
- **Folder menu contents**: `CollectMenuEntries` takes an `OpenAppsSnapshot` (taskbar-style top-level windows with their AUMID and process path, plus all process paths) each time a menu opens. An entry's window count covers windows with its AUMID, and windows of the same program unless they carry another pinned app's AUMID. A matching process with no windows (e.g. a tray app) still marks it open. Running buttons without a pinned shortcut are merged into a pinned entry as `aliasAppIds`. The match is first by folder app-list position (`order`), then by program. This covers Steam and Chrome, whose windows can get a separate button from their pin. Matching by position comes first because Chrome sets its AUMID for the whole process, not on its windows, so its program can't be found from window properties. A last pass merges any entries left with the same `order` (e.g. Chrome's profile picker). Each menu entry and its aliases are logged when the menu opens. ❌ removes the aliases too.
- **Settings are read-only to the mod**: Windhawk's mod API has no call for writing settings, and its settings storage is deliberately read-only from target processes (only `ModsWritable`, used by `Wh_Set*Value`, is writable). Don't try to write settings from the mod.
- **Folder clicks**: with the `CTaskBand::Launch` hook, a left or middle press on the host passes through to the taskbar, so native drag-to-reorder works. A press arms `g_hostClick`, and when the taskbar then launches the host app, `TakeHostClick` cancels the launch and posts `kMsgShowFolderMenu`. A drag never launches, and the pointer-movement check guards the rest. Right clicks, and every click when the hook is missing, are marked handled on the host's template root so they run before the button's own logic.
- **Drag glitch guard**: the taskbar hides a button's running indicator while it's being dragged. `pointerDown`/`frozenRunning` keep a button's running state fixed between press and release, so a dragged open app doesn't briefly become the folder host.
- **Removed apps**: the ❌ button can't edit settings, so removals are stored with `Wh_SetStringValue(L"removedApps")`, keyed by folder name plus a signature of its app list. `PruneRemovedApps` drops them when that folder's list changes. Each removal is a group of app IDs (the entry's own, then its `aliasAppIds`), so a merged Steam entry is removed and restored as one. The folder menu lists removals from `GetRemovedEntries` under "Removed from this folder", ↩ Restore calls `RestoreToFolder`, and Clear calls `ClearRemovedList`. That marks groups `cleared`: they stay removed but aren't listed. Cleared groups are stored with a `~` prefix.
- **Icons**: icon specs (`folders[].icon`, `defaultIcon`) are paths with an optional `,index`. `ResolveIconPath` looks up a relative path in the `iconFolder` setting, and tries image extensions for a bare name.
- **Teardown**: `DetachButton` must undo everything (visibility, icon, every event token). Uninit unadvises the watcher, runs cleanup on the UI thread, and only then releases the watcher.
