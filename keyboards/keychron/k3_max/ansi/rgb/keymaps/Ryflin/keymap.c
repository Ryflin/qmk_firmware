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
#include "features/math.h"
// #include "led_matrix/led_matrix.h"
#include "globals.h"

#define SPACE_LAYER_SHIFT LT(1, KC_SPC)

#define TD_D_R TD(TD_D_RIGHT)
#define TD_H_D TD(TD_DEL_H)
#define TD_E_F TD(TD_ESC_CAPS)
#define KC_M_1 KC_MS_BTN1
#define KC_M_2 KC_MS_BTN2
#define KC_M_3 KC_MS_BTN3
#define KC_M_4 KC_MS_BTN4
#define MS_W_R KC_MS_WH_RIGHT
#define MS_W_L KC_MS_WH_LEFT
#define M_LG_A LGUI_T(KC_A)
#define M_LA_R LALT_T(KC_R)
#define M_LS_S LSFT_T(KC_S)
#define M_LC_T LCTL_T(KC_T)
#define M_RC_N RCTL_T(KC_N)
#define M_RS_E RSFT_T(KC_E)
#define M_RA_I RALT_T(KC_I)
#define M_RG_O RGUI_T(KC_O)


enum layers { MAC_BASE, MAC_FN, WIN_BASE, WIN_FN, LAYR1, LAYR2 };
enum custom_keycodes {
    OSU_PSS = SAFE_RANGE,
    MR_SIGN,
    MR_ROOM,
    MR_RV3,
    C_PASS,
    CHK_PASS,
    SND_AWE,
    HOMEY_P,
    HOME_PS,
    PSS_FIX,
    HOM_KEY,
    MATH,
    MY_HELP,
    MY_VCMD,
    SND_KEY,
};
bool math;
bool checking_password;
char password[17];
int  password_index;

uint8_t room_enhanced[] = {32 + 21, 4, 160 + 3, 32 + 1, 3, 32 + 1, 3, 32 + 2, 2, 32 + 4, 5, 128 + 3, 32 + 1, 4, 128 + 1, 32 + 1, 10, 32 + 1, 6, 0};
uint8_t room_v3[]       = {32 + 21, 4, 160 + 3, 32 + 1, 5, 32 + 1, 3, 32 + 2, 128 + 2, 32 + 1, 18, 32 + 1, 6, 0};
char    help_string[]   = "This is the help.\nMathMode\nEnter='spc+5'\nPlus:'g'\nmult:'tab'\nMinus:'p'\nDiv:'/'\n\nVim:sp+l=:w\n:!";

