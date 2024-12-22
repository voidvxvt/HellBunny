#ifndef SYSNATIVE_H
#define SYSNATIVE_H

#include <common/phnt.h>
#include <common/Macros.h>
#include <Config.h>

#include <common/Debug.h>


#ifndef OPT
#define OPT
#endif//OPT

#if ( defined( SYS_INDIRECT ) && ( SYS_INDIRECT == TRUE ) )
// INDIRECT SYSCALLS
#define SYSCALL_INVOKE( NtName, ... )                                                                                       \
    if ( Instance->Syscall.SysAddress && Instance->Syscall.NtName ) {                                                       \
        PRINTF( "[i] Executing indirect Syscall for [" #NtName "] with SysAddress at %p", Instance->Syscall.SysAddress )    \
        SysConfig.Ssn = Instance->Syscall.NtName;                                                                           \
        SysConfig.Adr = Instance->Syscall.SysAddress;                                                                       \
        SysSetConfig( & SysConfig );                                                                                        \
        NtStatus = SysInvokeIndirect( __VA_ARGS__ );                                                                        \
    } else {                                                                                                                \
        PRINTF( "[i] Executing potentially hooked NTAPI [" #NtName "] at Address %p", Instance->NtDll.NtName )              \
        NtStatus = Instance->NtDll.NtName( __VA_ARGS__ );                                                                   \
    }                                                                                                                       \
    PRINTF( "[i] %s( ... ) = %08x", #NtName, NtStatus )

#elif ( defined( SYS_SYSCALLS ) && ( SYS_SYSCALLS == TRUE ) )
// DIRECT SYSCALLS
#define SYSCALL_INVOKE( NtName, ... )                                                                                       \
    if ( Instance->Syscall.NtName ) {                                                                                       \
        PRINTF( "[i] Executing direct Syscall for [" #NtName "] with SysAddress at %p", Instance->Syscall.SysAddress )      \
        SysConfig.Ssn = Instance->Syscall.NtName;                                                                           \
        SysSetConfig( & SysConfig );                                                                                        \
        NtStatus = SysInvokeDirect( __VA_ARGS__ );                                                                          \
    } else {                                                                                                                \
        PRINTF( "[i] Executing potentially hooked NTAPI [" #NtName "] at Address %p", Instance->NtDll.NtName )              \
        NtStatus = Instance->NtDll.NtName( __VA_ARGS__ );                                                                   \
    }                                                                                                                       \
    PRINTF( "[i] %s( ... ) = %08x", #NtName, NtStatus )

#else
// NTAPI
#define SYSCALL_INVOKE( NtName, ... )                                                                                       \
    PRINTF( "[i] Executing potentially hooked NTAPI [" #NtName "] at Address %p", Instance->NtDll.NtName )                  \
    NtStatus = Instance->NtDll.NtName( __VA_ARGS__ );                                                                       \
    PRINTF( "[i] %s( ... ) = %08x", #NtName, NtStatus )     
#endif

NTSTATUS NTAPI 
SysNtQuerySystemInformation(
    IN      SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT     PVOID                    SystemInformation,
    IN      ULONG                    SystemInformationLength,
    OUT OPT PULONG                   ReturnLength
);

NTSTATUS NTAPI 
SysNtOpenProcess(
    OUT    PHANDLE            ProcessHandle,
    IN     ACCESS_MASK        DesiredAccess,
    IN     POBJECT_ATTRIBUTES ObjectAttributes,
    IN OPT PCLIENT_ID         ClientId
);

NTSTATUS NTAPI 
SysNtAllocateVirtualMemory(
    IN     HANDLE    ProcessHandle,             // process handle in where to allocate memory 
    IN OUT PVOID*    BaseAddress,               // the returned allocated memory's base address
    IN     ULONG_PTR ZeroBits,                  // always set to 0
    IN OUT PSIZE_T   RegionSize,                // size of memory to allocate. will return the memory size rounded up to the page size (512 -> 4096)
    IN     ULONG     AllocationType,            // MEM_COMMIT | MEM_RESERVE
    IN     ULONG     Protect                    // page protection
);

NTSTATUS NTAPI 
SysNtWriteVirtualMemory(
    IN      HANDLE ProcessHandle,               // process handle whose memory is to be written to
    IN      PVOID  BaseAddress,                 // base address in the specified process to which data is written
    IN      PVOID  Buffer,                      // data to be written
    IN      SIZE_T NumberOfBytesToWrite,        // number of bytes to be written 
    OUT OPT PULONG NumberOfBytesWritten         // pointer to a variable that receives the number of bytes actually written
);

NTSTATUS NTAPI 
SysNtProtectVirtualMemory(
    IN     HANDLE  ProcessHandle,               // process handle whose memory protection is to be changed
    IN OUT PVOID*  BaseAddress,                 // pointer to the base address of the memory region to protect
    IN OUT PSIZE_T RegionSize,                  // pointer to the size of the memory region to protect
    IN     ULONG   NewProtect,                  // new memory protection to be set
    OUT    PULONG  OldProtect                   // pointer to a variable that receives the previous page access protection
);

NTSTATUS NTAPI 
SysNtCreateThreadEx(
    OUT    PHANDLE                    ThreadHandle,         // pointer to a HANDLE variable that receives the created thread's handle
    IN     ACCESS_MASK                DesiredAccess,        // thread's access rights ( set to THREAD_ALL_ACCESS - 0x1FFFFF )
    IN OPT POBJECT_ATTRIBUTES         ObjectAttributes,     // pointer to OBJECT_ATTRIBUTES structure ( set to NULL )
    IN     HANDLE                     ProcessHandle,        // handle to the process in which the thread is to be created
    IN     PUSER_THREAD_START_ROUTINE StartRoutine,         // base address of the application-defined function to be executed
    IN OPT PVOID                      Argument,             // pointer to a variable to be passed to the thread function ( set to NULL )
    IN     ULONG                      CreateFlags,          // the flags that control the creation of the thread ( set to NULL )
    IN     SIZE_T                     ZeroBits,             // set to NULL
    IN     SIZE_T                     StackSize,            // set to NULL
    IN     SIZE_T                     MaximumStackSize,     // set to NULL
    IN OPT PPS_ATTRIBUTE_LIST         AttributeList         // pointer to PS_ATTRIBUTE_LIST structure ( set to NULL )
);

NTSTATUS NTAPI
SysNtGetContextThread(
    IN     HANDLE   ThreadHandle,
    IN OUT PCONTEXT ThreadContext
);

NTSTATUS NTAPI
SysNtSetContextThread(
    IN     HANDLE   ThreadHandle,
    IN OUT PCONTEXT ThreadContext
);

NTSTATUS NTAPI
SysNtResumeThread(
    IN      HANDLE ThreadHandle,
    OUT OPT PULONG PreviousSuspendCount
);

NTSTATUS NTAPI 
SysNtWaitForSingleObject(
    IN     HANDLE         Handle,
    IN     BOOLEAN        Alertable,
    IN OPT PLARGE_INTEGER Timeout
);

NTSTATUS NTAPI
SysNtTerminateThread(
    IN OPT HANDLE   ThreadHandle,
    IN     NTSTATUS ExitStatus
);

NTSTATUS NTAPI
SysNtTerminateProcess(
    IN OPT HANDLE   ProcessHandle,
    IN     NTSTATUS ExitStatus
);

NTSTATUS NTAPI 
SysNtClose(
    IN HANDLE Handle
);

#endif // SYSNATIVE_H