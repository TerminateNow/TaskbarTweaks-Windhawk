#!/usr/bin/env bash
# Compile-checks a Windhawk mod on Linux with clang targeting x86-64 MinGW, the
# same kind of toolchain Windhawk uses, and links it into a DLL so missing
# Windows imports show up too. Nothing is run: the mod still has to be tested
# in Windhawk on Windows.
#
# Usage: tools/check-mod.sh mods/<mod>.wh.cpp
#
# Needs: clang, zstd, curl and the MinGW cross compiler
#   apt-get install clang zstd g++-mingw-w64-x86-64-posix
# MinGW headers with C++/WinRT and xamlom.h are downloaded from MSYS2 into
# $CHECK_MOD_CACHE (default ~/.cache/windhawk-check) on first use.

set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 mods/<mod>.wh.cpp" >&2
    exit 2
fi

src=$(realpath "$1")
cache=${CHECK_MOD_CACHE:-$HOME/.cache/windhawk-check}
repo=https://repo.msys2.org/mingw/clang64
packages=(
    mingw-w64-clang-x86_64-headers-14.0.0.r598.ge3d18511f-1-any.pkg.tar.zst
    mingw-w64-clang-x86_64-cppwinrt-3.0.260818.1-3-any.pkg.tar.zst
)

mkdir -p "$cache"
for package in "${packages[@]}"; do
    if [ ! -f "$cache/$package.done" ]; then
        echo "Downloading $package" >&2
        curl -fsSL -o "$cache/$package" "$repo/$package"
        tar --zstd -xf "$cache/$package" -C "$cache"
        rm "$cache/$package"
        touch "$cache/$package.done"
    fi
done

gcc_dir=$(ls -d /usr/lib/gcc/x86_64-w64-mingw32/*-posix | sort -V | tail -1)
stubs="$cache/stubs"
mkdir -p "$stubs"

# Minimal stand-ins for the headers and API Windhawk provides.
cat > "$stubs/windhawk_api.h" <<'EOF'
#pragma once
#include <windows.h>
#define WH_MOD_ID L"check-mod"
#ifdef __cplusplus
extern "C" {
#endif
void Wh_Log(const wchar_t* format, ...);
int Wh_GetIntSetting(const wchar_t* name, ...);
const wchar_t* Wh_GetStringSetting(const wchar_t* name, ...);
void Wh_FreeStringSetting(const wchar_t* value);
size_t Wh_GetStringValue(const wchar_t* name, wchar_t* buffer, size_t chars);
BOOL Wh_SetStringValue(const wchar_t* name, const wchar_t* value);
BOOL Wh_SetFunctionHook(void* target, void* hook, void** original);
#ifdef __cplusplus
}
#endif
EOF

cat > "$stubs/windhawk_utils.h" <<'EOF'
#pragma once
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
namespace WindhawkUtils {
template <typename P>
inline bool SetFunctionHook(P target, P hook, P* original) {
    return Wh_SetFunctionHook((void*)target, (void*)hook, (void**)original);
}

struct SYMBOL_HOOK {
    std::vector<std::wstring_view> symbols;
    void** pOriginalFunction;
    void* hookFunction = nullptr;
    bool optional = false;

    template <typename Prototype>
    SYMBOL_HOOK(std::vector<std::wstring_view> symbols,
                Prototype** originalFunction,
                std::type_identity_t<Prototype*> hookFunction = nullptr,
                bool optional = false)
        : symbols(std::move(symbols)),
          pOriginalFunction(reinterpret_cast<void**>(originalFunction)),
          hookFunction(reinterpret_cast<void*>(hookFunction)),
          optional(optional) {}
};

inline bool HookSymbols(HMODULE module,
                        const SYMBOL_HOOK* symbolHooks,
                        size_t symbolHooksCount,
                        const void* options = nullptr) {
    return false;
}
}  // namespace WindhawkUtils
EOF

cat > "$stubs/windhawk_api.c" <<'EOF'
#include <windows.h>
void Wh_Log(const wchar_t* format, ...) {}
int Wh_GetIntSetting(const wchar_t* name, ...) { return 0; }
const wchar_t* Wh_GetStringSetting(const wchar_t* name, ...) { return L""; }
void Wh_FreeStringSetting(const wchar_t* value) {}
size_t Wh_GetStringValue(const wchar_t* name, wchar_t* buffer, size_t chars) {
    if (chars) buffer[0] = 0;
    return 0;
}
BOOL Wh_SetStringValue(const wchar_t* name, const wchar_t* value) { return TRUE; }
BOOL Wh_SetFunctionHook(void* target, void* hook, void** original) { return TRUE; }
EOF

# Link with the libraries the mod asks for in @compilerOptions.
libs=$(sed -n 's|^// @compilerOptions||p' "$src")

out=$(mktemp -d)
trap 'rm -rf "$out"' EXIT

clang++ --target=x86_64-w64-mingw32 -std=c++20 -O1 -Wall -Wno-unknown-pragmas \
    -nostdinc -nostdinc++ \
    -isystem "$gcc_dir/include/c++" \
    -isystem "$gcc_dir/include/c++/x86_64-w64-mingw32" \
    -isystem "$(clang++ -print-resource-dir)/include" \
    -isystem "$cache/clang64/include" \
    -idirafter /usr/share/mingw-w64/include \
    -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 -DUNICODE -D_UNICODE \
    -include "$stubs/windhawk_api.h" -I"$stubs" \
    -x c++ "$src" -c -o "$out/mod.o"

x86_64-w64-mingw32-gcc -c "$stubs/windhawk_api.c" -o "$out/windhawk_api.o"

# Mixing clang objects with GCC's libstdc++ prints "duplicate section"
# warnings that don't matter here.
# shellcheck disable=SC2086
x86_64-w64-mingw32-g++-posix -shared -o "$out/mod.dll" "$out/mod.o" \
    "$out/windhawk_api.o" -static-libstdc++ -static-libgcc $libs 2>&1 |
    { grep -v "duplicate section" || true; }

if [ ! -s "$out/mod.dll" ]; then
    echo "Link failed" >&2
    exit 1
fi

echo "OK: $(basename "$src") compiles and links"
