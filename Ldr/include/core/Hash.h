#ifndef HASH_H
#define HASH_H

#include <Config.h>

#define OPT

#define DJB2_KEY            5381
#define FNV1A32_OFFSET_BASE 0x811C9DC5
#define FNV1A32_PRIME       0x01000193
#define CRC_POLYNOMIAL      0xEDB88320


UINT32
djb2(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
);

UINT32
fnv1a32(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
);

UINT32
crc32b(
    IN     PVOID String,
    IN OPT ULONG Length,
    IN     BOOL  Upper
);


#define DJB2HASH(    STR, LEN, UPPER ) ( djb2(    STR, LEN, UPPER ) )
#define FNV1A32HASH( STR, LEN, UPPER ) ( fnv1a32( STR, LEN, UPPER ) )
#define CRC32BHASH(  STR, LEN, UPPER ) ( crc32b(  STR, LEN, UPPER ) )

#if ( defined( API_HASHING ) && ( API_HASHING == TRUE ) )

    #if ( defined( DJB2 ) && ( DJB2 == TRUE ) )
        #define HASH DJB2HASH
        #define HASH_ALG DJB2
    #elif ( defined( FNV1A32 ) && ( FNV1A32 == TRUE ) )
        #define HASH FNV1A32HASH
        #define HASH_ALG FNV1A32
    #elif ( defined( CRC32B ) && ( CRC32B == TRUE ) )
        #define HASH CRC32BHASH
        #define HASH_ALG CRC32B
    #endif

    #else
        #define HASH     NONE
        #define HASH_ALG NONE
#endif // OBF_HASHING == TRUE

#endif//HASH_H