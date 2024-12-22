#pragma once
#ifndef XOR_H
#define XOR_H

void
xor_crypt(
    char* buf,
    int   len,
    char  key
);

void
xor_strcrypt(
    unsigned char* buf,
    int            len
);

#endif XOR_H