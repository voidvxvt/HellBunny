#!/usr/bin/env python3

import sys
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes

KEY_LENGTH = 32 # 256 bit for AES256

def AES_256_CBC_Encrypt(data):
    key = get_random_bytes(KEY_LENGTH)
    iv = get_random_bytes(AES.block_size)
    cipher = AES.new(key, AES.MODE_CBC)
    ct_bytes = cipher.encrypt(pad(data, AES.block_size))
    return ct_bytes, key, iv

def c_hxdump(buf):
    return '{ ' + ''.join( [ f'{ hex( buf[c] ) }, ' for c in range( len( buf ) ) ] )[:-2] + ' };'


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("usage: ./aes256cbc.py <filename>")
        exit(0)

    filename = sys.argv[1]

    with open(filename, 'rb') as f:
        data = f.read()

    ct_bytes, key, iv = AES_256_CBC_Encrypt(data)

    with open(f"{filename}.enc", 'wb') as f:
        f.write(ct_bytes)

    print(f"written {filename}.enc")
    print(f'uint8_t key[32] = {c_hxdump(key)}')
    print(f'uint8_t iv[16] = {c_hxdump(iv)}')
