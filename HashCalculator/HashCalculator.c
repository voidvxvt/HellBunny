#include <windows.h>
#include <stdio.h>

#include "HashCalculator.h"
#define OPT


#define DJB2_KEY 5381
#define DJB2_STR "_DJB2"

UINT32
djb2(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
)
{
    UINT32 Hash = DJB2_KEY;
    PUCHAR Ptr  = String;

    if ( ! String )
        return 0;

    do {
        UCHAR chr = *Ptr;

        if ( ! Length ) {
            /* NULL terminated ? */
            if ( ! *Ptr )
                break;
        } else {
            /* position exceeded length of buffer ? */
            if ( ( ULONG ) ( Ptr - String ) >= Length )
                break;

            /* NULL terminated ? */
            if ( ! *Ptr ) {
                ++Ptr;
                continue;
            }
        }

        if ( Upper ) {
            if ( chr >= 'a' && chr <= 'z' )
                chr -= 0x20;
        }

        /* dbj2 hash the character */
        Hash = ( ( Hash << 5 ) + Hash ) + chr;

        ++Ptr;
    } while ( TRUE );

    return Hash;
}

#define DJB2HASH( STR, LEN, UPPER ) ( djb2( STR, LEN, UPPER ) )


#define FNV1A32_OFFSET_BASE 0x811C9DC5
#define FNV1A32_PRIME       0x01000193
#define FNV1A32_STR         "_FNV1A32"

UINT32
fnv1a32(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
)
{
    UINT32 Hash = FNV1A32_OFFSET_BASE;
    PUCHAR Ptr  = String;

    if ( ! String )
        return 0;

    do {
        UCHAR chr = *Ptr;

        if ( ! Length ) {
            /* NULL terminated ? */
            if ( ! *Ptr )
                break;
        } else {
            /* position exceeded length of buffer ? */
            if ( ( ULONG ) ( Ptr - String ) >= Length )
                break;

            /* NULL terminated ? */
            if ( ! *Ptr ) {
                ++Ptr;
                continue;
            }
        }

        if ( Upper ) {
            if ( chr >= 'a' && chr <= 'z' )
                chr -= 0x20;
        }

        /* fnv1a32 hash the character */
        Hash ^= ( UCHAR ) chr;
        Hash *= FNV1A32_PRIME;

        ++Ptr;
    } while ( TRUE );

    return Hash;
}

#define FNV1A32HASH( STR, LEN, UPPER ) ( fnv1a32( STR, LEN, UPPER ) )


#define CRC_POLYNOMIAL 0xEDB88320
#define CRC32B_STR     "_CRC32B"

UINT32
crc32b(
    IN PVOID     String,
    IN OPT ULONG Length,
    IN BOOL      Upper
)
{
    UINT32 Mask = 0x00;
    UINT32 Hash = 0xFFFFFFFF;
    PUCHAR Ptr  = String;

    if ( ! String )
        return 0;

    do {
        UCHAR chr = *Ptr;

        if ( ! Length ) {
            /* NULL terminated ? */
            if ( ! *Ptr )
                break;
        } else {
            /* position exceeded length of buffer ? */
            if ( ( ULONG ) ( Ptr - String ) >= Length )
                break;

            /* NULL terminated ? */
            if ( ! *Ptr ) {
                ++Ptr;
                continue;
            }
        }

        if ( Upper ) {
            if ( chr >= 'a' && chr <= 'z' )
                chr -= 0x20;
        }

        /* crc32b hash the character */
        Hash ^= chr;

        for ( INT ii = 0; ii < 8; ii++ ) {
            Mask = -1 * ( Hash & 1 );
            Hash = ( Hash >> 1 ) ^ ( CRC_POLYNOMIAL & Mask );
        }

        ++Ptr;
    } while ( TRUE );

    return ~Hash;
}

#define CRC32BHASH( STR, LEN, UPPER ) ( crc32b( STR, LEN, UPPER ) )


#define DJB2    FALSE
#define FNV1A32 FALSE
#define CRC32B  TRUE

#if ( defined( DJB2 ) && ( DJB2 == TRUE ) )
    #define HASH_ALG DJB2HASH
    #define HASH_STR DJB2_STR
#elif ( defined( FNV1A32 ) && ( FNV1A32 == TRUE ) )
    #define HASH_ALG FNV1A32HASH
    #define HASH_STR FNV1A32_STR
#elif ( defined( CRC32B ) && ( CRC32B == TRUE ) )
    #define HASH_ALG CRC32BHASH
    #define HASH_STR CRC32B_STR
#endif


int
main( void )
{
    DWORD idx = 0;

    for ( idx = 0; g_ExeArray[ idx ].String != NULL; ++idx ) {
        printf( "#define H_EXE_%s%s 0x%0.8X\n", g_ExeArray[ idx ].Name, HASH_STR, HASH_ALG( g_ExeArray[ idx ].String, g_ExeArray[ idx ].Length, TRUE) );
    }
    printf( "\n" );

    for ( idx = 0; g_DllArray[ idx ].String != NULL; ++idx ) {
        printf( "#define H_DLL_%s%s 0x%0.8X\n", g_DllArray[ idx ].Name, HASH_STR, HASH_ALG( g_DllArray[ idx ].String, g_DllArray[ idx ].Length, TRUE) );
    }
    printf( "\n" );

    for ( idx = 0; g_FunctionNameStringsArray[ idx ] != NULL; ++idx ) {
        printf( "#define H_API_%s%s 0x%0.8X\n", g_FunctionNameStringsArray[ idx ], HASH_STR, HASH_ALG( g_FunctionNameStringsArray[ idx ], NULL, FALSE ) );
    }

    return 0;
}