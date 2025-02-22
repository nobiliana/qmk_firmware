// Copyright 2023 Kyle McCreery
// Copyright 2023 nobiliana aka. Surnia
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// kinetic variables
extern int16_t xVal, yVal;
extern int8_t LIFTOFF; 
extern int32_t frictionMultiplier;
extern void kineticCirque(report_mouse_t *mouse_report);

#include "drivers/sensors/cirque_pinnacle.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _COLEDH,
	_QWERTY,
  _NUMS,
	_FKEYS,
  _NAV,
  _BROWSER
};

enum td_keycodes {
  LAYL, //left layer switcher. 
  LAYR, //right layer switcher.
  MSMD, //middle click. double tap to access media keys on left and right mouse positions, through mouse layer. 
  ALTGUI //alt or GUI. basic TD, no advanced function needed... hopefully. 
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
  MSM1_ACTN,
  MSM2_HOLD,
  SINGLE_TAP
//  DOUBLE_TAP
} tappy_dance;

typedef struct {
  bool is_press_action;
  tappy_dance state;
} tap;

tappy_dance LAYL_dance (tap_dance_state_t *state);
tappy_dance LAYR_dance (tap_dance_state_t *state);
tappy_dance MSMD_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void LL_finished (tap_dance_state_t *state, void *user_data);
void LL_reset (tap_dance_state_t *state, void *user_data);

void LR_finished (tap_dance_state_t *state, void *user_data);
void LR_reset (tap_dance_state_t *state, void *user_data);

void MSM_finished (tap_dance_state_t *state, void *user_data);
void MSM_reset (tap_dance_state_t *state, void *user_data);


enum custom_keycodes {
  MS_LTR, //Left click primary (ms1), becomes right click (ms2) if x_layright is held
  MS_RTL,  //right click primary (ms2), becomes left click (ms1) if x_layright is held
  kineticBrake, //kinetic functions
  kineticGlide //kinetic functions
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LAYL:
            return 165;
        case LAYR:
            return 165;
        default:
            return g_tapping_term;
    }
};
/*
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
        pinnacle_data_t cirqueData = cirque_pinnacle_read_data();
//        uprintf("x: %i, y: %i, z: %i\n", cirqueData.xValue, cirqueData.yValue, cirqueData.zValue);
        cirque_pinnacle_scale_data(&cirqueData, cirque_pinnacle_get_scale(), cirque_pinnacle_get_scale());
//        uprintf("x: %i, y: %i, z: %i\n", cirqueData.xValue, cirqueData.yValue, cirqueData.zValue);

    //KINETIC MOTION

            if (cirqueData.zValue){//records last mouse input prior to liftoff. 
                if (cirqueData.xValue !=0 || cirqueData.yValue !=0){
                    xVal = cirqueData.xValue;
                    yVal = cirqueData.yValue;
                }
                LIFTOFF = 0;
                //uprintf("xVal: %i, yVal: %i, Liftoff: %i || mouse x: %i, mouse: y %i\n", xVal, yVal, LIFTOFF, mouse_report.x, mouse_report.y);
            } else if (!cirqueData.zValue) {
                LIFTOFF = 1;
                //uprintf("lift detected!: %i, xVal: %i, yVal %i\n", LIFTOFF, xVal, yVal);
            }
        kineticCirque(&mouse_report);

    //RADIAL KEYS


    return mouse_report;
};

*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEDH] = LAYOUT(                                                            
    KC_TAB,  KC_Q, KC_W,    KC_F,    KC_P,       KC_B,                          TAP_UP,        KC_J,   KC_L,     KC_U,   KC_Y, KC_SCLN, KC_BSPC, 
    KC_LCTL, KC_A, KC_R,    KC_S,    KC_T,       KC_G,             KC_MUTE,     TAP_DN,        KC_M,   KC_N,     KC_E,   KC_I, KC_O   ,  KC_ENT, 
    KC_LSFT, KC_Z, KC_X,    KC_C,    KC_D,       KC_V,                         KC_TRNS,        KC_K,   KC_H,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,    
          KC_LGUI,   TD(ALTGUI), KC_SPC,     TD(LAYL),   MS_LTR, TD(MSMD), MS_RTL,        TD(LAYR), KC_SPC,  KC_RCTL,         TG(_NUMS)
  ),
  [_QWERTY] = LAYOUT(                                                            
    KC_TAB,  KC_Q, KC_W,    KC_E,    KC_R,       KC_T,                          TAP_UP,       KC_Y,   KC_U,     KC_I,   KC_O,  KC_P,    KC_BSPC, 
    KC_LCTL, KC_A, KC_S,    KC_D,    KC_F,       KC_G,             KC_MUTE,     TAP_DN,       KC_H,   KC_J,     KC_K,   KC_L, KC_QUOT,  KC_ENT, 
    KC_LSFT, KC_Z, KC_X,    KC_C,    KC_V,       KC_B,                         KC_TRNS,       KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,    
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),
  [_NUMS] = LAYOUT(      //numbers                                            
    KC_TRNS, KC_ESC,  KC_TRNS, KC_PGUP, KC_GRV, kineticBrake,               DPI_UP,           KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_PENT,  KC_DEL,         
    KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END, DPI_FINE,         KC_TRNS,  DPI_DN,           KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PAST, KC_BSLS,
    KC_TRNS, KC_INS,  KC_SLSH, KC_MINS, KC_EQL,   KC_DEL,                   KC_TRNS,          KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PSLS, KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_PDOT, KC_TRNS,          KC_TRNS       
  ),                                                                                                                 
  [_FKEYS] = LAYOUT(      //Fs                                                                                           
    KC_TRNS, KC_F9,   KC_F10,  KC_F11,  KC_F12,  kineticGlide,              KC_TRNS,          KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_LBRC, KC_NUM,
    KC_TRNS, KC_F5,   KC_F6,   KC_F7,   KC_F8,    KC_ENT,          KC_TRNS, KC_TRNS,          KC_DLR,  KC_PERC, KC_CIRC, KC_QUOT, KC_RBRC, KC_CAPS,
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_BSPC,                   KC_TRNS,          KC_EXLM, KC_AT,   KC_HASH, KC_PSCR, KC_SCRL, KC_TRNS,
              KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),                                                                                                                 
  [_NAV] = LAYOUT(     //Nav                                                                                           
    KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                   KC_TRNS,          KC_PGUP, KC_HOME, KC_UP,   KC_END,  C(KC_C), KC_TRNS,
    KC_TRNS,  KC_F11,  KC_F12, KC_TRNS, KC_TRNS, KC_TRNS,         KC_TRNS,  KC_TRNS,          KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, C(KC_V), KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,          KC_TAB,  C(KC_F), KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  ),
  [_BROWSER] = LAYOUT(                                                                                                
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,         
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS,    KC_TRNS, KC_TRNS,    KC_TRNS, KC_WBAK,     KC_TRNS,    KC_WFWD,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS       
  )
};

#if defined(ENCODER_MAP_ENABLE)
//Left encoder, middle encoder, right encoder, mouse bottom encoder.
const uint16_t PROGMEM encoder_map[][4][2] = {
    [_COLEDH] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_PGUP, KC_PGDN),  ENCODER_CCW_CW(KC_UP, KC_DOWN),   ENCODER_CCW_CW(KC_WH_U, KC_WH_D)  },
    [_QWERTY] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_U, KC_WH_D)  },
    [_NUMS] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(DPI_UP, DPI_DN)    },
    [_FKEYS] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_HOME, KC_END),   ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_HOME, KC_END)   },
    [_NAV] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU)  },
    [_BROWSER] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_BRID, KC_BRIU)  }
};
#endif

/*  auto enables mouse layer. 
void pointing_device_init_user(void) {
    set_auto_BROWSER_layer(_BROWSER);
    set_auto_BROWSER_enable(true);
}
*/

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