enum { TD_ESC_CAPS, TD_DEL_H, TD_D_RIGHT, TD_SPC_UNDR };
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_ESC, WIN_FN),
    [TD_DEL_H]   = ACTION_TAP_DANCE_DOUBLE(KC_H, C(KC_BSPC)),
    [TD_D_RIGHT]  = ACTION_TAP_DANCE_DOUBLE(KC_D, KC_RIGHT),

};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_CAPS,  KC_DEL,   _______,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_G,     KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     TD_E_F,   M_LG_A,   M_LA_R,   M_LS_S,   M_LC_T,   KC_D,     TD_H_D,   M_RC_N,   M_RS_E,   M_RA_I,   M_RG_O,   KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_K,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                             SPACE_LAYER_SHIFT,                         KC_RALT,  _______,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_84(
     _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_SNAP,  _______,  RGB_TOG,
     MR_SIGN,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    MATH,     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            MR_ROOM,
     KC_ASTR,  KC_CIRC,  KC_AMPR,  KC_PIPE,  KC_MINUS, KC_PLUS, _______,   MY_VCMD,  DT_UP,    DT_DOWN,  DT_PRNT,  _______,  _______,  _______,            MR_RV3,
     KC_COLN,  KC_EXLM,  KC_UNDS,  KC_DLR,   KC_EQL,   KC_PERC,  MY_HELP,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, KC_UNDS,            _______,            _______,
     KC_LCTL,            KC_BSLS,  KC_HASH,  KC_LCBR,  KC_K,     BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
     OSU_PSS,  _______,  _______,                                _______,                                _______,  C_PASS,   _______,  _______,  _______,  AC_TOGG ),

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
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI, KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
    PSS_FIX,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    SND_KEY,  _______, _______,  _______,  _______,  _______,   _______,  _______,  _______,            _______,
    HOMEY_P,  HOME_PS,  HOM_KEY,  MS_W_L,   MS_W_R,   SND_AWE,  _______, KC_MS_WH_UP,KC_MS_WH_DOWN,KC_M_2,KC_M_3, _______,  _______,  _______,            _______,
    TG(WIN_FN),KC_M_4,  KC_M_3,   KC_M_2,   KC_M_1,   _______,  KC_M_1,  KC_MS_L,  KC_MS_UP, KC_MS_D,  KC_MS_R,   _______,            _______,            _______,
    CHK_PASS,           _______,  _______,  _______,  _______,  BAT_LVL, NK_TOGG,  _______,  _______,  _______,   _______,            _______,  _______,  _______,
    _______,  _______,  _______,                                      KC_SPC,                          _______,   _______,  _______,  _______,  _______,  _______)
};

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        if (!process_record_keychron_common(keycode, record)) {
            return false;
        }
        return true;
    }
    if (math) {
        if (keycode == KC_ESC) {
            math = false;
            return false;
        }
        if (keycode == KC_EQL) {
            apply_operations();
        } else if (keycode == KC_ENT) {
            uint16_to_string();
        } else if (keycode >= KC_A && keycode <= KC_F) {
            fill_math(keycode + 93);
        } else if (keycode == KC_0) {
            fill_math('0');
        } else if (keycode >= KC_1 && keycode <= KC_9) {
            fill_math(keycode + 19);
        } else if (keycode == M_RS_E) { // shit that is hardcoded to the exact finger that is determining a and e
            fill_math('e');
        } else if (keycode == M_LG_A) {
            fill_math('a');
        } else {
            cache_operation(keycode);
        }

        // SEND_STRING(temp);
        return false;
    }
    if (checking_password) {
        if (keycode == KC_ENT || password_index > 16) {
            password[password_index] = '\0';
            decrypt_all(password, password_index);
            checking_password = false;
        } else {
            password[password_index++] = keycode + 93;
            // password[password_index++] = keycode + 19;
        }
        return false;
    }
    switch (keycode) {
        case HOME_PS:
            SEND_STRING(passwords[2]);
            break;
        case HOMEY_P:
            SEND_STRING(passwords[1]);
            break;
        case SND_AWE:
            SEND_STRING(passwords[3]);
            break;
        case CHK_PASS:
            SEND_STRING(password);
            break;
        case C_PASS:
            checking_password = true;
            password_index    = 0;
            break;
        case MR_SIGN:
            SEND_STRING("schley.20" SS_DELAY(10) SS_TAP(X_TAB));
            SEND_STRING(passwords[0]);
            SEND_STRING(SS_DELAY(100) SS_TAP(X_ENT));
            break;
        case OSU_PSS:
            SEND_STRING(passwords[0]);
            break;
        case PSS_FIX:
            decrypt_all(password, password_index);
            return false;
        case MR_RV3:
            room_macro(room_v3);
            break;
        case MR_ROOM:
            room_macro(room_enhanced);
            break;
        case MATH:
            math = true;
            return false;
        case MY_HELP:
            SEND_STRING(help_string);
            break;
        case MY_VCMD:
            SEND_STRING(SS_TAP(X_ESC) ":w\n:!");
            break;
        case SND_KEY:
            SEND_STRING(passwords[4]);
            break;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case 3:
                rgb_matrix_set_color(i, RGB_BLUE);
                break;
            case 1:
                rgb_matrix_set_color(i, RGB_GREEN);
                break;
            default:
                break;
        }
        if (math && i > 0x10) {
            rgb_matrix_set_color(i, RGB_AZURE);
        }
        if (i > 0 && i <= 0x10) {
            if (in_number(i)) {
                rgb_matrix_set_color(i, RGB_GREEN);
            } else {
                if (i % 4 == 0) {
                    rgb_matrix_set_color(i, RGB_BLUE);
                } else {
                    rgb_matrix_set_color(i, RGB_RED);
                }
            }
        }
    }
    return false;
}
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_D_R:
        case TD_E_F:
            return TAPPING_TERM + 200;
        case M_LG_A:
            return TAPPING_TERM + 75;
        case M_RC_N:
            return TAPPING_TERM + 50;
        default:
            return TAPPING_TERM;
    }
}
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(1, KC_BSPC):
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}
