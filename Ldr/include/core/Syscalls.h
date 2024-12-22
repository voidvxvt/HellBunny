#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <common/phnt.h>
#include <Config.h>

/* syscall functions */

#define SEARCH_RANGE      0xFF
#define SYSCALL_STUB_SIZE 0x20   // size of a syscall stub is 32 byte
#define SYSCALL_ABOVE     ( -1 * SYSCALL_STUB_SIZE )
#define SYSCALL_BELOW     SYSCALL_STUB_SIZE
#define SYSCALL_RANGE     0x14   // size of a syscall stub up to the syscall instruction
#define OPCODE_SYSCALL    0x050F // `syscall` instruction bytes
#define OPCODE_JMP        0xE9
#define OPCODE_RET        0xC3
#define OPCODE_MOV0       0x4C
#define OPCODE_R10        0x8B
#define OPCODE_MOV1       0xB8
#define OPCODE_RCX        0xD1
#define SSN_OFFSET_L      0x4    // offset to low  byte of SSN
#define SSN_OFFSET_H      0x5    // offset to high byte of SSN

#define SYS_EXTRACT( NtName )                                                               \
    if ( Instance->NtDll.NtName ) {                                                         \
        if ( SysExtract(                                                                    \
            Instance->NtDll.NtName,                                                         \
            TRUE,                                                                           \
            &Instance->Syscall.NtName,                                                      \
            NULL                                                                            \
        ) ) {                                                                               \
            PRINTF( "[+] Extracted: [%-33s] SSN:[%4X]", #NtName, Instance->Syscall.NtName ) \
        } else {                                                                            \
            PRINTF( "[-] SysExtract failed for %-33s", #NtName )                            \
        }                                                                                   \
    } else {                                                                                \
        PRINTF( "[-] Cannot extract Ssn for %-33s. Function Address not known.", #NtName )  \
    }

typedef struct _SYS_CONFIG {
    PVOID Adr; /* address of `syscall` instruction */
    WORD  Ssn; /* system service number */
} SYS_CONFIG, *PSYS_CONFIG;

BOOL
SysInitialize(
    IN PVOID NtDll
);

BOOL
SysExtract(
    IN  PVOID  FunctionAddress,
    IN  BOOL   ResolveHooked,
    OUT PWORD  Ssn,
    OUT PVOID* SysAddr
);

BOOL 
FindSsnOfHookedSyscall(
    IN  PVOID FunctionAddress,
    OUT PWORD Ssn
);


VOID
SysSetConfig(
    IN PSYS_CONFIG Config
);

NTSTATUS
SysInvokeIndirect(
    IN OUT /* Args... */
);

NTSTATUS
SysInvokeDirect(
    IN OUT /* Args... */
);

#endif // SYSCALLS_H