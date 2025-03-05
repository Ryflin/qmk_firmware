

char *decrypt(char *seed, int seedlen, char *data, int datalen) {
    for (int i = 0; i < datalen; i++) {
        data[i] = (char)(data[i] ^ seed[i % seedlen]);
    }
    return data;
}
