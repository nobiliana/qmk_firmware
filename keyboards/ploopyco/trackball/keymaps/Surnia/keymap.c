/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2023 Surnia (aka. nobiliana)
 * Copyright 2020 Ploopy Corporation
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
  X_LAYERS
};

tappy_dance cur_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (tap_dance_state_t *state, void *user_data);
void x_reset (tap_dance_state_t *state, void *user_data);


/*
Button Mapping:
Lower Thumb, Scroll click, Upper thumb,
  Middle right, far right.

  Default functionality:
  Button 4 = back
  Button 5 = foward
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN4,
          KC_BTN2, TD(X_LAYERS)
    ),
    [1] = LAYOUT(
        _______, _______, _______,
          DPI_FINE, KC_TRNS
    ),
    [2] = LAYOUT(
      DRAG_SCROLL, _______, _______,
        _______, KC_TRNS
    ),
    [3] = LAYOUT(
      _______, _______, _______,
        _______, KC_TRNS
    )
};
tappy_dance cur_dance (tap_dance_state_t *state) {
    switch (state->count) {
    case 1:
      if (!state->pressed)  return SINGLE_TAP;
      //only if key is released and NOT interrupted, but they key is still held. Means you want to send a 'HOLD'. Interrupt should lead to HOLD.
      else return SINGLE_HOLD;
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
  */
}


//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = TD_NONE
};

void x_finished (tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(KC_BTN5); break;
//    case DOUBLE_TAP: register_code(KC_G); break;
    case SINGLE_HOLD: layer_on(1); break;
    case DOUBLE_HOLD: layer_on(2); break;
    case TRIPLE_HOLD: layer_on(3); break;
    default: break;
  }
}

void x_reset (tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(KC_BTN5); break;
//    case DOUBLE_TAP: unregister_code(KC_G); break;
    case SINGLE_HOLD: layer_off(1); break;
    case DOUBLE_HOLD: layer_off(2); break;
    case TRIPLE_HOLD: layer_off(3); break;
    default: break;
  }
  xtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
  [X_LAYERS]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,x_finished, x_reset)
};