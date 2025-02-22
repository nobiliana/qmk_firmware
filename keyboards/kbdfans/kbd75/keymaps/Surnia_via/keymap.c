#include QMK_KEYBOARD_H

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  SINGLE_HOLD,
  DOUBLE_HOLD,
  TRIPLE_HOLD,
  SINGLE_TAP
//  DOUBLE_TAP
} tappy_dance;

typedef struct {
  bool is_press_action;
  tappy_dance state;
} tap;

enum {
  X_LAY
};

tappy_dance cur_dance (qk_tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (qk_tap_dance_state_t *state, void *user_data);
void x_reset (qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT(
    KC_ESC,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  MO(1),    KC_DEL,
    KC_GRV,    KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_NO,    KC_BSPC,  KC_HOME,
    KC_TAB,    KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGUP,
    KC_LCTL,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                      KC_ENT,   KC_PGDN,
    KC_LSFT,   KC_NO,    KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_END,
    TD(X_LAY), KC_LGUI,  KC_LALT,                      KC_SPC,   KC_SPC,   KC_SPC,                       KC_RALT,  TT(1),    KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [1] = LAYOUT(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  KC_ACL0,  KC_ACL2,  KC_ACL2,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_NO,    _______,  _______,
    KC_TRNS,  KC_WH_L,  KC_MS_U,  KC_WH_R,  KC_WH_U,  _______,  KC_P7,    KC_P8,    KC_P9,    KC_P0,    KC_PENT,  C(KC_C),  C(KC_V),  _______,            _______,
    KC_TRNS,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_WH_D,  C(KC_F),  KC_P4,    KC_P5,    KC_P6,    KC_PPLS,  KC_PAST,  _______,                      _______,  _______,
    KC_TRNS,  KC_NO,    KC_BTN2,  KC_BTN3,  KC_BTN1,  _______,  KC_TAB,   KC_P1,    KC_P2,    KC_P3,    KC_PMNS,  KC_PSLS,  KC_TRNS,            _______,  _______,
    KC_TRNS,  _______,  _______,                      _______,  _______,  _______,                      _______,  KC_TRNS,  _______,  _______,  _______,  _______
  ),

  [2] = LAYOUT(
    RGB_TOG,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_NO,    _______,  _______,
    KC_TRNS,  RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    KC_TRNS,  RGB_M_TW, RGB_M_B,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,
    KC_TRNS,  KC_NO,    _______,  _______,  BL_DEC,   BL_TOGG,  BL_INC,   BL_STEP,  _______,  _______,  _______,  _______,  KC_TRNS,            _______,  _______,
    KC_TRNS,  _______,  _______,                      _______,  _______,  _______,                      _______,  KC_TRNS,  _______,  KC_NUM,   KC_CAPS,  KC_SCRL
  ),

  [3] = LAYOUT(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_NO,    RESET,    _______,
    _______,  RGB_TOG,  RGB_MOD,  RGB_HUI,  RGB_HUD,  RGB_SAI,  RGB_SAD,  RGB_VAI,  RGB_VAD,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,
    _______,  KC_NO,    _______,  _______,  BL_DEC,   BL_TOGG,  BL_INC,   BL_STEP,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    KC_TRNS,  _______,  _______,                      _______,  _______,  _______,                      _______,  KC_TRNS,  _______,  _______,  _______,  _______
  ),
};

tappy_dance cur_dance (qk_tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
        return SINGLE_HOLD;
    case 2:
        return DOUBLE_HOLD;
    case 3:
        return TRIPLE_HOLD;
    default:
        return TD_UNKNOWN;
  }
  /*
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  if (state->count == 2) {
    if (state->interrupted || !state->pressed)  return DOUBLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return DOUBLE_HOLD;
  }
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TD_UNKNOWN;
    else return TRIPLE_HOLD;
  }
  else return TD_UNKNOWN; //magic number. At some point this method will expand to work for more presses
  */
}
//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = TD_NONE
};

void x_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_HOLD: layer_on(1); break;
    case DOUBLE_HOLD: layer_on(2); break;
    case TRIPLE_HOLD: layer_on(3); break;
    default: break;
  }
}

void x_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_HOLD: layer_off(1); break;
    case DOUBLE_HOLD: layer_off(2); break;
    case TRIPLE_HOLD: layer_off(3); break;
    default: break;
  }
  xtap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [X_LAY]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,x_finished, x_reset)
};