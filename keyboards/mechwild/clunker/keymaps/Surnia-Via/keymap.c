// Copyright 2022 Kyle McCreery (@kylemccreery)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum {
  X_LAYERS
};

typedef enum {
  TD_NONE,
  SINGLE_HOLD,
  DOUBLE_HOLD,
  TRIPLE_HOLD,
  QUAD_HOLD
} tappy_dance;

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _BASE,
  _FN1,
  _FN2,
  _FN3
};

typedef struct {
  bool is_press_action;
  tappy_dance state;
} tap;

tappy_dance cur_dance (qk_tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (qk_tap_dance_state_t *state, void *user_data);
void x_reset (qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    /* 
    k00, k01, k02, k03, k04, k05, k06, k07,                     \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A,      \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, \
    k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, \
    k50, k51, k52,      k54,      k56,           k59, k5A, k5B  \
    */
    [_BASE] = LAYOUT(
    KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_MUTE,
    KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSPC,
    KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,              KC_ENT,
    KC_LSFT,  KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_RSFT,
    MO(_FN3), KC_LGUI, KC_LALT,          X_LAYERS,         KC_SPC,                    KC_RALT, MO(_FN2), KC_RCTL
    ),
    [_FN1] = LAYOUT(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,   HPT_FBK, HPT_TOG,
    _______, KC_ESC,  KC_PGUP, KC_GRV,  KC_4,     KC_P7,   KC_P8,   KC_P9,   KC_P0,   C(KC_C), C(KC_V),  KC_DEL,
    _______, KC_HOME, KC_PGDN, KC_END,  C(KC_F),  KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PAST,           KC_BSLS,
    _______, KC_BSPC, KC_SLSH, KC_MINS, KC_EQL,   KC_TAB,  KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PSLS,  _______,
    _______, _______, _______,          _______,           KC_PDOT,                   _______, _______,  _______
    ),
    [_FN2] = LAYOUT(
    _______, _______, _______, _______, _______,  _______, _______, _______,
    _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC,  KC_PSCR,
    _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_DLR,  KC_PERC, KC_CIRC, KC_SCLN, KC_QUOT,           KC_CAPS,
    _______, KC_DEL,  KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_EXLM, KC_AT,   KC_HASH, _______, _______,  KC_SCRL,
    _______, _______, _______,          _______,           KC_ENT,                    KC_INS,  _______,  KC_NUM
    ),
	[_FN3] = LAYOUT(
    _______, _______, _______, _______, _______,  _______, _______, _______,
    _______, _______, _______, _______, _______,  _______, KC_PGUP, KC_HOME, KC_UP,   KC_END,  _______,  _______,
    _______, _______, _______, _______, _______,  _______, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT,           _______,
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______,  _______,
    _______, _______, _______,          _______,           KC_ENT,                    _______, _______,  _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][1][2] = {
    [0] =  { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] =  { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)  },
    [2] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN)  },
    [3] =  { ENCODER_CCW_CW(KC_BRID, KC_BRIU) }
};
#endif

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
tappy_dance cur_dance (qk_tap_dance_state_t *state) {
  switch (state->count) {
    case 1:
        return SINGLE_HOLD;
    case 2:
        return DOUBLE_HOLD;
    case 3:
        return TRIPLE_HOLD;
    case 4:
        return QUAD_HOLD;
    default:
        return TD_NONE;
  }
  /*
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return 8;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  if (state->count == 2) {
    if (state->interrupted || !state->pressed)  return 8;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return DOUBLE_HOLD;
  }
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return 8;
    else return TRIPLE_HOLD;
  }
  if (state->count == 4) {
    if (state->interrupted || !state->pressed)  return 8;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return QUAD_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
  */
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void x_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_HOLD: layer_on(_BASE); break;
    case DOUBLE_HOLD: layer_on(_FN1); break;
    case TRIPLE_HOLD: layer_on(_FN2); break;
    case QUAD_HOLD: layer_on(_FN3); break;
    default: break;
  }
}

void x_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_HOLD: layer_off(_BASE); break;
    case DOUBLE_HOLD: layer_off(_FN1); break;
    case TRIPLE_HOLD: layer_off(_FN2); break;
    case QUAD_HOLD: layer_off(_FN3); break;
    default: break;
  }
  xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [X_LAYERS]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,x_finished, x_reset)
};