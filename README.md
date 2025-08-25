#8j45NKY9AfKgG6h# Identifying and Replacing Windows-Specific Dependencies in a C++ Project

This document outlines practical steps to surface **Windows-only dependencies** in your codebase and replace them with cross-platform counterparts so you can **cross-compile from Linux to Windows** with minimal issues.

---

## 1. Inventory What Your Current Windows Build Links

First, identify which Win32/COM/CRT libraries your app currently uses.

### On Windows (MSVC / VS2010)
- Rebuild in *Release* mode with:
  - `/VERBOSE:LIB` in the linker flags (Project → Properties → Linker → Command Line → Additional Options).
- Inspect the log for implicit libraries:
  - Examples: `kernel32.lib`, `user32.lib`, `ws2_32.lib`, `ole32.lib`, `gdi32.lib`, `advapi32.lib`, `shell32.lib`, `shlwapi.lib`, `crypt32.lib`.
- After building, run:
  ```powershell
  dumpbin /dependents your.exe
  dumpbin /imports your.exe

This shows imported DLLs and symbols.


On Linux

llvm-readobj --coff-imports your.exe
objdump -p your.exe


---

2. Dump the Include Tree

Headers imply APIs. Produce an include map:

MSVC: /showIncludes

Clang: clang++ -E -H file.cpp


Look for:

windows.h, winsock2.h, shlwapi.h, objbase.h, ole2.h, atlbase.h

afxwin.h (MFC), shellapi.h, commdlg.h, commctrl.h

wincrypt.h, winhttp.h, gdiplus.h, d3d*.h, etc.



---

3. Grep for Windows-isms

Search your repo for Windows macros and APIs:

rg -n '\b(windows\.h|winsock2\.h|shlwapi\.h)\b'
rg -n '\b(_WIN32|_WIN64|WINAPI|__declspec)\b'
rg -n '#pragma\s+commentlib,\s*".*"'
rg -n '\b(CreateFileW|ReadFile|CloseHandle|Sleep|GetTickCount)\b'
rg -n '\b(WSAStartup|SOCKET|recv|send|getaddrinfo)\b'
rg -n '\b(CoInitialize|CoCreateInstance|IUnknown|BSTR)\b'


---

4. Let the Cross-Compiler Flush Out the Rest

Set up a MinGW-w64 cross toolchain:

x86_64-w64-mingw32-g++ file.cpp -o file.exe

Missing symbols will surface as linker errors.

Use -Wl,--trace or -Wl,--verbose to see linked libs.



---

5. Map Windows APIs to Portable Replacements

Windows usage	Cross-platform alternative

CreateFileW, ReadFile, WriteFile	C++17 <filesystem>, <fstream>
Sleep, GetTickCount	std::this_thread::sleep_for, <chrono>
CreateThread, CRITICAL_SECTION	std::thread, std::mutex, std::atomic
Winsock (WSAStartup, SOCKET)	Asio / Boost.Asio
Registry (RegOpenKeyEx)	Config files: JSON/TOML/YAML
COM (CoInitializeEx, IUnknown)	Abstract behind interface or replace with non-COM lib
GUI: Win32/GDI	Qt, wxWidgets, SDL2, Dear ImGui
Crypto (Crypt32)	OpenSSL, libsodium



---

6. Encapsulate in a Portability Layer

Create platform/ (or os/) with clean interfaces for FS, net, time, threads, etc.

Implement per-platform: platform/windows/, platform/posix/.

Keep #if defined(_WIN32) inside those modules only.



---

7. Replace Preprocessor Sprawl

Avoid scattering _WIN32 checks across app logic.

Centralize them in your platform layer.



---

8. Use CMake for Portability

Even if your project is VS2010-only now, add CMake:

Easier to manage cross builds.

Use try_compile / check_symbol_exists to detect APIs.

Add CI checks that fail when windows.h sneaks in.



---

9. Automated Checks

Ban Windows headers outside platform/windows/:

#ifdef _WIN32
#ifndef ALLOW_WINDOWS_HEADERS
#error "windows.h forbidden outside platform/ layer"
#endif
#endif

CI grep for windows.h, #pragma comment(lib, ...).

Compile with Clang using -Wmicrosoft to detect MSVC-only extensions.



---

10. Handling Windows-Only Functionality

Keep behind an abstraction layer.

Provide stubs, reduced-functionality, or portable alternatives elsewhere.

Document platform deltas.



---

11. SBOM Angle

Once cleaned up:

Generate SBOM (SPDX or CycloneDX) with tools.

Include:

Compiler/runtime (MinGW, libstdc++)

Third-party libs (Asio, OpenSSL, etc.)

System/DLL imports (ideally trending to zero).




---

12. Minimal Checklist

1. Run /VERBOSE:LIB on Windows build.


2. Run dumpbin /imports.


3. Log includes with /showIncludes.


4. Grep repo for _WIN32, windows.h, etc.


5. Attempt cross-compile with MinGW-w64.


6. Create platform/ abstraction.


7. Replace Win32 with std, Asio, filesystem, etc.


8. Start lightweight SBOM.




---

Next Steps

If you share dumpbin /imports or some grep hits, we can map each item directly to replacements and migration strategies.


Would you like me to also include some **example SBOM entries** in SPDX/CycloneDX format inside this README so you can seed your own SBOM right away?

