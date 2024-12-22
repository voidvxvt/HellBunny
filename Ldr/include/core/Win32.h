#pragma once
#ifndef WIN32_H
#define WIN32_H

#include <common/Macros.h>
#include <common/phnt.h>
#include <Config.h>


#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )

PVOID
LdrGetModuleBase(
    IN DWORD Hash
);

PVOID
LdrGetFuncAddr(
    IN PVOID Module,
    IN DWORD Hash
);

BOOL
ProcessSearchId(
    IN  DWORD  ProcHash,
    OUT PDWORD ProcId
);

#else

PVOID
LdrGetModuleBase(
    IN PWSTR Name
);
PVOID

LdrGetFuncAddr(
    IN PVOID Module,
    IN PSTR  Name
);

BOOL
ProcessSearchId(
    IN  PWSTR  ProcName,
    OUT PDWORD ProcId
);

#endif // API_HASHING == TRUE

PVOID
LdrSearchModule(
    IN PWSTR ModuleName
);
PVOID
LdrLoadModule(
    IN PSTR ModuleName
);

NTSTATUS
ProcessSnapShot(
    OUT PSYSTEM_PROCESS_INFORMATION* SnapShot,
    OUT PSIZE_T                      Size
);

HANDLE
ProcessOpen(
    IN DWORD Pid,
    IN DWORD Access
);


#endif // WIN32_H