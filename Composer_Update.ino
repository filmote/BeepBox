#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>


void composer_Update() {

  // Get focus note ..

  Note &note = composerVars.notes[composerVars.noteX];

  if (composerVars.menuCounter < MENU_DELAY) {


    // -----------------------------------------------------------------------------------
    //  A Pressed

    if (arduboy.justPressed(A_BUTTON)) {

      if (note.freq == TONES_END) {
        composerVars.notes[composerVars.noteX + 1].freq = TONES_END;
      }
      note.freq = composerVars.noteY;
      if (note.duration == 0)
        note.duration = composerVars.noteLength;

    }
    else if (arduboy.pressed(A_BUTTON)) {

      if (arduboy.justPressed(LEFT_BUTTON)) {

        if (note.duration > 1 * composerVars.noteLength) {
          note.duration = note.duration - composerVars.noteLength;
        }
        else {
          if (note.freq > 0x8000) {
            note.freq = note.freq & 0x7FFF;
          }
          else {
            note.freq = note.freq | 0x8000;
          }

        }

      }
      else if (arduboy.justPressed(RIGHT_BUTTON)) {

        if (note.duration < 8 * composerVars.noteLength)
          note.duration = note.duration + composerVars.noteLength;
      }
      else if (arduboy.justPressed(UP_BUTTON))
      {

        if (composerVars.noteY > 0) {
          note.freq = getNoteAbove(composerVars.range, note.freq);
          composerVars.noteY = getNoteAbove(composerVars.range, composerVars.noteY);
        }
      }
      else if (arduboy.justPressed(DOWN_BUTTON))
      {

        if (composerVars.noteY > 0) {
          note.freq = getNoteBelow(composerVars.range, note.freq);
          composerVars.noteY = getNoteBelow(composerVars.range, composerVars.noteY);
        }
      }
    }


    // -----------------------------------------------------------------------------------
    //  B Pressed

    if (arduboy.justPressed(B_BUTTON)) {

      Serial.println("1 _________________ ");
      for (uint16_t x = 0; x < NUMBER_OF_NOTES; x++)
      {
        Serial.print(composerVars.notes[x].freq);
        Serial.print(",");
        Serial.print(composerVars.notes[x].duration);
        Serial.print(" ");
      }
      Serial.println(" ");

    }
    else if (arduboy.pressed(B_BUTTON)) {

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        for (uint16_t x = NUMBER_OF_NOTES - 2; x >= composerVars.noteX; x--) {

          composerVars.notes[x + 1] = composerVars.notes[x];

        }

        composerVars.notes[composerVars.noteX].freq = composerVars.notes[composerVars.noteX + 1].freq;
        composerVars.notes[composerVars.noteX].duration = composerVars.noteLength;

      }

      if (arduboy.justPressed(LEFT_BUTTON) && composerVars.noteX > 0) {

        for (uint16_t x = composerVars.noteX; x < NUMBER_OF_NOTES - 1; x++) {

          composerVars.notes[x] = composerVars.notes[x + 1];

        }

        composerVars.notes[NUMBER_OF_NOTES].freq = 0;
        composerVars.notes[NUMBER_OF_NOTES].duration = composerVars.noteLength;

        composerVars.noteX--;
        composerVars.noteY = composerVars.notes[composerVars.noteX].freq;

      }

    }


    // -----------------------------------------------------------------------------------
    //  A + B Not Pressed

    if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON)) { 

      // if (arduboy.justPressed(UP_BUTTON) && note.duration == 0) { // a blank note
      //   composerVars.noteY = getNoteAbove(composerVars.range, composerVars.noteY);
      // }

      // if (arduboy.justPressed(DOWN_BUTTON) && note.duration == 0) { // a blank note
      //   composerVars.noteY = getNoteBelow(composerVars.range, composerVars.noteY);
      // }

      if (arduboy.justPressed(LEFT_BUTTON)) {

        if (composerVars.noteX > 0) {
          composerVars.noteX--;
          composerVars.noteY = getFreq(composerVars.noteX);
        }

      }

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        if ((composerVars.noteX == 0 && composerVars.notes[composerVars.noteX].freq != TONES_END) || (composerVars.noteX > 0 && composerVars.noteX < NUMBER_OF_NOTES - 2)) {

          composerVars.noteX++;
          composerVars.noteY = getFreq(composerVars.noteX);
          
          if (composerVars.notes[composerVars.noteX].freq == TONES_END) {

            Note &note = composerVars.notes[composerVars.noteX];
            makeNextNote(note);

          }
          
        }

        if (composerVars.noteY == 0) { composerVars.noteY = getNoteMiddle(composerVars.range); }

      }

      if (arduboy.justPressed(UP_BUTTON)) {

        if ((composerVars.noteX == 0 && composerVars.notes[composerVars.noteX].freq != TONES_END) || (composerVars.noteX < NUMBER_OF_NOTES - 2 && composerVars.notes[composerVars.noteX + 1].freq == TONES_END)) {

          composerVars.noteX++;
          composerVars.noteY = getNoteAbove(composerVars.range, getFreq(composerVars.noteX));
          Note &note = composerVars.notes[composerVars.noteX];
          makeNextNote(note);
          
        }

        if (composerVars.noteY == 0) { composerVars.noteY = getNoteMiddle(composerVars.range); }

      }

      if (arduboy.justPressed(DOWN_BUTTON)) {

        if ((composerVars.noteX == 0 && composerVars.notes[composerVars.noteX].freq != TONES_END) || (composerVars.noteX < NUMBER_OF_NOTES - 2 && composerVars.notes[composerVars.noteX + 1].freq == TONES_END)) {

          composerVars.noteX++;
          composerVars.noteY = getNoteBelow(composerVars.range, getFreq(composerVars.noteX));
          Note &note = composerVars.notes[composerVars.noteX];
          makeNextNote(note);
          
        }

        if (composerVars.noteY == 0) { composerVars.noteY = getNoteMiddle(composerVars.range); }

      }

    }


    // -----------------------------------------------------------------------------------
    //  A + B  Pressed Together

    if ((arduboy.pressed(A_BUTTON) && arduboy.justPressed(B_BUTTON)) ||
        (arduboy.justPressed(A_BUTTON) && arduboy.pressed(B_BUTTON))) {
      composerVars.menuCounter = 1;
    }

    if (composerVars.menuCounter < MENU_DELAY && arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) {
      composerVars.menuCounter++;
    }

    if (arduboy.notPressed(A_BUTTON) || arduboy.notPressed(B_BUTTON)) {
      composerVars.menuCounter = 0;
    }

  }
  else {


    // -----------------------------------------------------------------------------------
    //  Menu Controls ..

    switch (menu.page) {

      case 0:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.page = 3;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.page = 1;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.firstIndex > 0) {

            menu.firstIndex--;

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.firstIndex < 4) {

          if (menu.firstIndex == 2) {

            uint8_t c;
            EEPROM.get(50, c);

            if (c == 'c') { 
              menu.firstIndex++;
            }

          }
          else {
            menu.firstIndex++;
          }

        }
        
        if (arduboy.justPressed(B_BUTTON)) {
          composerVars.menuCounter = 0;
        }

        if (arduboy.justPressed(A_BUTTON)) {

          switch (menu.firstIndex) {

            case 0:
              sound.tonesInRAM(static_cast<uint16_t *>(&composerVars.notes[0].freq));
              break;

            case 1:
              sound.tonesInRAM(static_cast<uint16_t *>(&composerVars.notes[composerVars.noteX].freq));
              break;

            case 2:
              composerVars.hasBeenSaved = 'c';
              EEPROM.put(50, composerVars);
              break;

            case 3:
              {
                uint8_t c;
                EEPROM.get(50, c);
                if (c == 'c') EEPROM.get(50, composerVars);
              }
              break;

            case 4:
              EEPROM.put(50, 'd');
              composerVars.hasBeenSaved = ' ';
              menu.firstIndex = 2;
              break;

          }

        }

        break;
  
      case 1:

        if (arduboy.justPressed(LEFT_BUTTON) && (menu.mode == MenuMode::Nothing)) {

          menu.page = 0;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON) && menu.mode == MenuMode::Nothing) {

          menu.page = 2;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.secondIndex > 0 && menu.mode == MenuMode::Nothing) {

          menu.secondIndex--;

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.secondIndex < 3 && menu.mode == MenuMode::Nothing) {

          menu.secondIndex++;

        }

        if (menu.mode != MenuMode::Nothing && arduboy.justPressed(LEFT_BUTTON)) {

          switch (menu.secondIndex) {

            case 0:
              {
                uint16_t noteLength = composerVars.noteLength;
                uint16_t noteLengthAbove = getTempo_Above(noteLength);

                if (noteLength != noteLengthAbove) {

                  composerVars.noteLength = noteLengthAbove;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    composerVars.notes[x].duration = composerVars.notes[x].getDurationInterval(noteLength) * noteLengthAbove;

                  }

                }

              }
              break;

            case 1:
              {
                uint16_t range = composerVars.range;

                if (range > 0) {

                  composerVars.range--;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    if (composerVars.notes[x].freq != TONES_END && composerVars.notes[x].freq != NOTE_REST) {
                      composerVars.notes[x].freq = pgm_read_word( &noteSeq[range - 1][getNoteIndex(range, composerVars.notes[x].freq)] );
                      composerVars.noteY = getFreq(composerVars.noteX);
                    }

                  }

                }

              }
              break;            

          }
                    
        }

        if (menu.mode != MenuMode::Nothing && arduboy.justPressed(RIGHT_BUTTON)) {

          switch (menu.secondIndex) {

            case 0:
              {
                uint16_t noteLength = composerVars.noteLength;
                uint16_t noteLengthBelow = getTempo_Below(noteLength);

                if (noteLength != noteLengthBelow) {

                  composerVars.noteLength = noteLengthBelow;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    composerVars.notes[x].duration = composerVars.notes[x].getDurationInterval(noteLength) * noteLengthBelow;

                  }

                }

              }
              break;

            case 1:
              {
                uint16_t range = composerVars.range;

                if (range < 7) {

                  composerVars.range++;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    if (composerVars.notes[x].freq != TONES_END && composerVars.notes[x].freq != NOTE_REST) {
                      composerVars.notes[x].freq = pgm_read_word(&noteSeq[range + 1][getNoteIndex(range, composerVars.notes[x].freq)]);
                      composerVars.noteY = getFreq(composerVars.noteX);
                    }

                  }

                }

              }
              break;            

          }
                    
        }

        if (arduboy.justPressed(A_BUTTON)) {

          if (menu.mode == MenuMode::Nothing) {

            switch (menu.secondIndex) {

              case 0 ... 1:
                menu.mode = static_cast<MenuMode>(menu.secondIndex);
                break;

              case 2:
                resetAll();
                break;

              case 3:
                exportToSerial();
                break;

            }

          }
          else {

            menu.mode = MenuMode::Nothing;

          }

        }
    
        if (arduboy.justPressed(B_BUTTON)) {
          composerVars.menuCounter = 0;
          menu.mode = MenuMode::Nothing;
        }

        break; 

      case 2:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.page = 1;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.page = 3;
          
        }
        
        if (arduboy.justPressed(B_BUTTON)) {

          composerVars.menuCounter = 0;

        }
        break;

      case 3:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.page = 2;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.page = 0;
          
        }
        
        if (arduboy.justPressed(B_BUTTON)) {

          composerVars.menuCounter = 0;
          
        }
        break;

    }

  }

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
