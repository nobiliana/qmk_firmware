/* Copyright 2020
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

// Defines names for use in layer keycodes and the keymap
// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN1,
    _FN2,
    _FN3
};


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
    /* Base Layer
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │Esc│ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ \ │
     * ├───┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴───┤
     * │Tab │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Enter│
     * ├────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬────┤
     * │Shift │ Z │ X │ C │ V │ B │ N │ M │ , │ . │  / │
     * ├────┬─┴─┬─┴──┬┴───┴───┼───┴───┴──┬┴───┼───┼────┤
     * │Ctrl│GUI│Alt │        │ Backspace│Lowr│Rse│ Alt│
     * └────┴───┴────┴────────┴──────────┴────┴───┴────┘
     */
    [_BASE] = LAYOUT(
        QK_GESC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,             KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_BSPC,                   LOWER,   RAISE,   KC_RALT
    ),

    /* Function Layer 1 (Lower)
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │
     * └───┴┬──┴┬──┴┬──┴───┴───┴┬──┴┬──┴┬──┴┬──┴┬──┴───┘
     * │    │ = │ ▴ │   │   │   │ [ │ ] │ ; │ ' │      │
     * ├────└─┬─┴─┬─┴─┬───┐─┴─┬─└───┴───┴───┴───┘─┬────┤
     * │      │ ◂ │ ▾ │ ▸ │   │   │   │   │   │   │    │
     * ├────┬─└───┴───┴───┘───┌──────────┐┴───┼───┼────┤
     * │    │   │    │        │   Delete │    │   │    │
     * └────┴───┴────┴────────└──────────┘────┴───┴────┘
     */
    [_FN1] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        _______, KC_EQL,  KC_UP,   _______, _______, _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_QUOT,          _______,
        _______,          KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,                   _______, KC_DEL,                    _______, _______, _______
    ),

    /* Function Layer 2 (Raise)
     * ┌───┬───┬───┬───┬───┬───┬───┐───┌───┬───┐───┬───┐
     * │Rst│F1 │F2 │F3 │F4 │F5 │F6 │   │Hom│PgU│   │   │
     * └───┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐──└┬──┴┬──┴┐──┴───┤
     * │    │F7 │F8 │F9 │F10│F11│F12│   │ ; │ ' │      │
     * ├────└─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴───┴───┴───┴─┐────┤
     * │      │RGB│Mod│HuD│HuI│VaD│VaI│SaD│SaI│BLS│    │
     * ├────┬─└───┴───┴───┴───┴───┴───┴───┴───┴───┘────┤
     * │    │   │    │        │          │    │   │    │
     * └────┴───┴────┴────────┴──────────┴────┴───┴────┘
     */
    [_FN2] = LAYOUT(
        QK_BOOT,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   _______, KC_HOME, KC_PGUP, _______, _______,
        _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_END,  KC_PGDN,          _______,
        _______,          RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, RGB_VAD, RGB_VAI, RGB_SAD, RGB_SAI, BL_STEP, _______,
        _______, _______, _______,                   _______, _______,                   _______, _______, _______
    ),

    /* Empty 4th layer for VIA
     * ┌───┬───┬───┬───┬───┬───┬───┐───┌───┬───┐───┬───┐
     * |   |   |   |   |   |   |   |   |   |   |   |   |
     * └───┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐──└┬──┴┬──┴┐──┴───┤
     * │    │   │   │   │   │   │   │   │   │   │      │
     * ├────└─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴───┴───┴───┴─┐────┤
     * │      │   │   │   │   │   │   │   │   │   │    │
     * ├────┬─└───┴───┴───┴───┴───┴───┴───┴───┴───┘────┤
     * │    │   │    │        │          │    │   │    │
     * └────┴───┴────┴────────┴──────────┴────┴───┴────┘
     */
    [_FN3] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

tappy_dance LAYL_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
      if (!state->pressed)  return SINGLE_TAP;
      //only if key is released and NOT interrupted, but they key is still held. Means you want to send a 'HOLD'. Interrupt should lead to HOLD.
      else return SINGLE_HOLD;
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
    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case L1_HOLD: layer_on(_FN2); break;
    case L2_HOLD: layer_on(_FN1); break;
    case L3_HOLD: layer_on(_FN3); break;
    default: break;
  }
}
void LL_reset (tap_dance_state_t *state, void *user_data) {
  switch (ltap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case L1_HOLD: layer_off(_FN2); break;
    case L2_HOLD: layer_off(_FN1); break;
    case L3_HOLD: layer_off(_FN3); break;
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
    case R1_HOLD: layer_on(_FN1); break;
    case R2_HOLD: layer_on(_FN2); break;
    case R3_HOLD: layer_on(_FN3); break;
    default: break;
  }
}
void LR_reset (tap_dance_state_t *state, void *user_data) {
  switch (rtap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case R1_HOLD: layer_off(_FN1); break;
    case R2_HOLD: layer_off(_FN2); break;
    case R3_HOLD: layer_off(_FN3); break;
    default: break;
  }
  rtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
  [LAYL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LL_finished, LL_reset),
  [LAYR]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LR_finished, LR_reset)
};

