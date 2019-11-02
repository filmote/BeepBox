//#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Arduboy2Ext.h"

#include <ArduboyTones.h>
#include <EEPROM.h>
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Font3x5 font3x5;

uint16_t noteLength = 400;
uint16_t numberOfNotes = 200;
uint8_t menuIndex = 0;

struct Note {
  uint16_t freq = 0;
  uint16_t duration = noteLength;
  uint8_t getDurationInterval() { return (duration / noteLength); }
  uint8_t getDurationIndex() { return (duration / noteLength) - 1; }
};

Note notes[200] = {};

uint8_t noteX = 0;
uint16_t noteY = 0;
uint16_t maxNoteX = 1;
uint8_t menuCounter = 0;

uint16_t getPrevFreq(uint16_t x) {

  for (uint16_t i = x; i >= 0; i--) {

    Note note = notes[i];
    if (note.freq != NOTE_REST && note.freq != TONES_END) return note.freq;

  }

  return NOTE_REST;

}




const uint16_t noteSeq[] = { NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6 };


uint8_t getNoteYVal(uint16_t currNote) {

  for (uint8_t x = 0; x < 15; x++) {
    if (noteSeq[x] == currNote) {
      return 58 - (x * 4);
    }
  } 

  return 0;

}

uint16_t getNoteAbove(uint16_t currNote) {

  for (uint8_t x = 0; x < 15; x++) {
    if (noteSeq[x] == currNote) {
      if (x == 14) {
        return noteSeq[14];
      }
      else {
        return noteSeq[x + 1];
      }
    }
  } 

  return 0;

}

uint16_t getNoteBelow(uint16_t currNote) {

  for (uint8_t x = 0; x < 15; x++) {
    if (noteSeq[x] == currNote) {
      if (x == 0) {
        return noteSeq[0];
      }
      else {
        return noteSeq[x - 1];
      }
    }
  } 

  return 0;

}


uint16_t getNoteMiddle() {

  return NOTE_B5;
  
}

/*
#define TONES_END 0x8000

#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
*/


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  arduboy.boot();
  arduboy.flashlight();

  arduboy.initRandomSeed();
  arduboy.audio.on();

  for (uint8_t x = 0; x < numberOfNotes; x++) {
    notes[x].freq = 0;
    notes[x].duration = noteLength;
  }

  notes[0].freq = TONES_END;

  noteY = NOTE_B5;
  sound.tone(NOTE_A3, 500);

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */


void loop() {

  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  arduboy.pollButtons();

  composer_Update();
  composer_Render();

  arduboy.display(true);
}
