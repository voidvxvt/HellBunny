#include <Ldr.h>
#include <core/SysNative.h>
#include <core/Syscalls.h>
#include <common/phnt.h>


NTSTATUS NTAPI 
SysNtQuerySystemInformation(
    IN      SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT     PVOID                    SystemInformation,
    IN      ULONG                    SystemInformationLength,
    OUT OPT PULONG                   ReturnLength
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtQuerySystemInformation, SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtOpenProcess(
    OUT    PHANDLE            ProcessHandle,
    IN     ACCESS_MASK        DesiredAccess,
    IN     POBJECT_ATTRIBUTES ObjectAttributes,
    IN OPT PCLIENT_ID         ClientId
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtOpenProcess, ProcessHandle, DesiredAccess, ObjectAttributes, ClientId );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtAllocateVirtualMemory(
    IN     HANDLE    ProcessHandle,
    IN OUT PVOID*    BaseAddress,
    IN     ULONG_PTR ZeroBits,
    IN OUT PSIZE_T   RegionSize,
    IN     ULONG     AllocationType,
    IN     ULONG     Protect
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtAllocateVirtualMemory, ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtWriteVirtualMemory(
    IN      HANDLE ProcessHandle,
    IN      PVOID  BaseAddress,
    IN      PVOID  Buffer,
    IN      SIZE_T NumberOfBytesToWrite,
    OUT OPT PULONG NumberOfBytesWritten    
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtWriteVirtualMemory, ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtProtectVirtualMemory(
    IN     HANDLE  ProcessHandle,
    IN OUT PVOID*  BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG   NewProtect,
    OUT    PULONG  OldProtect
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtProtectVirtualMemory, ProcessHandle, BaseAddress, RegionSize, NewProtect, OldProtect );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtCreateThreadEx(
    OUT    PHANDLE                    ThreadHandle,
    IN     ACCESS_MASK                DesiredAccess,
    IN OPT POBJECT_ATTRIBUTES         ObjectAttributes,
    IN     HANDLE                     ProcessHandle,
    IN     PUSER_THREAD_START_ROUTINE StartRoutine,
    IN OPT PVOID                      Argument,
    IN     ULONG                      CreateFlags,
    IN     SIZE_T                     ZeroBits,
    IN     SIZE_T                     StackSize,
    IN     SIZE_T                     MaximumStackSize,
    IN OPT PPS_ATTRIBUTE_LIST         AttributeList
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtCreateThreadEx, ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, StartRoutine, Argument, CreateFlags, ZeroBits, StackSize, MaximumStackSize, AttributeList );

    return NtStatus;
}

NTSTATUS NTAPI
SysNtGetContextThread(
    IN     HANDLE   ThreadHandle,
    IN OUT PCONTEXT ThreadContext
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtGetContextThread, ThreadHandle, ThreadContext );

    return NtStatus;
}

NTSTATUS NTAPI
SysNtSetContextThread(
    IN     HANDLE   ThreadHandle,
    IN OUT PCONTEXT ThreadContext
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtSetContextThread, ThreadHandle, ThreadContext );

    return NtStatus;
}

NTSTATUS NTAPI
SysNtResumeThread(
    IN      HANDLE ThreadHandle,
    OUT OPT PULONG PreviousSuspendCount
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtResumeThread, ThreadHandle, PreviousSuspendCount );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtWaitForSingleObject(
    IN     HANDLE         Handle,
    IN     BOOLEAN        Alertable,
    IN OPT PLARGE_INTEGER Timeout
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtWaitForSingleObject, Handle, Alertable, Timeout );

    return NtStatus;
}

NTSTATUS NTAPI
SysNtTerminateThread(
    IN OPT HANDLE   ThreadHandle,
    IN     NTSTATUS ExitStatus
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtTerminateThread, ThreadHandle, ExitStatus );

    return NtStatus;
}

NTSTATUS NTAPI
SysNtTerminateProcess(
    IN OPT HANDLE   ProcessHandle,
    IN     NTSTATUS ExitStatus
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtTerminateProcess, ProcessHandle, ExitStatus );

    return NtStatus;
}

NTSTATUS NTAPI 
SysNtClose(
    IN HANDLE Handle
) {
    NTSTATUS   NtStatus  = STATUS_SUCCESS;
    SYS_CONFIG SysConfig = { 0 };

    SYSCALL_INVOKE( NtClose, Handle );

    return NtStatus;
}

