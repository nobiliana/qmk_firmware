/* Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
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

extern bool is_drag_scroll;

enum keyboard_keycodes {
  DPI_FINE
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch(keycode) {
        case DPI_FINE:
            if (record->event.pressed) {
                pointing_device_set_cpi(dpi_array[0]);
            } else {
                pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
            }
            break;
    }
    return true;
};


enum td_keycodes {
  LAYL, //left layer switcher. 
  LAYR, //right layer switcher.
};

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  L1_HOLD,
  L2_HOLD,
//  L3_HOLD,
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

tappy_dance LAYL_dance (tap_dance_state_t *state);
tappy_dance LAYR_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void LL_finished (tap_dance_state_t *state, void *user_data);
void LL_reset (tap_dance_state_t *state, void *user_data);

void LR_finished (tap_dance_state_t *state, void *user_data);
void LR_reset (tap_dance_state_t *state, void *user_data);


/*
Button Mapping:
Lower Thumb, Scroll click, Upper thumb,
  Middle right, far right.

  Default functionality:
  Button 1 = Left
  Button 2 = Right
  Button 3 = Middle
  Button 4 = back
  Button 5 = foward
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_ENT,   KC_BTN4, TD(LAYR), TD(LAYL), 
                  KC_BTN1,                     KC_BTN2 ),

    [1] = LAYOUT( C(KC_C),  KC_BTN5, KC_TRNS,  KC_TRNS, 
                  C(KC_V),                     _______ ),

    [2] = LAYOUT( _______,  _______, KC_TRNS,  KC_TRNS, 
                  _______,                     _______ ),

    [3] = LAYOUT( _______,  _______, KC_TRNS,  KC_TRNS, 
                  _______,                     _______ )
};

tappy_dance LAYL_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
      if (!state->pressed)  return SINGLE_TAP;
      //only if key is released and NOT interrupted, but they key is still held. Means you want to send a 'HOLD'. Interrupt should lead to HOLD.
      else return L1_HOLD;
    case 2:
        return L2_HOLD;
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

static tap ltap_state = {
  .is_press_action = true,
  .state = TD_NONE
};
void LL_finished (tap_dance_state_t *state, void *user_data) {
  ltap_state.state = LAYL_dance(state);
  switch (ltap_state.state) {
    case SINGLE_TAP: register_code(KC_BTN3); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case L1_HOLD: is_drag_scroll    = true; break;
    case L2_HOLD: pmw33xx_set_cpi(0, (dpi_array[keyboard_config.dpi_config] / 2) ); break;
    default: break;
  }
}
void LL_reset (tap_dance_state_t *state, void *user_data) {
  switch (ltap_state.state) {
    case SINGLE_TAP: unregister_code(KC_BTN3); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case L1_HOLD: is_drag_scroll    = false; break;
    case L2_HOLD:  pmw33xx_set_cpi(0, dpi_array[keyboard_config.dpi_config] ); break;
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