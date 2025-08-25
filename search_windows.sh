# headers and macros
rg -n '\b(windows\.h|winsock2\.h|ws2tcpip\.h|shlwapi\.h|shellapi\.h|objbase\.h|ole2\.h|atlbase\.h|afxwin\.h)\b'
rg -n '\b(_WIN32|_WIN64|_MSC_VER|WINAPI|__declspec|__uuidof|__try|__except|__finally)\b'

# linker pragmas and MSVC hints
rg -n '#pragma\s+comment\(lib,\s*".*"\)'
rg -n '\b(__declspec\(dllexport|dllimport\)|__stdcall|__fastcall)\b'

# Win32 symbols
rg -n '\b(CreateFileW|ReadFile|WriteFile|CloseHandle|HANDLE|OVERLAPPED|GetLastError|Sleep|GetTickCount|FILE_ATTRIBUTE_|CRITICAL_SECTION|InterlockedCompareExchange|CreateThread)\b'
rg -n '\b(WSAStartup|WSACleanup|SOCKET|closesocket|recv|send|getaddrinfo)\b'
rg -n '\b(CoInitialize|CoCreateInstance|IUnknown|BSTR|VARIANT|GUID|CLSID)\b'
rg -n '\b(RegOpenKeyEx|RegQueryValueEx|HKEY_|SHGetKnownFolderPath|PathCombine)\b'