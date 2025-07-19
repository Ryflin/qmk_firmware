#include "../globals.h"
void decrypt(char *seed, int seedlen, char *data, unsigned short datalen) {
    unsigned short index = 0;
    for (unsigned short i = 0; i < datalen; i++) {
        unsigned char temp = (data[i] ^ seed[i % seedlen]);
        if (!(temp & 0x1)) {
            data[index++] = temp / 2;
        }
    }
    data[index] = '\0';
}

void decrypt_all(char *seed, int seelen) {
    for (int i = 0; passwords[i] != 0 || password_sizes[i] != 0; i++) {
        decrypt(seed, seelen, passwords[i], password_sizes[i]);
    }
}
