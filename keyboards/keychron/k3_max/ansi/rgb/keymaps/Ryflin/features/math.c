#include QMK_KEYBOARD_H
#include "keychron_common.h"
uint16_t a             = 0;
uint16_t operation     = KC_Q;
uint16_t b             = 0;
bool     hex           = true;
bool     bin_rep[0x11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
bool     ans           = false;
bool verbose = false;

int hex_char_to_int(unsigned char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else
        return -1; // invalid hex character
}

bool in_number_array(int i, uint16_t n) {
    return (n >> (0x10 - i)) & 0x1;
}
void convert_to_bin(uint16_t n) {
    // int index = 16;
    for (int i = 17; i > 0; i--) {
        bin_rep[i - 1] = in_number_array(i, n);
    }
    bin_rep[16] = bin_rep[0];
}

void uint16_to_string(void) {
    char buffer[17];
    if (hex) {
        // Convert to hexadecimal format
        buffer[0] = 'a';
        buffer[1] = 'x';
        for (int i = 0; i < 4; i++) {
            uint8_t nibble   = (a >> (12 - 4 * i)) & 0xF;
            char    hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            buffer[i + 2]    = hex_char;
        }
        buffer[6] = 'b';
        buffer[7] = '0';
        buffer[9] = 'x';
        for (int i = 0; i < 4; i++) {
            uint8_t nibble   = (b >> (12 - (4 * i)) & 0xF);
            char    hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            buffer[i + 10]   = hex_char;
        }
        buffer[14] = 'b';
        buffer[15] = '\n';
        buffer[16] = '\0';
        SEND_STRING(buffer);
    } else {
        // Convert to decimal format
        if (a == 0) {
            buffer[0] = '0';
            buffer[1] = '\0';
            return;
        }

        int  index = 0;
        char temp[6]; // Max 5 digits for uint16_t + null terminator

        while (a > 0) {
            temp[index++] = '0' + (a % 10);
            a /= 10;
        }

        // Reverse the string
        for (int i = 0; i < index; i++) {
            buffer[i] = temp[index - i - 1];
        }
        buffer[index] = '\0';
    }
}


void fill_math(unsigned char c) {
    // char buff[2];
    // buff[0] = c;
    // buff[1] = '\0';
    // SEND_STRING(buff);
    // SEND_STRING("\n");
    ans = false;

    if (hex) {
        b = (b << 4) + hex_char_to_int(c);
    } else {
        b = (b * 10) + hex_char_to_int(c);
    }
    // uint16_to_string();
}
void cache_operation(uint16_t keycode) {
    operation = keycode;
    if (!ans) {
        a = b;
    }
    convert_to_bin(a);
    b = 0;
    // SEND_STRING("CACHE OPERATION\n");
}

void apply_operations(void) {
    // char buff[2] = {98, 0};
    switch (operation) {
        case KC_G: // *^&|+-
            a = a + b;
            break;
        case KC_TAB:
            a = a * b;
            break;
        case KC_P:
            a = a - b;
            break;
        case KC_SLSH:
            a = a / b;
            break;
        case KC_Q:
            a = a ^ b;
            break;
        case KC_W:
            a = a & b;
            break;
        case KC_N:
            a = a | b;
            break;
        case KC_DOT:
            a = a >> b;
            break;
        case KC_COMM:
            a = a << b;
            break;
        case KC_D:
            a = a % b;
            break;
    }

    convert_to_bin(a);
    b   = 0;
    // if (verbose) {
    //     uint16_to_string();
    // }
    ans = true;
}

bool in_number(int i) {
    return bin_rep[i];
}

// plan 1 have the color degradations determine location. This is better as it introduces doing hex. problem duplicate values
