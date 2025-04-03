#include "../globals.h"
// #include <stdio.h>
void decrypt(char *seed, int seedlen, char *data, char *buffer, unsigned long datalen) {
    unsigned long index = 0;
    for (int i = 0; i < datalen; i++) {
        unsigned char temp = (data[i] ^ seed[i % seedlen]);
        if (temp > ' ' && temp <= '~') {
            buffer[index++] = temp;
        }
    }
    data[index] = '\0';
}

// void decrypt(char *seed, int seedlen, char *data, char *buffer, unsigned long datalen) {
//     // AES_ctx
// }


void decrypt_all(char *seed, int seelen) {
    for (int i = 0; passwords[i] != 0; i++) {
        decrypt(seed, seelen, passwords[i], password_sizes[i]);
    }
}
