#ifndef _AES_H
#define _AES_H

typedef unsigned char uint8_t;
typedef unsigned long long size_t;

// state - matrix holding the intermediate results during encryption
// this layout is great for converting between matrix and array
typedef uint8_t MATRIX[4][4];
typedef MATRIX *PMATRIX; /*
{   c0  c1  c2  c3
  {  0,  1,  2,  3 }, r0
  {  4,  5,  6,  7 }, r1
  {  8,  9, 10, 11 }, r2
  { 12, 13, 14, 15 }, r3
} */

#define AES128 0
#define AES256 1
#define CTR 1
#define CBC 0

#define AES_BLOCKLEN 16 // block length in bytes - AES is 128b block only

#if defined(AES256) && (AES256 == 1)
    #define AES_KEYLEN 32
    #define AES_ExpKeySize 240
#elif defined (AES128) && (AES128 == 1)
    #define AES_KEYLEN 16
    #define AES_ExpKeySize 176
#endif

typedef struct _AESCTX {
    uint8_t ExpandedKey[AES_ExpKeySize];
    uint8_t Iv[AES_BLOCKLEN];
} AESCTX, *PAESCTX;

void AesInitCtx( PAESCTX ctx, const uint8_t* key, const uint8_t* iv );

#if defined(CBC) && (CBC == 1)
void AesSetCtxIv( PAESCTX ctx, const uint8_t* iv );
// buffer size must be multiple of AES_BLOCKLEN
void AesCbcEncryptBuffer( PAESCTX ctx, uint8_t* buf, size_t len );
void AesCbcDecryptBuffer( PAESCTX ctx, uint8_t* buf, size_t len );
#endif
#if defined(CTR) && (CTR == 1)
void AesCtrXCryptBuffer( PAESCTX ctx, uint8_t* buf, size_t len );
#endif

#endif // _AES_H_

