#ifndef MACROS_H
#define MACROS_H


typedef unsigned long long QWORD;

//
// concatenation macros
//
#define CONCAT_( prefix, suffix ) prefix##suffix
#define WSTRCAT_( prefix, suffix ) L## #prefix##L## #suffix
#define CSTRCAT_( prefix, suffix ) #prefix## #suffix
#define H_CAT_( H_XXX, prefix, suffix ) H_XXX##_##prefix##_##suffix

#define DLLEXPORT       __declspec( dllexport )

#ifdef _MSC_VER
    #define SEC_DATA    __declspec( allocate( ".data" ) )   // MSVC
#elif defined(__MINGW64__)
    #define SEC_DATA    __attribute__( section( ".data" ) ) // MinGW
#endif

#if defined(__MINGW64__)
    #define WIN_FUNC( x ) __typeof__( x ) * x;
#elif defined(_MSC_VER)
    #define WIN_TYPE( X ) typedef typeof( &X ) Fn##X;
    #define WIN_FUNC( X ) Fn##X X;
    #define WIN_API( X ) typeof( &X ) X;
    #define SYS_API( X ) typeof( X ) Sys##X // doesnt work as intended -> creates a data variable
#endif


#ifdef _WIN64
    #define PTEB_PTR __readgsqword( 0x30 )
    #define PPEB_PTR __readgsqword( 0x60 )
#else
    #define PTEB_PTR __readgsqword( 0x18 )
    #define PPEB_PTR __readgqdword( 0x30 )
#endif


//#define NT_SUCCESS( Status )    ( ( ( NTSTATUS ) ( Status ) ) >= 0 )
//#define NtCurrentProcess( )     ( ( HANDLE ) ( LONG_PTR ) - 1 )
//#define NtCurrentThread( )      ( ( HANDLE ) ( LONG_PTR ) - 2 )
#define NtGetLastError( )       Instance->Teb->LastErrorValue
#define NtSetLastError( x )     Instance->Teb->LastErrorValue = x

/* Heap Functions */
#define NtProcessHeap()         Instance->Teb->ProcessEnvironmentBlock->ProcessHeap
#define DATA_FREE( d, l )   \
    if ( d ) {              \
        MemSet( d, 0, l );  \
        MmHeapFree( d );    \
        d = NULL;           \
    }

//
// Base Address + Relative Virtual Address = Virtual Address
//
#define RVA2VA( TYPE, BASE, RVA ) ( TYPE )( ( UINT_PTR ) BASE + RVA )

//
// memory macros
//
#define MemCopy         __builtin__memcpy
#define MemSet          __stosb
#define MemZero( p, l ) __stosb( p, 0, l )

//
// casting macros
// 
#define C_PTR( x )   ( ( PVOID    ) ( x ) )
#define U_PTR( x )   ( ( UINT_PTR ) ( x ) )
#define B_PTR( x )   ( ( UINT8    ) ( x ) )
#define U_PTR16( x ) ( ( UINT16   ) ( x ) )
#define U_PTR32( x ) ( ( UINT32   ) ( x ) )
#define U_PTR64( x ) ( ( UINT64   ) ( x ) )

//
// dereference macros
//
#define DEREF( x )     ( ( PVOID ) *( PVOID*  ) ( x ) )
#define DEREF_U8( x )  ( ( BYTE  ) *( UINT8*  ) ( x ) )
#define DEREF_U16( x ) ( ( WORD  ) *( UINT16* ) ( x ) )
#define DEREF_U32( x ) ( ( DWORD ) *( UINT32* ) ( x ) )
#define DEREF_U64( x ) ( ( QWORD ) *( UINT64* ) ( x ) )

#define MAX( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )


#endif//MACROS_H
