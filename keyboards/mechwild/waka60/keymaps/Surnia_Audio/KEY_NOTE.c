/*
Typewriter song test. designed to play the typewriter song, per key press. 
  - check the click function for how it detects ANY key press and plays something. process_clicky in process keycode!
      function uses PLAY_SONG(), defines a custom "song". can program typewriter, pull from array?
      similar concept with pacman waka, can play song directly without needing secondary play array
        with leroy, or songs in general; will likely need a counting array to track position within music array,
          then end the song and reset the counter.

bool audio_is_playing_melody(void);

// These macros are used to allow audio_play_melody to play an array of indeterminate
// length. This works around the limitation of C's sizeof operation on pointers.
// The global float array for the song must be used here.
#define NOTE_ARRAY_SIZE(x) ((int16_t)(sizeof(x) / (sizeof(x[0]))))


 / @brief convenience macro, to play a melody/SONG once

#define PLAY_SONG(note_array) audio_play_melody(&note_array, NOTE_ARRAY_SIZE((note_array)), false)


void clicky_play(void) {
#    ifndef NO_MUSIC_MODE
    if (music_activated || midi_activated || !audio_config.enable) return;
#    endif  // !NO_MUSIC_MODE
    clicky_song[1][0] = 2.0f * clicky_freq * (1.0f + clicky_rand * (((float)rand()) / ((float)(RAND_MAX))));
    clicky_song[2][0] = clicky_freq * (1.0f + clicky_rand * (((float)rand()) / ((float)(RAND_MAX))));
    PLAY_SONG(clicky_song);
}

    if (audio_config.enable && audio_config.clicky_enable) {
        if (record->event.pressed) {                       // Leave this separate so it's easier to add upstroke sound
            if (keycode != AU_OFF && keycode != AU_TOG) {  // DO NOT PLAY if audio will be disabled, and causes issuse on ARM
                clicky_play();
            }
        }
    }
*/

#include "audio.h"
#include "KEY_NOTE.h"

__attribute__ ((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  return true;
}

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
    if (kn_active){
      key_note_array[0][0] = kn_song[kn_position][0]; //setting the new note to play
      key_note_array[0][1] = kn_song[kn_position][1]; //setting the note duration to play
      if(kn_position<song_size){
        kn_position = kn_position+1; //increment array forwards by one
      } else {
        kn_position = 0;
        kn_active = false;
      }
      PLAY_SONG(key_note_array);
    } else if (waka_active){
      float key_note_array[][2] = SONG(WAKA_ONE);
      PLAY_SONG(key_note_array);
    }

  }

  void typewriter_song(void){
    if(!kn_active) {
      float intro_song[][2] = SONG(TYPE_INTRO);
      PLAY_SONG(intro_song);
      song_size = KN_SIZE(kn_song);
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

  bool process_key_note(uint16_t keycode, keyrecord_t *record) { //key detection?
/*    if (keycode == KN_TTOG && record->event.pressed) {
      typewriter_song(); //typewriter process
    }
    if (keycode == KN_WTOG && record->event.pressed){
      waka_song(); //waka process
    } */
    if (audio_config.enable) {
      if (record->event.pressed){
        if (keycode != AU_OFF && keycode != AU_TOG && kn_active){ //makes sure audio system is on, and key_note sequence is still online
          key_note_play();
        }
      }
    }
    return true;
  }

#endif // AUDIO_ENABLE