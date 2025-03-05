#include "aes.h"

void decrypt(char *seed, int seedlen, char *data, int datalen) {
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) {
        iv[i] = data[i];
    }
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, (const uint8_t *)seed, iv);
    AES_CBC_decrypt_buffer(&ctx, (uint8_t *)(data + 16), datalen);

}
