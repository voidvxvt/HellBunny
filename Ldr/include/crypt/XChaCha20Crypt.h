/*************************************************************************
 * This is a small library for the XChaCha20 encryption algorithm. This  *
 * library is based on Daniel J. Bernstein's ChaCha reference            *
 * implementation, which can be found here: http://cr.yp.to/chacha.html  *
 * The xchacha_hchacha() and xchacha_set_counter functions are based on  *
 * code found in libsodium. To find the associated license and more      *
 * info., look in the NOTICE file.                                       *
 *************************************************************************/
#ifndef XCHACHA20_H_
#define XCHACHA20_H_

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

 /** Key and IV sizes that are supported by XChaCha20.
  *  All sizes are in bits.
  */
#define NAME "XChaCha20"
#define KEYSIZE 256                 /* 256-bits, 32 bytes */
#define BLOCKSIZE 512               /* 512-bits, 64 bytes */
#define IVSIZE 192                  /* 192-bits, 24 bytes */


  /* XChaCha20 block size in bytes */
#define XCHACHA_BLOCKLENGTH 64


/* The following macros are used to obtain exact-width results. */
#define U8V(v) ((uint8_t)(v) & (0xFF))
#define U16V(v) ((uint16_t)(v) & (0xFFFF))
#define U32V(v) ((uint32_t)(v) & (0xFFFFFFFF))
#define U64V(v) ((uint64_t)(v) & (0xFFFFFFFFFFFFFFFF))


/** The following macros return words with their bits rotated over n
 *  positions to the left/right.
 */
#define ROTL32(v, n) \
  (U32V((v) << (n)) | ((v) >> (32 - (n))))


 /** The following macros load words from an array of bytes with
  *  different types of endianness, and vice versa.
  */
#define U8TO32_LITTLE(p) \
  (((uint32_t)((p)[0])      ) | \
   ((uint32_t)((p)[1]) <<  8) | \
   ((uint32_t)((p)[2]) << 16) | \
   ((uint32_t)((p)[3]) << 24))

#define U32TO8_LITTLE(p, v) \
  do { \
    (p)[0] = U8V((v)      ); \
    (p)[1] = U8V((v) >>  8); \
    (p)[2] = U8V((v) >> 16); \
    (p)[3] = U8V((v) >> 24); \
  } while (0)


#define ROTATE(v,c) (ROTL32(v,c))
#define XOR(v,w) ((v) ^ (w))
#define PLUS(v,w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v),1))


  /* The ChaCha quarter round */
#define QUARTERROUND(a,b,c,d) \
  a = PLUS(a,b); d = ROTATE(XOR(d,a),16); \
  c = PLUS(c,d); b = ROTATE(XOR(b,c),12); \
  a = PLUS(a,b); d = ROTATE(XOR(d,a), 8); \
  c = PLUS(c,d); b = ROTATE(XOR(b,c), 7);


/** ChaCha_ctx is the structure containing the representation of the
 *  internal state of the XChaCha20 cipher.
 *
 */
typedef struct
{
    uint32_t input[16];
} XChaCha20Ctx;

typedef XChaCha20Ctx XCHACHA20CTX;


/* ------------------------------------------------------------------------- */


/** hchacha an intermediary step towards XChaCha20 based on the
 * construction and security proof used to create XSalsa20.
 * @param out Holds output of hchacha
 * @param in The input to process with hchacha
 * @param k The key to use with hchacha
 *
 */
void xchacha_hchacha20(uint8_t* out, const uint8_t* in, const uint8_t* k);


/** Set the encryption key and iv to be used with XChaCha
 * @param ctx The XChaCha context to use
 * @param k The 256-bit/32-byte key to use for encryption
 * @param iv The 192-bit/24-byte iv or nonce to use
 * @note It is the user's responsibility to ensure that the key
 * and the iv are of the correct lengths!
 */
void xchacha_keysetup(XChaCha20Ctx* ctx, const uint8_t* k, uint8_t* iv);


