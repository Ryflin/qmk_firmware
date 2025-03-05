/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "features/room_macro.h"
#include "features/decrypt.h"
#include "globals.h"

#define SPACE_LAYER_SHIFT LT(1, KC_SPC)
#define TD_E_F TD(TD_ESC_CAPS)

enum layers { MAC_BASE, MAC_FN, WIN_BASE, WIN_FN, LAYR1, LAYR2 };
enum custom_keycodes {
    OSU_PSS = SAFE_RANGE,
    MR_SIGN,
    MR_ROOM,
    C_PASS,
    CHK_PASS,
    SND_AWE,
    SND_HOM,
};

bool           checking_password;
char           password[17];
int            password_index;
unsigned char *room_main = dreese_room_matrix;
enum {
    TD_ESC_CAPS,
    TD_B1_3,
};
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_ESC, WIN_FN),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_CAPS,  KC_DEL,   _______,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_APP,
     KC_TAB,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_G,     KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     TD_E_F,   KC_A,     KC_R,     KC_S,     KC_T,     KC_D,     KC_H,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_K,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                             SPACE_LAYER_SHIFT,                         KC_RALT,  C_PASS,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_84(
     _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_SNAP,  _______,  RGB_TOG,
     MR_SIGN,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     KC_ASTR,  KC_CIRC,  KC_AMPR,  KC_PIPE,  KC_PLUS,  KC_MINUS, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     KC_COLN,  KC_TILD,  KC_PERC,  KC_DLR,   KC_EQL,   KC_PERC,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, KC_UNDS,            _______,            _______,
     KC_LCTL,            KC_BSLS,  KC_HASH,  _______,  KC_K,     BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
     OSU_PSS,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[WIN_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

// [LAYR1] = LAYOUT_ansi_84(
//      _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
//      _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
//      RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
//      _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
//      _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
//      _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),
// [LAYR2] = LAYOUT_ansi_84(
//      _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
//      _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
//      RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
//      _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
//      _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
//      _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[WIN_FN] = LAYOUT_ansi_84(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,    KC_MPRV,    KC_MPLY,      KC_MNXT,   KC_MUTE,   KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,    _______,    _______,      _______,   _______,   _______,  _______,  _______,            _______,
    C_PASS,   _______,  _______,  _______,  _______,  _______,  _______,    KC_MS_WH_UP,KC_MS_WH_DOWN,KC_MS_BTN2,KC_MS_BTN3,_______,  _______,  _______,            _______,
  TG(WIN_FN), SND_HOM,  _______,  _______,  _______,  _______,  KC_MS_BTN1, KC_MS_L,    KC_MS_UP,     KC_MS_D,   KC_MS_R,   _______,            _______,            _______,
    CHK_PASS,           _______,  _______,  _______,  _______,  BAT_LVL,    NK_TOGG,    _______,      _______,   _______,   _______,            _______,  _______,  _______,
    SND_AWE,  _______,  _______,                                      KC_SPC,                                    _______,   _______,  _______,  _______,  _______,  _______)
};
// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (checking_password) {
        if (keycode == KC_ENT || password_index > 16) {
            password[password_index] = '\0';
            checking_password        = false;
            decrypt(password, password_index, lamb, 5);
            decrypt(password, password_index, awesome, 15);
            decrypt(password, password_index, my_home, 17);

            return false;
        } else {
            if (record->event.pressed && keycode < 40) {
                password[password_index++] = keycode + 93;
            }
        }
        return false;
    }
    switch (keycode) {
        case SND_HOM:
            if (record->event.pressed) {
                SEND_STRING(my_home);
            }
        case SND_AWE:
            if (record->event.pressed) {
                SEND_STRING(awesome);
            }
            break;
        case CHK_PASS:
            if (record->event.pressed) {
                SEND_STRING(password);
            }
            break;
        case C_PASS:
            if (record->event.pressed) {
                checking_password = true;
                password_index    = 0;
            }
            return true;
        // macros
        case MR_SIGN:
            // goto mr_sign;
            // mr_sign:
            if (record->event.pressed) {
                SEND_STRING("schley.20" SS_TAP(X_TAB));
                SEND_STRING(osu_crypt);
                SEND_STRING(SS_TAP(X_ENT));
            }
            break;
        case OSU_PSS:
            if (record->event.pressed) {
                SEND_STRING(test);
            }
            break;
        case MR_ROOM:
            if (record->event.pressed) {
                room_macro(room_main);
            }
    }
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    return true;
}
