#include <Ldr.h>
#include <core/Win32.h>
#include <common/phnt.h>
#include <common/Macros.h>
#include <core/MiniStd.h>
#include <core/Memory.h>
#include <core/Hash.h>
#include <core/SysNative.h>
#include <common/Defines.h>

#include <common/Debug.h>


/*!
 * @brief
 *  get module base address from the PEB->InLoadOrderModuleList
 * 
 * @param Hash
 *  hashed uppercase name of module ( NTDLL.DLL )
 * 
 * @return
 *  pointer to module base
 */
PVOID
LdrGetModuleBase(
#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    IN DWORD Hash
#else
    IN PWSTR Name
#endif
) {
    PLDR_DATA_TABLE_ENTRY Dte = NULL;
    PLIST_ENTRY           Hdr = NULL;
    PLIST_ENTRY           Ent = NULL;

    Hdr = & ( NtCurrentPeb()->Ldr->InLoadOrderModuleList );

    for ( Ent = Hdr->Flink; Hdr != Ent; Ent = Ent->Flink ) {
        Dte = ( PLDR_DATA_TABLE_ENTRY ) Ent;

#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
        if ( Hash == 0 || HASH( Dte->BaseDllName.Buffer, Dte->BaseDllName.Length, TRUE ) == Hash ) {
#else
        if ( Name == 0 || IsStringEqualW( Dte->BaseDllName.Buffer, Name ) == TRUE ) {
#endif
            return Dte->DllBase;
        }
    }

    return NULL;
}


/*!
 * @brief
 *  parses EAT of given module and 
 *  hash each exported function, toget address of function from module by parsing it's EAT
 * 
 * @param Module
 *  module base address
 * 
 * @param Hash
 *  hashed function name
 * 
 * @return
 *  pointer to function address
 */
PVOID
LdrGetFuncAddr(
    IN PVOID ModuleBase,
#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    IN DWORD Hash
#else
    IN PSTR  Name
#endif
) {
    PIMAGE_NT_HEADERS       ImgNtHdrs     = { 0 };
    PIMAGE_EXPORT_DIRECTORY ImgExpDir     = { 0 };
    SIZE_T                  ImgExpDirSize = { 0 };
    PDWORD                  FuncNameArray = { 0 };
    PDWORD                  FuncAddrArray = { 0 };
    PWORD                   FuncOrdnArray = { 0 };
    PVOID                   FuncAddr      = { 0 };
    PCHAR                   FuncName      = { 0 };
    ANSI_STRING             AnsiString    = { 0 };

#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    if ( ! ModuleBase || ! Hash ) {
#else 
    if ( ! ModuleBase || ! Name ) {
#endif
        return NULL;
    }

    ImgNtHdrs = ( UINT_PTR ) ModuleBase + ( ( PIMAGE_DOS_HEADER ) ModuleBase )->e_lfanew;
    if ( ImgNtHdrs->Signature != IMAGE_NT_SIGNATURE ) {
        return NULL;
    }

    //
    // parse EDT from Optional Header
    //
    ImgExpDir     = RVA2VA( PVOID, ModuleBase, ImgNtHdrs->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );
    ImgExpDirSize = RVA2VA( DWORD, ModuleBase, ImgNtHdrs->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size );

    FuncNameArray = RVA2VA( PVOID, ModuleBase, ImgExpDir->AddressOfNames );         // Export Name Pointer Table
    FuncAddrArray = RVA2VA( PVOID, ModuleBase, ImgExpDir->AddressOfFunctions );     // Export Address Table (EAT)
    FuncOrdnArray = RVA2VA( PVOID, ModuleBase, ImgExpDir->AddressOfNameOrdinals );  // Export Ordinal Table

    //
    // iterate over EAT
    //
    for ( DWORD i = 0; i < ImgExpDir->NumberOfFunctions; i++ )
    {
        //
        // retrieve the current function name
        //
        FuncName = RVA2VA( PCHAR, ModuleBase, FuncNameArray[ i ] );

        //
        // hash function name to compare with given hash
        //
#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
        if ( HASH( FuncName, 0, FALSE ) == Hash ) {
#else 
        if ( IsStringEqualA( Name, FuncName ) == TRUE ) {
#endif
            //
            // get function address
            //
            FuncAddr = RVA2VA( PVOID, ModuleBase, FuncAddrArray[ FuncOrdnArray[ i ] ] );

            //
            // if this is a forwarded function, use LdrGetProcedureAddress
            // 
            if ( U_PTR( FuncAddr ) >= U_PTR( ImgExpDir ) &&
                 U_PTR( FuncAddr ) <  U_PTR( ImgExpDir ) + ImgExpDirSize )
            {
                //
                // use LdrGetProcedureAddress to lazily resolve the forwarded func
                //
                WIN_API( LdrGetProcedureAddress );
                LdrGetProcedureAddress = LdrGetFuncAddr( LdrGetModuleBase( H_DLL( NTDLL ) ), H_API( LdrGetProcedureAddress ) );

                AnsiString.Length        = StrLenA( FuncName );
                AnsiString.MaximumLength = AnsiString.Length + sizeof( CHAR );
                AnsiString.Buffer        = FuncName;

                if ( ! NT_SUCCESS( LdrGetProcedureAddress( ModuleBase, &AnsiString, 0, &FuncAddr ) ) ) {
                    return NULL;
                }
            }

            PRINTF( "[+] [ %0.4d ] FOUND API FUNCTION - NAME: %-33s - ADDRESS: 0x%p - ORDINAL: %d", i, FuncName, FuncAddr, FuncOrdnArray[ i ] )
            return FuncAddr;
        }
    }
#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    PRINTF( "[-] API not found: FunctionHash:[%lx]", Hash )
#else
    PRINTF( "[-] API not found: FunctionName:[%s]", Name )
#endif
    return NULL;
}


PVOID
LdrSearchModule(
    IN PWSTR ModuleName
) {
    PVOID                 FirstEntry = NULL;
    PLDR_DATA_TABLE_ENTRY Entry      = NULL;

    FirstEntry = & ( NtCurrentPeb()->Ldr->InLoadOrderModuleList.Flink );
    Entry      =     NtCurrentPeb()->Ldr->InLoadOrderModuleList.Flink;
 
    do {
        if ( IsStringEqualW( ModuleName, Entry->BaseDllName.Buffer ) ) {
            return Entry->DllBase;
        }

        Entry = Entry->InLoadOrderLinks.Flink;

    } while ( Entry != FirstEntry );

    return NULL;
}


PVOID
LdrLoadModule(
    IN PSTR ModuleName
) {
    UNICODE_STRING UnicodeString     = { 0 };
    WCHAR          NameW[ MAX_PATH ] = { 0 };
    PVOID          Module            = { 0 };
    USHORT         DestSize          = 0;
    NTSTATUS       NtStatus          = STATUS_SUCCESS;

    if ( ! ModuleName ) {
        return NULL;
    }

    /* convert module ansi string to unicode string */
    CharStringToWCharString( NameW, ModuleName, StrLenA( ModuleName ) );

    DestSize = StrLenW( NameW ) * sizeof( WCHAR );

    /* check if the module is already loaded */
    Module = LdrSearchModule( NameW );
    
    /* if found, avoid generating an image-load event */
    if ( Module ) {
        return Module;
    }

    if ( Instance->NtDll.LdrLoadDll ) {
        PRINTF( "[i] Loading Module %s using LdrLoadDll", ModuleName )

        /* prepare unicode string */
        UnicodeString.Buffer        = NameW;
        UnicodeString.Length        = DestSize;
        UnicodeString.MaximumLength = DestSize;

        if ( ! NT_SUCCESS( NtStatus = Instance->NtDll.LdrLoadDll( NULL, 0, &UnicodeString, &Module ) ) ) {
            PRINTF( "[-] LdrLoadDll failed. NtStatus: %p", NtStatus )
            NtSetLastError( NtStatus );
        }
    }

    MemZero( NameW, sizeof( NameW ) );
    MemZero( & UnicodeString, sizeof( UnicodeString ) );

    PRINTF( "[+] Module \"%s\": %p", ModuleName, Module )

    return Module;
}


NTSTATUS
ProcessSnapShot(
    OUT PSYSTEM_PROCESS_INFORMATION* SnapShot,
    OUT PSIZE_T                      Size
) {
    ULONG    Length   = 0;
    NTSTATUS NtStatus = STATUS_SUCCESS;

    if ( ! SnapShot || ! Size ) {
        return STATUS_INVALID_PARAMETER;
    }

    if ( STATUS_INFO_LENGTH_MISMATCH == ( NtStatus = SysNtQuerySystemInformation( 
        SystemProcessInformation, 
        NULL, 
        0, 
        & Length 
    ) ) ) {
        PRINTF( "[i] SystemProcessInformation Length: %d", Length )

        /* just in case that some processes or threads were created between the calls */
        //Length += 0x1000;

        *SnapShot = MmHeapAlloc( Length );
        if ( *SnapShot ) {
            if ( ! NT_SUCCESS( NtStatus = SysNtQuerySystemInformation( 
                SystemProcessInformation, 
                *SnapShot, 
                Length, 
                &Length 
            ) ) ) {
                PRINTF( "[-] 2nd Call to NtQuerySystemInformation failed. NtStatus: 0x%0.8X", NtStatus )
                MmHeapFree( *SnapShot );
                goto LEAVE;
            }
        } else NtStatus = STATUS_NO_MEMORY;
            
        *Size = Length;
    } else {
        PRINTF( "[-] 1st Call to NtQuerySystemInformation failed. NtStatus: 0x%0.8X", NtStatus )
    }

LEAVE:

    return NtStatus;
}


BOOL
ProcessSearchId(
#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    IN  DWORD  ProcHash,
#else
    IN  PWSTR  ProcName,
#endif
    OUT PDWORD ProcId
) {
    PSYSTEM_PROCESS_INFORMATION SystemProcInfo  = NULL;
    PSYSTEM_PROCESS_INFORMATION PtrSysProcInfo  = NULL; /* will hold the original pointer of SysProcInfo */
    SIZE_T                      SysProcInfoSize = 0;

#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
    if ( ! ProcId || ! ProcHash ) {
#else
    if ( ! ProcId || ! ProcName ) {
#endif
        return FALSE;
    }

    if ( ! NT_SUCCESS( ProcessSnapShot( & SystemProcInfo, & SysProcInfoSize ) ) ) {
        PUTS( "[-] ProcessSnapshot failed." )
        return FALSE;
    }

    /* save the original pointer to free */
    PtrSysProcInfo = SystemProcInfo;

    //
    // iterate over proces snapshot linked list
    //
    while ( SystemProcInfo->NextEntryOffset ) 
    {
        if ( ! SystemProcInfo->ImageName.Length || SystemProcInfo->ImageName.Length >= MAX_PATH )
            goto _NEXT_ELEMENT;

        //
        // convert process name to uppercase
        //
        WCHAR UprProcName[ MAX_PATH ] = { 0 };
        
        for ( INT i = 0; i < SystemProcInfo->ImageName.Length; i++ ) {
            if ( SystemProcInfo->ImageName.Buffer[i] >= 'a' && SystemProcInfo->ImageName.Buffer[i] <= 'z' )
                UprProcName[i] = SystemProcInfo->ImageName.Buffer[i] - 'a' + 'A';
            else
                UprProcName[i] = SystemProcInfo->ImageName.Buffer[i];
        }

#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )
        if ( SystemProcInfo->ImageName.Length != 0 && HASH( SystemProcInfo->ImageName.Buffer, SystemProcInfo->ImageName.Length, TRUE ) == ProcHash ) {
#else
        if ( SystemProcInfo->ImageName.Length != 0 && IsStringEqualW( SystemProcInfo->ImageName.Buffer, ProcName ) == TRUE ) {
#endif
            *ProcId = ( DWORD )SystemProcInfo->UniqueProcessId;
            break;
        }

_NEXT_ELEMENT:
        /* move to the next element in the linked list */
        SystemProcInfo = RVA2VA( PSYSTEM_PROCESS_INFORMATION, SystemProcInfo, SystemProcInfo->NextEntryOffset );
    }

    MmHeapFree( PtrSysProcInfo );
    
    if ( *ProcId == NULL ) {
        return FALSE;
    }

    return TRUE;
}


HANDLE
ProcessOpen(
    IN DWORD Pid,
    IN DWORD Access
) {
    HANDLE            ProcessHandle = NULL;
    CLIENT_ID         Client        = { 0 };
    OBJECT_ATTRIBUTES ObjAttr       = { 0 };
    NTSTATUS          NtStatus      = STATUS_SUCCESS;

    InitializeObjectAttributes( & ObjAttr, NULL, 0, NULL, NULL );

    Client.UniqueProcess = C_PTR( Pid );

    if ( ! NT_SUCCESS( NtStatus = SysNtOpenProcess( 
        & ProcessHandle, 
        Access, 
        & ObjAttr, 
        & Client 
    ) ) ) {
        PRINTF( "[-] SysNtOpenProcess failed. NtStatus: 0x%0.8X", NtStatus )
        return INVALID_HANDLE_VALUE;
    }

    return ProcessHandle;
}
