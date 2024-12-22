#!/usr/bin/env python3

import sys
from Crypto.Cipher import ChaCha20
from Crypto.Random import get_random_bytes


def XChaCha20_Encrypt(data):
    key = get_random_bytes(32)
    nonce = get_random_bytes(24)
    cipher = ChaCha20.new(key=key, nonce=nonce)
    ct_bytes = cipher.encrypt(data)
    return ct_bytes, key, nonce

def c_hxdump(buf):
    return '{ ' + ''.join( [ f'{ hex( buf[c] ) }, ' for c in range( len( buf ) ) ] )[:-2] + ' };'


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("usage: ./xchacha20.py <filename>")
        exit(0)

    filename = sys.argv[1]

    with open(filename, 'rb') as f:
        data = f.read()

    ct_bytes, key, nonce = XChaCha20_Encrypt(data)

    with open(f"{filename}.enc", 'wb') as f:
        f.write(ct_bytes)

    print(f"written {filename}.enc")
    print(f'uint8_t key[32] = {c_hxdump(key)}')
    print(f'uint8_t nonce[24] = {c_hxdump(nonce)}')