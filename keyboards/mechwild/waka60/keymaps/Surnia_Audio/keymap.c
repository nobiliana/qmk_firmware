/* Copyright 2021 Kyle McCreery
 * Copyright 2021 Surnia/Astrotia/nobiliana
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
 #include "KEY_NOTE.h"

 #include "print.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN1,
	_FN2,
	_FN3,
    //custom keycodes??
    //WAKA1 = SAFE_RNGE,
    //FFVIC,
    //RENAI,
    //BOLDLY
};


enum custom_keycodes {
    WAKA1 = SAFE_RANGE,
    FFVIC,
    RENAI,
    BOLDLY,
    WAKA2,
    PACO,
    TYPEIN,
    TYPE1,
    KN_WTOG, 
    KN_TTOG,
};



//keynote process????
extern audio_config_t audio_config;
#    ifndef NO_MUSIC_MODE
extern bool music_activated;
extern bool midi_activated;
#    endif  // !NO_MUSIC_MODE

#ifdef AUDIO_ENABLE
  #ifndef key_note_n //key note note
    #define key_note_n 440.0f
  #endif
  #ifndef key_note_du //key note duration
    #define key_note_du 5
  #endif
  #define KN_SIZE(a) ((int16_t)(sizeof(a) / (sizeof(a[0])))) //defines the array size of a, divinding total bytes by byte of one element

  float play_note = key_note_n;
  float play_du = key_note_du;
  float kn_song[][2] = SONG(LEROY_TYPE);
  float waka_key[][2] = SONG(WAKA_ONE);

  float intro_song[][2] = {{key_note_n, key_note_du}}; //intro blurb variable
  float key_note_array[][2] = {{key_note_n, key_note_du}}; //the song 
  uint16_t song_size = KN_SIZE(kn_song); //tracking full song size. needed to verify song is done
  uint16_t kn_position = 0; //setting play position within song
  bool kn_active = false; //ensuring key note is ON to continue song
  bool waka_active = false; //per key waka check

  void key_note_play (void) { //where the key_note function is reassigning arrays to play a note.
  #   ifndef NO_MUSIC_MODE
      if (music_activated || midi_activated || !audio_config.enable) return;
  #   endif //!NO_MUSIC_MODE
    if (kn_active && !waka_active){
      key_note_array[0][0] = kn_song[kn_position][0]; //setting the new note to play
      key_note_array[0][1] = kn_song[kn_position][1]; //setting the note duration to play
      if(kn_position<song_size){
        kn_position = kn_position+1; //increment array forwards by one
      } else if (kn_position>=song_size) {
        kn_position = 0;
        kn_active = false;
      }
      PLAY_SONG(key_note_array);
    } //else if (waka_active && !kn_active){}

  }

  void typewriter_song(void){
    if(!kn_active) {
      float intro_song[][2] = SONG(TYPE_INTRO);
      PLAY_SONG(intro_song);
      song_size = KN_SIZE(kn_song);
      //kn_position = 0;
      kn_active = true;
      waka_active = false; //ensure waka system is offline!
    } else if (kn_active) {
      kn_active = false;
    }
  }
  void waka_song(void){
    if(!waka_active){
      float intro_song[][2] = SONG(PACMAN_INTRO);
      PLAY_SONG(intro_song);
      waka_active = true;
      kn_active =  false; //ensure the other system is offline!
    } else if (waka_active) {
      float intro_song[][2] = SONG(PACMAN_DEAD);
      PLAY_SONG(intro_song);
      waka_active = false;
    }
  }

#endif // AUDIO_ENABLE









//static uint16_t dTempo = 120;
static float song1[][2] = SONG(WAKA_ONE);
static float song2[][2] = SONG(VICTORY_FANFARE_SHORT);
static float song3[][2] = SONG(RENAI_CIRCULATION);
static float song4[][2] = SONG(TO_BOLDLY_GO);
static float song5[][2] = SONG(WAKA_TWO);
static float song6[][2] = SONG(PACMAN_INTRO);
static float song7[][2] = SONG(TYPE_INTRO);
static float song8[][2] = SONG(LEROY_TYPE);


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WAKA1:
            if(record->event.pressed){
                //audio_set_tempo(WAKA_TEMPO);
                PLAY_SONG(song1);
                //audio_set_tempo(dTempo);
            }
            break;
        case FFVIC:
            if(record->event.pressed){
                PLAY_SONG(song2);
            }
            break;
        case RENAI:
            if(record->event.pressed){
                PLAY_SONG(song3);
            }
            break;
        case BOLDLY:
            if(record->event.pressed){
                PLAY_SONG(song4);
            }
            break;
        case WAKA2:
            if(record->event.pressed){
                PLAY_SONG(song5);
            }
            break;
        case PACO:
            if(record->event.pressed){
                PLAY_SONG(song6);
            }
            break;
        case TYPEIN:
            if(record->event.pressed){
                PLAY_SONG(song7);
            }
            break;
        case TYPE1:
            if(record->event.pressed){
                PLAY_SONG(song8);
            }
            break;
        case KN_WTOG:
            if(record->event.pressed){
                waka_song();
            }
            break;
        case KN_TTOG:
            if(record->event.pressed){
                typewriter_song();
            }
            break;
    }
/*
    #ifdef CONSOLE_ENABLE
        uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif 
*/
    if (audio_config.enable) {
      if (record->event.pressed){
        if (keycode != AU_OFF && keycode != AU_TOG && kn_active){ //makes sure audio system is on, and key_note sequence is still online
          key_note_play();
        } else if (keycode != AU_OFF && keycode != AU_TOG && waka_active){
          PLAY_SONG(waka_key);
        }
      }
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base 
    [_BASE] = LAYOUT(
    KC_GESC, WAKA1,   FFVIC,   RENAI,   BOLDLY,  WAKA2,            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_TAB,  PACO,    TYPEIN,  TYPE1,   KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
    MO(1),   KC_A,    KC_S,    KC_D,    KN_WTOG, KN_TTOG,          KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_MUTE,  KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    KC_LCTL, KC_LGUI, KC_LALT, MO(1),   KC_SPC,  KC_SPC, KC_SPC,   KC_SPC,  KC_SPC,  MO(1),   KC_RALT, KC_RGUI, KC_RCTL
    ),
    [_FN1] = LAYOUT(                                                                                   
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
    KC_TRNS, AU_ON,   RGB_TOG, MU_TOG, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, AU_OFF,  RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, MU_MOD,  RGB_RMOD,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    */
    [_BASE] = LAYOUT(
    KC_ESC,  WAKA1,   FFVIC,   RENAI,   BOLDLY,  WAKA2,            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_TAB,  PACO,    TYPEIN,  TYPE1,   KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
    MO(1),   KC_A,    KC_S,    KC_D,    KN_WTOG, KN_TTOG,          KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_MUTE,  KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    KC_LCTL, KC_LGUI, KC_LALT, MO(1),   KC_SPC,  KC_SPC, KC_SPC,   KC_SPC,  KC_SPC,  MO(1),   KC_RALT, KC_RGUI, KC_RCTL
    ),
    [_FN1] = LAYOUT(                                                                                   
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
    KC_TRNS, AU_ON,   RGB_TOG, MU_TOG, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, AU_OFF,  RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, MU_MOD,  RGB_RMOD,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_FN2] = LAYOUT(                                                                                   
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_FN3] = LAYOUT(                                                                                   
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
