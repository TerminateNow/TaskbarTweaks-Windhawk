# TaskbarTweaks-Windhawk

[Windhawk](https://windhawk.net) mods for the Windows 11 taskbar.

| Mod | What it does |
| --- | --- |
| [Taskbar App Folders](mods/taskbar-app-folders.wh.cpp) | Groups pinned apps into folders. Clicking a folder opens a menu of its apps. Each app has a ❌ button that puts it back on the taskbar. |

## Installing a mod

1. In Windhawk, click **Create a new mod**.
2. Replace the editor's contents with the mod's `.wh.cpp` file.
3. Click **Compile mod**, then **Exit editing mode**.
4. Open the mod's **Settings** tab to configure it.

Each mod's own documentation is in the `==WindhawkModReadme==` block at the
top of its file. Windhawk shows it on the mod's **Details** tab.

## Checking a mod on Linux

`tools/check-mod.sh` compiles a mod with clang against MinGW headers and
links it into a DLL. It catches compile errors and missing Windows imports
without a Windows machine, but it doesn't run the mod.

```bash
sudo apt-get install clang zstd g++-mingw-w64-x86-64-posix
tools/check-mod.sh mods/taskbar-app-folders.wh.cpp
```

## License

GPL-3.0. See [LICENSE](LICENSE).