tappy_dance MSMD_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
        return MSM1_ACTN;
    case 2:
        return MSM2_HOLD;
    default:
        return TD_UNKNOWN;
  }
};

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
  else return TD_UNKNOWN; //magic number. At some point this method will expand to work for more presses
  */
};

static tap mtap_state = {
  .is_press_action = true,
  .state = TD_NONE
};
void MSM_finished (tap_dance_state_t *state, void *user_data) {
  mtap_state.state = MSMD_dance(state);
  switch (mtap_state.state) {
//    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case MSM1_ACTN: register_code(KC_BTN3); break;
    case MSM2_HOLD: layer_on(_BROWSER); break;
    default: break;
  }
}
void MSM_reset (tap_dance_state_t *state, void *user_data) {
  switch (mtap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case MSM1_ACTN: unregister_code(KC_BTN3); break;
    case MSM2_HOLD: layer_off(_BROWSER); break;
    default: break;
  }
  mtap_state.state = TD_NONE;
}

static tap ltap_state = {
  .is_press_action = true,
  .state = TD_NONE
};
void LL_finished (tap_dance_state_t *state, void *user_data) {
  ltap_state.state = LAYL_dance(state);
  switch (ltap_state.state) {
//    case SINGLE_TAP: register_code(KC_SPC); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case L1_HOLD: layer_on(_FKEYS); break;
    case L2_HOLD: layer_on(_NUMS); break;
    case L3_HOLD: layer_on(_NAV); break;
    default: break;
  }
}
void LL_reset (tap_dance_state_t *state, void *user_data) {
  switch (ltap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case L1_HOLD: layer_off(_FKEYS); break;
    case L2_HOLD: layer_off(_NUMS); break;
    case L3_HOLD: layer_off(_NAV); break;
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
    case R1_HOLD: layer_on(_NUMS); break;
    case R2_HOLD: layer_on(_NAV); break;
    case R3_HOLD: layer_on(_FKEYS); break;
    default: break;
  }
}
void LR_reset (tap_dance_state_t *state, void *user_data) {
  switch (rtap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case R1_HOLD: layer_off(_NUMS); break;
    case R2_HOLD: layer_off(_NAV); break;
    case R3_HOLD: layer_off(_FKEYS); break;
    default: break;
  }
  rtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
  [ALTGUI]   = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_LGUI),
  [MSMD]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,MSM_finished, MSM_reset),
  [LAYL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LL_finished, LL_reset),
  [LAYR]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LR_finished, LR_reset)
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case MS_LTR:
        if (record->event.pressed) {
            if (rtap_state.state != TD_NONE){
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
            if (rtap_state.state != TD_NONE){
              register_code(KC_BTN1);
            } else {
              register_code(KC_BTN2);
            }
        } else {
            // when keycode is released
        }
        break;

      case kineticBrake:
          if (record->event.pressed) {
              frictionMultiplier = 500;
          } else {
              frictionMultiplier = 100;
          }
        break;
      case kineticGlide:
          if (record->event.pressed) {
              frictionMultiplier = 5;
          } else {
              frictionMultiplier = 100;
          }
        break;
    }
  return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case MS_LTR: //exsel is the last one before the modifier keys
    if (!record->event.pressed) {
        unregister_code(KC_BTN2);
        unregister_code(KC_BTN1);
    }
    break;

  case MS_RTL: //exsel is the last one before the modifier keys
    if (!record->event.pressed) {
        unregister_code(KC_BTN1);
        unregister_code(KC_BTN2);
    }
    break;
  
  }
};