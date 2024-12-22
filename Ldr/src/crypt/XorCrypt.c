#include <crypt/XorCrypt.h>

void
xor_crypt( 
    char* buf, 
    int   len, 
    char  key
) {
    for ( int i = 0; i < len; i++ ) {
        buf[i] ^= key;
    }
}

void
xor_strcrypt(
    unsigned char* buf,
    int            len
) {
    unsigned char key = buf[ len - 1 ]; // 0x00 XOR key == key
    for ( int i = 0; i < len; i++ ) {
        buf[i] ^= key;
    }
}
