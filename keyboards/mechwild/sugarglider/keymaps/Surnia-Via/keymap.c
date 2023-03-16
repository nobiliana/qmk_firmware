// Copyright 2023 Kyle McCreery
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _COLEDH,
	_QWERTY,
  _LOWER,
	_RAISE,
  _ADJUST,
  _MOUSE
};

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
  X_LEFT, //left layer switcher. 
  X_RIGHT, //right layer switcher.
  X_MSMID, //middle click. double tap to access media keys on left and right mouse positions, through mouse layer. 
  MS_LTR, //Left click primary (ms1), becomes right click (ms2) if x_layright is held
  MS_RTL  //right click primary (ms2), becomes left click (ms1) if x_layright is held
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case MS_LTR:
        if (record->event.pressed) {
            if (keycode == X_RIGHT){
              register_code(KC_BTN2);
            } else {
              register_code(KC_BTN1);
            }
        } else {
            // when keycode is released
        }
      break;

      case MS_RTL:
        if (record->event.pressed) {
            if (keycode == X_RIGHT){
              register_code(KC_BTN1);
            } else {
              register_code(KC_BTN2);
            }
        } else {
            // when keycode is released
        }
        break;
    }
  return true;
};

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case MS_LTR: //exsel is the last one before the modifier keys
    if (!record->event.pressed) {
      if (keycode == X_RIGHT){
        unregister_code(KC_BTN2);
      } else {
        unregister_code(KC_BTN1);
      }
    }
    break;

  case MS_RTL: //exsel is the last one before the modifier keys
    if (!record->event.pressed) {
      if (keycode == X_RIGHT){
        unregister_code(KC_BTN1);
      } else {
        unregister_code(KC_BTN2);
      }
    }
    break;
  
  }
}

tappy_dance cur_dance (qk_tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (qk_tap_dance_state_t *state, void *user_data);
void x_reset (qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEDH] = LAYOUT(                                                            
    KC_TAB,  KC_Q, KC_W,    KC_F,    KC_P,       KC_B,                          TAP_UP,        KC_J,   KC_L,     KC_U,   KC_Y, KC_SCLN, KC_BSPC, 
    KC_LCTL, KC_A, KC_R,    KC_S,    KC_T,       KC_G,             KC_MUTE,     TAP_DN,        KC_M,   KC_N,     KC_E,   KC_I, KC_O   ,  KC_ENT, 
    KC_LSFT, KC_Z, KC_X,    KC_C,    KC_D,       KC_V,                         KC_TRNS,        KC_K,   KC_H,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,    
          KC_LGUI,       KC_LALT, KC_SPC,  TD(X_LEFT), MS_LTR, TD(X_MSMID),     MS_RTL, TD(X_RIGHT), KC_SPC,  KC_RCTL,         TG(_LOWER)
  ),
  [_QWERTY] = LAYOUT(                                                            
    KC_TAB,  KC_Q, KC_W,    KC_E,    KC_R,       KC_T,                          TAP_UP,       KC_Y,   KC_U,     KC_I,   KC_O,  KC_P,    KC_BSPC, 
    KC_LCTL, KC_A, KC_S,    KC_D,    KC_F,       KC_G,             KC_MUTE,     TAP_DN,       KC_H,   KC_J,     KC_K,   KC_L, KC_QUOT,  KC_ENT, 
    KC_LSFT, KC_Z, KC_X,    KC_C,    KC_V,       KC_B,                         KC_TRNS,       KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,    
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),
  [_LOWER] = LAYOUT(      //numbers                                            
    KC_TRNS, KC_ESC,  KC_TRNS, KC_PGUP, KC_GRV,     KC_5,                   DPI_UP,           KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_PENT,  KC_DEL,         
    KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END, DPI_FINE,         KC_TRNS,  DPI_DN,           KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PAST, KC_BSLS,
    KC_TRNS, KC_INS,  KC_SLSH, KC_MINS, KC_EQL,   KC_DEL,                   KC_TRNS,          KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PSLS, KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),                                                                                                                 
  [_RAISE] = LAYOUT(      //Fs                                                                                           
    KC_TRNS, KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PERC,                   KC_TRNS,          KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_LBRC, KC_NUM,
    KC_TRNS, KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_ENT,          KC_TRNS, KC_TRNS,          KC_DLR,  KC_PERC, KC_CIRC, KC_QUOT, KC_RBRC, KC_CAPS,
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_BSPC,                   KC_TRNS,          KC_EXLM, KC_AT,   KC_HASH, KC_PSCR, KC_SCRL, KC_TRNS,
              KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),                                                                                                                 
  [_ADJUST] = LAYOUT(     //Nav                                                                                           
    KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                   KC_TRNS,          KC_PGUP, KC_HOME, KC_UP,   KC_END,  C(KC_C), KC_TRNS,
    KC_TRNS,  KC_F11,  KC_F12, KC_TRNS, KC_TRNS, KC_TRNS,         KC_TRNS,  KC_TRNS,          KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, C(KC_V), KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,                   KC_TRNS,          KC_TAB,  C(KC_F), KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),
  [_MOUSE] = LAYOUT(                                                                                                
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,         
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_WBAK,     KC_TRNS,    KC_WFWD,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][4][2] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_PGUP, KC_PGDN),  ENCODER_CCW_CW(KC_WH_U, KC_WH_D)  },
    [_LOWER] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_HOME, KC_END),   ENCODER_CCW_CW(DPI_UP, DPI_DN)    },
    [_RAISE] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_HOME, KC_END),   ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_HOME, KC_END)   },
    [_ADJUST] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS),  ENCODER_CCW_CW(KC_BRID, KC_BRIU)  },
    [_MOUSE] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS),  ENCODER_CCW_CW(KC_BRID, KC_BRIU)  }
};
#endif

void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE);
    set_auto_mouse_enable(true);
}

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
    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case SINGLE_HOLD: layer_on(_FN1); break;
    case DOUBLE_HOLD: layer_on(_FN2); break;
    case TRIPLE_HOLD: layer_on(_FN3); break;
    default: break;
  }
}

void x_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case SINGLE_HOLD: layer_off(_FN1); break;
    case DOUBLE_HOLD: layer_off(_FN2); break;
    case TRIPLE_HOLD: layer_off(_FN3); break;
    default: break;
  }
  xtap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [X_LEFT]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,x_finished, x_reset)
};