#include QMK_KEYBOARD_H
#include "keychron_common.h"

void send_one_thing(unsigned char cnt, char* type) {
    for (int i = 0; i < cnt; i++) {
        SEND_STRING(type);
    }
}
void send_tab_spaces(unsigned char cnt) {
    for (int i = 0; i < cnt; i++) {
        SEND_STRING(SS_TAP(X_SPACE));
        SEND_STRING(SS_TAP(X_TAB));
    }
}
// key for the tabs layout:
// this is a char array that is split into 8 sections.
// 0-32 is the number of tab-spaces to run
// 32-64 is just tabs,
// 64-96 is just spaces,
// 96-128 is down arrows
// 128-160 is up arrows
// for now the rest will remain unassigned
bool room_macro(uint8_t *tabs) {
    for (int i = 0; tabs[i] != '\0'; i++) {
        if (tabs[i] <= 32) {
            send_tab_spaces(tabs[i]);
        } else if (tabs[i] <= 64) {
            send_one_thing(tabs[i] - 32, SS_TAP(X_TAB));
        } else if (tabs[i] <= 96) {
            // send_spaces(tabs[i] - 64);
            send_one_thing(tabs[i] - 64, SS_TAP(X_SPACE));
        } else if (tabs[i] <= 128) {
            // send_down_arrow(tabs[i] - 96);
            send_one_thing(tabs[i] - 96, SS_TAP(X_1));
        } else if (tabs[i] <= 160) {
            // send_up_arrow(tabs[i] - 128);
            send_one_thing(tabs[i] - 128, SS_TAP(X_UP));
        }
    }
    return true;
}
