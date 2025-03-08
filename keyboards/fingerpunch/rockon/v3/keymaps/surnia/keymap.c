#include QMK_KEYBOARD_H


// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QWERTY,
    _COLEMAK,
    _LOWER, //numpad
    _RAISE, //F keys
    _ADJUST //extras?
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    DRAG_SCROLL,
    MS_LTR, //Left click primary (ms1), becomes right click (ms2) if x_layright is held
    MS_RTL,  //right click primary (ms2), becomes left click (ms1) if x_layright is held
    RGB_STR,    //starlight dual hue macro
    RGB_SPA,    //splash 
    RGB_REA     //solid reactive
};

enum td_keycodes {
    LAYL, //left layer switcher. 
    LAYR, //right layer switcher.
    MSMD, //mid click switcher for dragscroll and precision
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
    M1_TAP,
    M1_HOLD,
    M2_HOLD
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

  void MS_finished (tap_dance_state_t *state, void *user_data);
  void MS_reset (tap_dance_state_t *state, void *user_data);


bool set_scrolling = false;

// Modify these values to adjust the scrolling speed
#define SCROLL_DIVISOR_H 60.0
#define SCROLL_DIVISOR_V 4.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Function to handle mouse reports and perform drag scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // Check if drag scrolling is active
    if (set_scrolling) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v -= (float)mouse_report.y / SCROLL_DIVISOR_V; //negative to invert scroll.

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = (int8_t)scroll_accumulated_h;
        mouse_report.v = (int8_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int8_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

// All the KC_MUTE keycodes below represent the encoders. If you aren't using encoders, you can ignore these:
//            -->  KC_MUTE,   KC_LCTL,   KC_LGUI,   LOWER,     KC_SPC,    KC_LALT,   KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3,    KC_RALT,   KC_SPC,    RAISE,     KC_RGUI,   KC_RCTL,   KC_MUTE, <--
//                                                                                            --> KC_MUTE <--
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_rockon(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LBRC,                                          KC_RBRC,   KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_DEL,
    KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_MINS,                                          KC_EQL,    KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,      KC_BSPC,
    KC_LCTL,   KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_GRV,                                           KC_QUOT,   KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,   KC_ENT,
    KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_HOME,                                          KC_END,    KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,   KC_RSFT,
               KC_A,      KC_NO,     KC_NO,    TD(ALTGUI), KC_SPC,    TD(LAYL),  MS_LTR,     TD(MSMD),   MS_RTL,        TD(LAYR),  KC_SPC,    KC_RCTL,   KC_NO,     KC_NO,     KC_A,
                                                                                              KC_MUTE
),


[_COLEMAK] = LAYOUT_rockon(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LBRC,                                          KC_RBRC,   KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_DEL,
    KC_TAB,    KC_Q,      KC_W,      KC_F,      KC_P,      KC_B,      KC_MINS,                                          KC_EQL,    KC_J,      KC_L,      KC_U,      KC_Y,      KC_SCLN,   KC_BSPC,
    KC_LCTL,   KC_A,      KC_R,      KC_S,      KC_T,      KC_G,      KC_GRV,                                           KC_QUOT,   KC_M,      KC_N,      KC_E,      KC_I,      KC_O,      KC_ENT,
    KC_LSFT,   KC_Z,      KC_X,      KC_C,      KC_D,      KC_V,      KC_HOME,                                          KC_END,    KC_K,      KC_H,      KC_COMM,   KC_DOT,    KC_SLSH,   KC_RSFT,
               KC_A,      KC_NO,     KC_NO,    TD(ALTGUI), KC_SPC,    TD(LAYL),  MS_LTR,     TD(MSMD),   MS_RTL,        TD(LAYR),  KC_SPC,    KC_RCTL,   KC_NO,     KC_NO,     KC_A,
                                                                                              KC_MUTE
),


[_LOWER] = LAYOUT_rockon(
    KC_TRNS,   _______,   _______,   _______,   _______,   _______,   _______,                                          _______,   _______,   _______,   _______,   _______,   _______,   KC_TRNS,
    KC_TRNS,   KC_GRV,    KC_HOME,   KC_UP,     KC_END,    KC_PGUP,   _______,                                          C(KC_C),   KC_P7,     KC_P8,     KC_P9,     KC_P0,     KC_PENT,   KC_DEL,
    KC_TRNS,   _______,   KC_LEFT,   KC_DOWN,   KC_RGHT,   KC_PGDN,   _______,                                          C(KC_V),   KC_P4,     KC_P5,     KC_P6,     KC_PPLS,   KC_PAST,   KC_BSLS,
    KC_TRNS,   _______,   KC_SLSH,   KC_MINS,   KC_EQL,    KC_DEL,    _______,                                          C(KC_F),   KC_P1,     KC_P2,     KC_P3,     KC_PMNS,   KC_PSLS,   KC_TRNS,
               _______,   _______,   _______,   _______,   _______,   KC_TRNS,   _______,    KC_TRNS,    _______,       KC_TRNS,   KC_PDOT,   _______,   _______,   _______,   _______,
                                                                                             _______
),

[_RAISE] = LAYOUT_rockon(
    KC_TRNS,   _______,   _______,   _______,   _______,   _______,   _______,                                          _______,   _______,   _______,   _______,   _______,   _______,   KC_SCRL,
    KC_TRNS,   KC_F9,     KC_F10,    KC_F11,    KC_F12,    _______,   _______,                                          _______,   KC_AMPR,   KC_ASTR,   KC_LPRN,   KC_RPRN,   KC_LBRC,   KC_NUM,
    KC_TRNS,   KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_ENT,    _______,                                          _______,   KC_DLR,    KC_PERC,   KC_CIRC,   KC_QUOT,   KC_RBRC,   KC_CAPS,
    KC_TRNS,   KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_BSPC,   _______,                                          _______,   KC_EXLM,   KC_AT,     KC_HASH,   KC_PSCR,   KC_INS,    KC_TRNS,
               _______,   _______,   _______,   _______,   _______,   KC_TRNS,   _______,    KC_TRNS,    _______,       KC_TRNS,   _______,   _______,   _______,   _______,   _______,
                                                                                             _______
),

[_ADJUST] =  LAYOUT_rockon( //LED left, nav right?
    KC_TRNS,   RM_HUEU,   RM_SATU,   RM_VALU,   RM_SPDU,   RM_NEXT,   DF(_QWERTY),                                      _______,   _______,   _______,   _______,   _______,   _______,   KC_TRNS,
    KC_TRNS,   RM_HUED,   RM_SATD,   RM_VALD,   RM_SPDD,   RM_PREV,   DF(_COLEMAK),                                     _______,   _______,   _______,   _______,   _______,   _______,   KC_TRNS,
    KC_TRNS,   _______,   _______,   _______,   _______,   _______,   _______,                                          _______,   _______,   _______,   _______,   _______,   _______,   KC_TRNS,
    KC_TRNS,   RM_TOGG,   _______,   RGB_STR,   RGB_SPA,   RGB_REA,   _______,                                          _______,   _______,   _______,   _______,   _______,   _______,   KC_TRNS,
               _______,   _______,   _______,   _______,   _______,   KC_TRNS,   _______,    KC_TRNS,    _______,       KC_TRNS,   _______,   _______,   _______,   _______,   _______,
                                                                                             _______
)
};

/*#ifdef ENCODER_MAP_ENABLE
//Left encoder, middle encoder, right encoder, mouse bottom encoder.
const uint16_t PROGMEM encoder_map[][2][2] = {
    [0] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN)},
    [1] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN)},
    [2] =  { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN)},
    [3] =  { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN)},
    [4] =  { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN)},
};
#endif
*/


  bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
          tap_code(KC_RIGHT);
        } else {
          tap_code(KC_LEFT);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
          tap_code(KC_DOWN);
        } else {
          tap_code(KC_UP);
        }
    }
    return false;
  }

  
tappy_dance MSMD_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
      if (!state->pressed)  return M1_TAP;
      //only if key is released and NOT interrupted, but they key is still held. Means you want to send a 'HOLD'. Interrupt should lead to HOLD.
      else return M1_HOLD;
    case 2:
        return M2_HOLD;
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
void MS_finished (tap_dance_state_t *state, void *user_data) {
  mtap_state.state = MSMD_dance(state);
  switch (mtap_state.state) {
    case M1_TAP: register_code(KC_BTN3); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case M1_HOLD: set_scrolling    = true; break;
    case M2_HOLD: pmw33xx_set_cpi(0, (dpi_array[keyboard_config.dpi_config] / 2)); break;
    default: break;
  }
}
void MS_reset (tap_dance_state_t *state, void *user_data) {
  switch (mtap_state.state) {
    case M1_TAP: unregister_code(KC_BTN3); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case M1_HOLD: set_scrolling    = false; break;
    case M2_HOLD:  pmw33xx_set_cpi(0, (dpi_array[keyboard_config.dpi_config])); break;
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
      case L1_HOLD: layer_on(_RAISE); break;
      case L2_HOLD: layer_on(_LOWER); break;
      case L3_HOLD: layer_on(_ADJUST); break;
      default: break;
    }
  }
  void LL_reset (tap_dance_state_t *state, void *user_data) {
    switch (ltap_state.state) {
  //    case SINGLE_TAP: unregister_code(KC_SPC); break;
  //    case DOUBLE_TAP: unregister_code(KC_G); break;
      case L1_HOLD: layer_off(_RAISE); break;
      case L2_HOLD: layer_off(_LOWER); break;
      case L3_HOLD: layer_off(_ADJUST); break;
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
    case R1_HOLD: layer_on(_LOWER); break;
    case R2_HOLD: layer_on(_RAISE); break;
    case R3_HOLD: layer_on(_ADJUST); break;
    default: break;
  }
}
void LR_reset (tap_dance_state_t *state, void *user_data) {
  switch (rtap_state.state) {
//    case SINGLE_TAP: unregister_code(KC_SPC); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case R1_HOLD: layer_off(_LOWER); break;
    case R2_HOLD: layer_off(_RAISE); break;
    case R3_HOLD: layer_off(_ADJUST); break;
    default: break;
  }
  rtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
  [ALTGUI]   = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_LGUI), 
  [LAYL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LL_finished, LL_reset),
  [LAYR]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,LR_finished, LR_reset),
  [MSMD]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,MS_finished, MS_reset)
};

//macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case DRAG_SCROLL:
      // Toggle set_scrolling when DRAG_SCROLL key is pressed or released
        set_scrolling = record->event.pressed;
      break;

      case MS_LTR: //swaps right click for left click if right layer switch held
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

      case MS_RTL: //swaps left click for right click if right layer switch held
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
      case RGB_STR:
        if (record->event.pressed) {
            register_code(RGB_MATRIX_STARLIGHT_DUAL_HUE);
        }
        break;
      case RGB_SPA:
        if (record->event.pressed) {
            register_code(RGB_MATRIX_SPLASH);
        }
        break;
      case RGB_REA:
        if (record->event.pressed) {
            register_code(RGB_MATRIX_SOLID_REACTIVE);
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
    case RGB_STR:
    if (!record->event.pressed) {
        unregister_code(RGB_MATRIX_STARLIGHT_DUAL_HUE);
    }
    break;
  case RGB_SPA:
    if (!record->event.pressed) {
        unregister_code(RGB_MATRIX_SPLASH);
    }
    break;
  case RGB_REA:
    if (!record->event.pressed) {
        unregister_code(RGB_MATRIX_SOLID_REACTIVE);
    }
    break;

  }
};
/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
        default:
            break;
    }

    return true;
}
    */
