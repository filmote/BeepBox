#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include <EEPROM.h>

void composer_Update() {

  // Get focus note ..

  Note &note = notes[noteX];
  // Serial.print(noteX);
  // Serial.print(" ");
  // Serial.println(note.freq);

  if (menuCounter < 100) {

    if (arduboy.justPressed(A_BUTTON)) {

    Serial.print(">>>>> ");
    Serial.print(noteY);
    Serial.print(" ");
    Serial.println(note.freq);

      if (note.freq == TONES_END) {
        notes[noteX + 1].freq = TONES_END;
      }
      note.freq = noteY;
      if (note.duration == 0)
        note.duration = noteLength;

      if (maxNoteX < noteX) maxNoteX = noteX + 1;

    }
    else if (arduboy.pressed(A_BUTTON))
    {
  //       Serial.print(noteX);
  //       Serial.print(",");
  //       Serial.print(noteY);
  //       Serial.print(" ");
  //       Serial.print(note.freq);
  //       Serial.print(",");
  //       Serial.println(note.duration);

  // Serial.print("Right ");
  // Serial.println(note.duration);
      if (arduboy.justPressed(LEFT_BUTTON))
      {

        if (note.duration > 1 * noteLength)
          note.duration = note.duration - noteLength;

      }
      else if (arduboy.justPressed(RIGHT_BUTTON))
      {


        if (note.duration < 8 * noteLength)
          note.duration = note.duration + noteLength;
      }
      else if (arduboy.justPressed(UP_BUTTON))
      {

        if (noteY > 0) {
          note.freq = getNoteAbove(note.freq);
          noteY = getNoteAbove(noteY);
        }
      }
      else if (arduboy.justPressed(DOWN_BUTTON))
      {

        if (noteY > 0) {
          note.freq = getNoteBelow(note.freq);
          noteY = getNoteBelow(noteY);
        }
      }
    }


    if (arduboy.justPressed(B_BUTTON)) {
      Serial.println("1 _________________ ");
      for (uint16_t x = noteX; x < 200; x++)
      {
        Serial.print(notes[x].freq);
        Serial.print(",");
        Serial.print(notes[x].duration);
        Serial.print(" ");
      }
      Serial.println(" ");
    }
    else if (arduboy.pressed(B_BUTTON)) {

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        for (uint16_t x = numberOfNotes - 1; x >= noteX; x--) {

          notes[x + 1] = notes[x];
        }

        notes[noteX].freq = notes[noteX + 1].freq;
        notes[noteX].duration = noteLength;
      }

      if (arduboy.justPressed(LEFT_BUTTON) && noteX > 0) {

        for (uint16_t x = noteX - 1; x < numberOfNotes - 1; x++) {

          notes[x] = notes[x + 1];

        }

        notes[numberOfNotes].freq = 0;
        notes[numberOfNotes].duration = noteLength;

        noteX--;
        noteY = notes[noteX].freq;


      }

    }


    // A + B Not Pressed

    if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON)) { 

      if (arduboy.justPressed(UP_BUTTON) && note.duration == 0) { // a blank note
  //      noteY = noteY - 4;
        noteY = getNoteAbove(noteY);
      }

      if (arduboy.justPressed(DOWN_BUTTON) && note.duration == 0) { // a blank note
    //    noteY = noteY + 4;
        noteY = getNoteBelow(noteY);
      }

      if (arduboy.justPressed(LEFT_BUTTON)) {

        if (noteX > 0) {
          noteX--;
          noteY = getPrevFreq(noteX);
        }

      }

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        if (noteX < 198) {
          noteX++;
          noteY = getPrevFreq(noteX);
          Serial.print("++ ");
          Serial.print(noteX);
          Serial.print(" ++ ");
          Serial.println(noteY);
        }

        if (noteY == 0) { noteY = getNoteMiddle(); }

      }

    }


    if ((arduboy.pressed(A_BUTTON) && arduboy.justPressed(B_BUTTON)) ||
        (arduboy.justPressed(A_BUTTON) && arduboy.pressed(B_BUTTON))) {

      // Serial.print(notes[noteX].freq);
      // Serial.print(" ");
      // if (notes[noteX].freq != NOTE_REST) {
      // Serial.println("Make Rest");
      //   notes[noteX].freq = NOTE_REST;
      // }
      // else {
      // Serial.print("Make ");
      // Serial.println(noteY);
      //   notes[noteX].freq = noteY;
      // }

      menuCounter = 1;
    }

    if (menuCounter < 100 && arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) {
      menuCounter++;
    }

    if (arduboy.notPressed(A_BUTTON) || arduboy.notPressed(B_BUTTON)) {
      menuCounter = 0;
    }

  }
  else {

    // Menu Controls ..

    if (arduboy.justPressed(UP_BUTTON) && menuIndex > 0) {
      menuIndex--;
    }

    if (arduboy.justPressed(DOWN_BUTTON) && menuIndex < 5) {
      menuIndex++;
    }
    
    if (arduboy.justPressed(B_BUTTON)) {
      menuCounter = 0;
    }

    if (arduboy.justPressed(A_BUTTON)) {

      switch (menuIndex) {

        case 0:
          sound.tonesInRAM(static_cast<uint16_t *>(&notes[0].freq));
          break;

        case 1:
          sound.tonesInRAM(static_cast<uint16_t *>(&notes[noteX].freq));
          break;

      }

    }

  }

}