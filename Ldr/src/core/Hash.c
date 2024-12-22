#include <Ldr.h>
#include <core/Hash.h>
#include <common/Macros.h>


// https://github.com/realoriginal/titanldr-ng/blob/master/Hash.c
UINT32
djb2(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
) {
    UINT32 Hash = DJB2_KEY;
    PUCHAR Ptr  = String;

    if ( ! String ) {
        return 0;
    }

    do {
        UCHAR chr = *Ptr;

        if ( ! Length ) {
            /* NULL terminated ? */
            if ( ! *Ptr ) {
                break;
            }
        } else {
            /* position exceeded length of buffer ? */
            if ( ( ULONG ) ( Ptr - String ) >= Length ) {
                break;
            }

            /* NULL terminated ? */
            if ( ! *Ptr ) {
                ++Ptr;
                continue;
            }
        }

        if ( Upper ) {
            if ( chr >= 'a' && chr <= 'z' ) {
                chr -= 0x20;
            }
        }

        /* dbj2 hash the character */
        Hash = ( ( Hash << 5 ) + Hash ) + chr;

        ++Ptr;
    } while ( TRUE );

    return Hash;
}


UINT32
fnv1a32(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
) {
    UINT32 Hash = FNV1A32_OFFSET_BASE;
    PUCHAR Ptr  = String;

    if ( ! String ) {
        return 0;
    }

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


UINT32
crc32b(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
) {
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

        for ( INT ii = 0; ii < 8; ii++ )
        {
            Mask = -1 * ( Hash & 1 );
            Hash = ( Hash >> 1 ) ^ ( CRC_POLYNOMIAL & Mask );
        }

        ++Ptr;
    } while ( TRUE );

    return ~Hash;
}
