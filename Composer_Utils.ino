#include "src/utils/Arduboy2Ext.h"

#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Structs.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>


// Return the frequency of the note or the previous, valid frequency ..

uint16_t getFreq(uint16_t x) {

  for (uint16_t i = x; i >= 0; i--) {

    Note note = composerVars.notes[i];
    if (note.freq != TONES_END) return note.freq;

  }

  return composerVars.notes[x].freq;

}


uint8_t getNoteIndex(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      return x;
    }
  } 

  return 0;

}


uint8_t getNoteYVal(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      return 58 - (x * 4);
    }
  } 

  return 0;

}


int8_t getNoteLinesAboveBelow(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {

    if (pgm_read_word(&noteSeq[range][x]) == currNote) {

      if (x < 2) return -(3 - x);
      if (x > 11) return x - 11;
      break;

    }

  } 

  return 0;

}

uint16_t getNoteAbove(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      if (x == 14) {
        return pgm_read_word(&noteSeq[range][14]);
      }
      else {
        return pgm_read_word(&noteSeq[range][x + 1]);
      }
    }
  } 

  return 0;

}

uint16_t getNoteBelow(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;
  
  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      if (x == 0) {
        return pgm_read_word(&noteSeq[range][0]);
      }
      else {
        return pgm_read_word(&noteSeq[range][x - 1]);
      }
    }
  } 

  return 0;

}


uint16_t getNoteMiddle(uint8_t range) {

  return pgm_read_word(&noteSeq[range][7]);
  
}

void resetAll() {

  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {
    composerVars.notes[x].freq = 0;
    composerVars.notes[x].duration = composerVars.noteLength;
  }

  composerVars.range = 3;
  composerVars.notes[0].freq = TONES_END;
  composerVars.noteY = getNoteMiddle(composerVars.range);
  composerVars.noteX = 0;

}


void makeNextNote(Note &note) {

  if (note.freq == TONES_END) {
    composerVars.notes[composerVars.noteX + 1].freq = TONES_END;
  }
  note.freq = composerVars.noteY;
  if (note.duration == 0)
    note.duration = composerVars.noteLength;

}

void exportToSerial() {

  Serial.println("const uint16_t beepBox[] PROGMEM = {");

  for (uint16_t x = 0; x < NUMBER_OF_NOTES; x++) {

    Note note = composerVars.notes[x];
    
    if (note.freq == TONES_END) break;
    if (note.freq > 0x8000) note.freq = 0;

    Serial.print(note.freq);
    Serial.print(",");
    Serial.print(note.duration);
    Serial.print(", ");

    if (x > 0 && x % 8 == 8) Serial.println(""); 

  }
    
  Serial.println("TONES_END };");

}


uint16_t getTempo_Above(uint16_t tempo) {

  for (uint8_t x = 0; x < 7; x++) {

    if (tempo == pgm_read_byte(&tempos[x])) {
      
      return (x < 6 ? pgm_read_byte(&tempos[x + 1]) : pgm_read_byte(&tempos[x])); 

    }

  }

  return tempo;

}

uint16_t getTempo_Below(uint16_t tempo) {

  for (uint8_t x = 0; x < 7; x++) {

    if (tempo == pgm_read_byte(&tempos[x])) {
      
      return (x > 0 ? pgm_read_byte(&tempos[x - 1]) : pgm_read_byte(&tempos[x])); 

    }

  }

  return tempo;

}
