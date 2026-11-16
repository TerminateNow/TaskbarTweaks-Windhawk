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

The mod uses no internal symbols. It reaches the taskbar's XAML tree through XAML diagnostics, the same approach Windows 11 Taskbar Styler uses:

- **Injection**: `CreateWindowExW` is hooked to catch the taskbar's `Windows.UI.Composition.DesktopWindowContentBridge` window. If the taskbar already exists, `Wh_ModAfterInit` handles it instead. The mod then calls `InitializeXamlDiagnosticsEx` with its own DLL as the TAP. That requires the exported `DllGetClassObject`, and it tries `VisualDiagConnectionN` names until one is free. `VisualTreeWatcher::OnVisualTreeChange` reports every `Taskbar.TaskListButton` added to or removed from the tree.
- **Threading**: all XAML work happens on the taskbar UI thread. A hidden window (`g_hwnd`) created on that thread is used to defer work (`ScheduleUpdate` posts `kMsgUpdate`, so `UpdateAll` runs once per burst of changes) and owns the classic menu. Settings changes and uninit get onto that thread with `RunFromWindowThread`.
- **Button state**: a button's app comes from its `AutomationId` (`"Appid: <AUMID>"`). Its running state comes from the template root's `RunningIndicatorStates` visual state group. Its icon is the `Image` named `Icon` in its template. Buttons are recycled, so `UpdateAll` re-reads all of this on every pass and never caches it per element.
- **Folders**: folders come only from settings. In each taskbar (`XamlRoot`), a folder's host is its first member that isn't running. The host's icon is swapped for the folder icon and the other non-running members are collapsed. Pointer, tap and context events are handled on the host's template root, so they're marked handled before the button's own logic runs. That's how a click opens the folder menu instead of launching the app.
- **Removed apps**: the ❌ button can't edit settings, so removals are stored with `Wh_SetStringValue(L"removedApps")`, keyed by folder name plus a signature of its app list. `PruneRemovedApps` drops them when that folder's list changes.
- **Teardown**: `DetachButton` must undo everything (visibility, icon, every event token). Uninit unadvises the watcher, runs cleanup on the UI thread, and only then releases the watcher.