/** Set the internal counter to a specific number. Depending
 * on the specification, sometimes the counter is started at 1.
 * @param ctx The XChaCha context to modify
 * @param counter The number to set the counter to
 *
 */
void xchacha_set_counter(XChaCha20Ctx* ctx, uint8_t* counter);


/** Encryption/decryption of arbitrary length messages.
 *
 *  For efficiency reasons, the API provides two types of
 *  encrypt/decrypt functions. The xchacha_encrypt_bytes() function
 *  (declared here) encrypts byte strings of arbitrary length, while
 *  the xchacha_encrypt_blocks() function (defined later) only accepts
 *  lengths which are multiples of CHACHA_BLOCKLENGTH.
 *
 *  The user is allowed to make multiple calls to
 *  xchacha_encrypt_blocks() to incrementally encrypt a long message,
 *  but he is NOT allowed to make additional encryption calls once he
 *  has called xchacha_encrypt_bytes() (unless he starts a new message
 *  of course). For example, this sequence of calls is acceptable:
 *
 *  xchacha_keysetup();
 *
 *  xchacha_ivsetup();
 *  xchacha_encrypt_blocks();
 *  xchacha_encrypt_blocks();
 *  xchacha_encrypt_bytes();
 *
 *  xchacha_ivsetup();
 *  xchacha_encrypt_blocks();
 *  xchacha_encrypt_blocks();
 *
 *  xchacha_ivsetup();
 *  xchacha_encrypt_bytes();
 *
 *  The following sequence is not:
 *
 *  xchacha_keysetup();
 *  xchacha_ivsetup();
 *  xchacha_encrypt_blocks();
 *  xchacha_encrypt_bytes();
 *  xchacha_encrypt_blocks();
 *
 */


 /** Encrypt a set of bytes with XChaCha20
  * @param ctx The XChaCha20 context to use
  * @param plaintext The data to be encrypted
  * @param ciphertext A buffer to hold the encrypted data
  * @param msglen Message length in bytes
  *
  */
void xchacha_encrypt_bytes(XChaCha20Ctx* ctx, const uint8_t* plaintext,
    uint8_t* ciphertext,
    uint32_t msglen);


/** Dencrypt a set of bytes with XChaCha20
 * @param ctx The XChaCha20 context to use
 * @param ciphertext The encrypted data to decrypt
 * @param plaintext A buffer to hold the decrypted data
 * @param msglen Message length in bytes
 *
 */
void xchacha_decrypt_bytes(XChaCha20Ctx* ctx, const uint8_t* ciphertext,
    uint8_t* plaintext,
    uint32_t msglen);


/** For testing purposes it can sometimes be useful to have a function
 *  which immediately generates keystream without having to provide it
 *  with a zero plaintext.
 *  @param ctx The XChaCha context to use
 *  @param keystream A buffer to hold the keystream
 *  @param length Length of keystream in bytes
 *
 */
void xchacha_keystream_bytes(XChaCha20Ctx* ctx, uint8_t* keystream, uint32_t length);


/** Encrypt/decrypt of blocks.
 *  @param ctx The XChaCha context to use
 *  @param plaintext A buffer which holds unencrypted data
 *  @param ciphertext A buffer which holds encrypted data
 *  @param blocks The number of 512 blocks to process with XChaCha20
 *
 */
#define xchacha_encrypt_blocks(ctx, plaintext, ciphertext, blocks)         \
    xchacha_encrypt_bytes(ctx, plaintext, ciphertext,                      \
    (blocks) * XCHACHA_BLOCKLENGTH)


#define xchacha_decrypt_blocks(ctx, ciphertext, plaintext, blocks)         \
    xchacha_decrypt_bytes(ctx, ciphertext, plaintext,                      \
    (blocks) * XCHACHA_BLOCKLENGTH)


#define xchacha_keystream_blocks(ctx, keystream, blocks)                   \
    xchacha_keystream_bytes(ctx, keystream,                                \
    (blocks) * XCHACHA_BLOCKLENGTH)


#endif
