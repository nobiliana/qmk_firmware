/* Copyright 2021 Danny Nguyen <danny@keeb.io>
   Copyright 2023 Surnia (aka nobiliana)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  L1_HOLD,
  L2_HOLD,
  L3_HOLD,
  R1_HOLD,
  R2_HOLD,
  R3_HOLD,
  SINGLE_TAP
//  DOUBLE_TAP
} tappy_dance;

typedef struct {
  bool is_press_action;
  tappy_dance state;
} tap;

enum td_keycodes {
  LAYL, //left layer switcher. 
  LAYR //right layer switcher.
};


tappy_dance LAYL_dance (tap_dance_state_t *state);
tappy_dance LAYR_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void LL_finished (tap_dance_state_t *state, void *user_data);
void LL_reset (tap_dance_state_t *state, void *user_data);

void LR_finished (tap_dance_state_t *state, void *user_data);
void LR_reset (tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_MUTE, KC_ESC,  KC_GRV,  KC_1,    KC_2,    KC_3,     KC_4,    KC_5,        KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
    KC_HOME,          KC_TAB,  KC_Q,    KC_W,    KC_E,     KC_R,    KC_T,        KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
    KC_PGUP,          KC_LCTL, KC_A,    KC_S,    KC_D,     KC_F,    KC_G,        KC_H,    KC_J,     KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
    KC_PGDN,          KC_LSFT, KC_Z,    KC_X,    KC_C,     KC_V,    KC_B,        KC_N,    KC_M,     KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_PSCR,
    KC_END,           _______, KC_LGUI, KC_LALT, TD(LAYL), _______,  KC_SPC,      KC_SPC,  TD(LAYR), KC_RALT, KC_RGUI, KC_MENU, KC_RCTL
  ),
  [1] = LAYOUT(
    KC_MUTE, _______, _______, _______, _______, _______,  _______, _______,     _______, _______,  _______, _______, _______, _______, _______, KC_DEL,
    _______,          _______, KC_HOME, KC_UP,   KC_END,   KC_PGUP, _______,     _______, KC_P7,    KC_P8,   KC_P9,   KC_P0,   KC_PENT, C(KC_C), C(KC_V),
    _______,          _______, KC_LEFT, KC_DOWN, KC_RGHT,  KC_PGDN, _______,     C(KC_F), KC_P4,    KC_P5,   KC_P6,   KC_PPLS, KC_PAST,          _______,
    _______,          _______, KC_ENT,  KC_BSPC, KC_DEL,   _______, _______,     KC_TAB,  KC_P1,    KC_P2,   KC_P3,   KC_PMNS, KC_PSLS, _______,
    _______,          _______, _______, _______, KC_TRNS,  _______, _______,     _______, KC_TRNS,  KC_PDOT, _______, _______, _______
  ),
  [2] = LAYOUT(
    _______, _______, _______, _______, _______, _______,  _______, _______,     _______, _______,  _______, _______, _______, _______, _______, KC_INS,
    _______,          _______, KC_F9,   KC_F10,  KC_F11,   KC_F12,  _______,     _______, KC_AMPR,  KC_ASTR, KC_LPRN, KC_RPRN, _______, _______, KC_NUM,
    _______,          _______, KC_F5,   KC_F6,   KC_F7,    KC_F8,   _______,     _______, KC_DLR,   KC_PERC, KC_CIRC, _______, _______,          KC_CAPS,
    _______,          _______, KC_F1,   KC_F2,   KC_F3,    KC_F4,   _______,     _______, KC_EXLM,  KC_AT,   KC_HASH, _______, _______, KC_SCRL,
    _______,          _______, _______, _______, KC_TRNS,  _______, _______,     _______, KC_TRNS,  _______, _______, _______, _______
  ),
  [3] = LAYOUT(
    _______, _______, BL_BRTG, BL_UP,    BL_DOWN, _______,  _______, _______,     _______, _______,  _______, _______, _______, _______, _______, _______,
    BL_TOGG,          _______, RGB_HUD,  RGB_HUI, _______,  _______, _______,     _______, _______,  _______, _______, _______, _______, _______, _______,
    RGB_TOG,          _______, RGB_SAD,  RGB_SAI, _______,  _______, _______,     _______, _______,  _______, _______,   _______, _______,          _______,
    RGB_MOD,          _______, RGB_VAD,  RGB_VAI, _______,  _______, _______,     _______, _______,  _______, _______, _______, _______, _______,
    RGB_RMOD,         _______, RGB_M_TW, RGB_M_B, KC_TRNS,  _______, _______,     _______, KC_TRNS,  _______, _______, _______, _______
  )
};

 #ifdef ENCODER_MAP_ENABLE
    const uint16_t PROGMEM encoder_map[][1][2] = {
        [0] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
        [1]  = { ENCODER_CCW_CW(KC_DOWN, KC_UP) },
        [2]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
        [3]  = { ENCODER_CCW_CW(_______, _______) }        
    };
#endif

tappy_dance LAYL_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
        return L1_HOLD;
    case 2:
        return L2_HOLD;
    case 3:
        return L3_HOLD;
    default:
        return TD_UNKNOWN;
  }
};

tappy_dance LAYR_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
        return R1_HOLD;
    case 2:
        return R2_HOLD;
    case 3:
        return R3_HOLD;
    default:
        return TD_UNKNOWN;
  }
};

static tap ltap_state = {
  .is_press_action = true,
  .state = TD_NONE
};

void LL_finished (tap_dance_state_t *state, void *user_data) {
  ltap_state.state = LAYL_dance(state);
  switch (ltap_state.state) {
//    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case L1_HOLD: layer_on(2); break;
    case L2_HOLD: layer_on(1); break;
    case L3_HOLD: layer_on(3); break;
    default: break;
  }
}
void LL_reset (tap_dance_state_t *state, void *user_data) {
  switch (ltap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case L1_HOLD: layer_off(2); break;
    case L2_HOLD: layer_off(1); break;
    case L3_HOLD: layer_off(3); break;
    default: break;
  }
  ltap_state.state = TD_NONE;
}

static tap rtap_state = {
  .is_press_action = true,
  .state = TD_NONE
};
void LR_finished (tap_dance_state_t *state, void *user_data) {
  rtap_state.state = LAYR_dance(state);  
  switch (rtap_state.state) {
//    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case R1_HOLD: layer_on(1); break;
    case R2_HOLD: layer_on(2); break;
    case R3_HOLD: layer_on(3); break;
    default: break;
  }
}
void LR_reset (tap_dance_state_t *state, void *user_data) {
  switch (rtap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case R1_HOLD: layer_off(1); break;
    case R2_HOLD: layer_off(2); break;
    case R3_HOLD: layer_off(3); break;
    default: break;
  }
  rtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
  [LAYL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LL_finished, LL_reset),
  [LAYR]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LR_finished, LR_reset)
};

