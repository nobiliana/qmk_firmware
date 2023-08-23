// Copyright 2023 Kyle McCreery (@kylemccreery)
// SPDX-License-Identifier: GPL-2.0-or-later

 #include QMK_KEYBOARD_H

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
  LAYL = QK_KB, //left layer switcher. 
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
    /* Base */
    [_BASE] = LAYOUT(
    KC_TAB,           KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_ENT,   KC_BSPC,
    KC_LCTL,          KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,              KC_O,
    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,             KC_COMM, KC_DOT,   KC_RSFT,
    MO(2), KC_LGUI, KC_LALT,          KC_SPC,             TD(LAYL),         KC_SPC,           KC_RALT, TD(LAYR), KC_RCTL, KC_MUTE
    ),

    [_FN1] = LAYOUT(
    KC_TRNS,          KC_ESC,  KC_PGUP, KC_GRV,  _______, KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_PENT, C(KC_C), KC_DEL,
    KC_TRNS,          KC_HOME, KC_PGDN, KC_END,  C(KC_F), KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PAST,          C(KC_V),
    KC_TRNS,          KC_SLSH, KC_MINS, KC_EQL,  KC_TAB,  KC_P1,   KC_P2,   KC_P3,            KC_PMNS, KC_PSLS, _______,
    KC_TRNS, _______, _______,          _______,          KC_TRNS,          KC_PDOT,          KC_TRNS, KC_TRNS, KC_TRNS, _______
    ),  
    [_FN2] = LAYOUT(
    KC_TRNS,          KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC, _______,
    KC_TRNS,          KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_DLR,  KC_PERC, KC_CIRC, KC_SCLN, KC_QUOT,          KC_BSLS,
    KC_TRNS,          KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_EXLM, KC_AT,   KC_HASH,          _______, _______, _______,
    KC_TRNS, _______, _______,          _______,          KC_TRNS,          _______,          KC_TRNS, KC_TRNS, KC_TRNS, _______
    ),  
    [_FN3] = LAYOUT(
    KC_TRNS,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    KC_TRNS,          _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_CAPS,
    KC_TRNS,          _______, _______, _______, _______, _______, _______, _______,          KC_NUM,  KC_SCRL, _______,
    KC_TRNS, _______, _______,          _______,          KC_TRNS,          _______,          KC_TRNS, KC_TRNS, KC_TRNS, _______
    ),  
};

#ifdef ENCODER_MAP_ENABLE
    const uint16_t PROGMEM encoder_map[][1][2] = {
        [_BASE] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
        [_FN1]  = { ENCODER_CCW_CW(KC_DOWN, KC_UP) },
        [_FN2]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
        [_FN3]  = { ENCODER_CCW_CW(_______, _______) }        
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

