#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include <EEPROM.h>
#include "src/utils/Constants.h"


const uint8_t PROGMEM yPos[] = { 12, 20, 32, 40, 48 };

void composer_Render() {

  arduboy.drawFastHLine(0, 16, WIDTH);
  arduboy.drawFastHLine(0, 24, WIDTH);
  arduboy.drawFastHLine(0, 32, WIDTH);
  arduboy.drawFastHLine(0, 40, WIDTH);
  arduboy.drawFastHLine(0, 48, WIDTH);

  bool flash = arduboy.getFrameCountHalf(48);

  int16_t xPos = 64;

  for (uint16_t i = composerVars.noteX; i < composerVars.noteX + 12; i++) {

    Note note = composerVars.notes[i];


    // If rendering the focus note and the freq is a 'pause' then render it at the current 'Y' position ..

    if (i == composerVars.noteX && note.freq == NOTE_REST) {
      note.freq = composerVars.noteY;
    }

    if (note.freq == TONES_END) {

      arduboy.drawRect(xPos, 14, 3, 37, WHITE);
      break;

    }
    else {


      // Draw additional lines on the staff for this note if required ..

      int8_t x = getNoteLinesAboveBelow(composerVars.range, note.freq);

      if (x > 0) {
        for (uint8_t y = 0; y <= x; y = y + 2) {
          arduboy.drawFastHLine(xPos - 2, 16 - (y*4), 9 + (6 * note.getDurationIndex(composerVars.noteLength)), WHITE);
        }
      }
      else if (x < 0) {
        for (uint8_t y = 0; y <= -x; y = y + 2) {
          arduboy.drawFastHLine(xPos - 2, 48 + (y*4), 9 + (6 * note.getDurationIndex(composerVars.noteLength)), WHITE);
        }

      }


      if (note.duration > 0) {

        // If rendering the focus node ..

        if ((i == composerVars.noteX && !flash) || i > composerVars.noteX || note.freq != composerVars.noteY) {

          if (note.freq < 0x8000) {   
            Sprites::drawExternalMask(xPos, getNoteYVal(composerVars.range, note.freq), Images::Note, Images::Note_Mask, note.getDurationIndex(composerVars.noteLength), note.getDurationIndex(composerVars.noteLength));
          }
          else {
            Sprites::drawExternalMask(xPos, getNoteYVal(composerVars.range, note.freq), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(composerVars.noteLength), note.getDurationIndex(composerVars.noteLength));
          }

        }

      }

      // Render flashing cursor ..

      if (i == composerVars.noteX && flash) {

        if (note.duration == 0) {
          Sprites::drawExternalMask(xPos, getNoteYVal(composerVars.range, composerVars.noteY), Images::Note_Hollow, Images::Note_Mask, 0, 0);
        }
        else {
          Sprites::drawExternalMask(xPos, getNoteYVal(composerVars.range, composerVars.noteY), Images::Note_Hollow, Images::Note_Mask, note.getDurationIndex(composerVars.noteLength), note.getDurationIndex(composerVars.noteLength));
        }

      }

    }

    xPos = xPos + (note.getDurationInterval(composerVars.noteLength) * 6);

  }
  

  // Render previous notes ..

  xPos = 64;

  for (int16_t i = composerVars.noteX - 1; i > composerVars.noteX - 24; i--) {

    Note note = composerVars.notes[i];


    // Draw additional lines on the staff for this note if required ..
    
    int8_t x = getNoteLinesAboveBelow(composerVars.range, note.freq);

    if (x > 0) {
      for (uint8_t y = 0; y <= x; y = y + 2) {
        arduboy.drawFastHLine(xPos - (note.getDurationInterval(composerVars.noteLength) * 6) - 2, 16 - (y*4), 9 + (6 * note.getDurationIndex(composerVars.noteLength)), WHITE);
      }
    }
    else if (x < 0) {
      for (uint8_t y = 0; y <= -x; y = y + 2) {
        arduboy.drawFastHLine(xPos - (note.getDurationInterval(composerVars.noteLength) * 6) - 2, 48 + (y*4), 9 + (6 * note.getDurationIndex(composerVars.noteLength)), WHITE);
      }

    }


    // Render note ..

    if (i >= 0) {
      if (note.freq < 0x8000) {
        Sprites::drawExternalMask(xPos - (note.getDurationInterval(composerVars.noteLength) * 6), getNoteYVal(composerVars.range, note.freq), Images::Note, Images::Note_Mask, note.getDurationIndex(composerVars.noteLength), note.getDurationIndex(composerVars.noteLength));
      }
      else {
        Sprites::drawExternalMask(xPos - (note.getDurationInterval(composerVars.noteLength) * 6), getNoteYVal(composerVars.range, note.freq), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(composerVars.noteLength), note.getDurationIndex(composerVars.noteLength));
      }
    }

    xPos = xPos - (note.getDurationInterval(composerVars.noteLength) * 6);

  }



  if (composerVars.menuCounter == MENU_DELAY) {

    arduboy.fillRect(54,0, WIDTH - 54, HEIGHT, BLACK);
    arduboy.fillRect(54,0, WIDTH - 54, 8, WHITE);
    arduboy.drawRect(54,0, WIDTH - 54, HEIGHT, WHITE);

    Sprites::drawErase(117, 2, Images::Arrow_Left, 0);
    Sprites::drawErase(121, 2, Images::Arrow_Right, 0);
    
    font3x5.setCursor(56, 1);
    font3x5.setTextColor(BLACK);
    font3x5.print("BEEPBOX");
    font3x5.setCursor(60, 11);
    font3x5.setTextColor(WHITE);

    switch (menu.page) {

      case 0:

        font3x5.print("Play from start\n");
        font3x5.print("Play from cursor\n");
        font3x5.setCursor(60, 31);
        font3x5.print("Save to EEPROM\n");

        uint8_t c;
        EEPROM.get(50, c);

        if (c == 'c') { 
          font3x5.print("Load from EEPROM\n"); 
          font3x5.print("Clear EEPROM\n");
        }
        else {
          font3x5.print("\n\n");
        }

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos[menu.firstIndex]), 5);
        break;

      case 1:

        font3x5.print("Tempo: ");
        
        if (menu.mode == MenuMode::Tempo) {
          Sprites::drawOverwrite(87, 12, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(114, 12, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 11, 20, 7, WHITE);
  
          font3x5.setCursor(96, 11);
          font3x5.setTextColor(BLACK);
          font3x5.print(composerVars.noteLength);
          font3x5.setTextColor(WHITE);
        }
        else {
          font3x5.print(composerVars.noteLength);
          font3x5.print(" ms");
        }

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(60, 19);
        font3x5.print("Range: ");
        
        if (menu.mode == MenuMode::Range) {
          Sprites::drawOverwrite(87, 20, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(105, 20, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 19, 11, 7, WHITE);
  
          font3x5.setCursor(96, 19);
          font3x5.setTextColor(BLACK);
          font3x5.print(composerVars.range + 1);
          font3x5.setTextColor(WHITE);
        }
        else {
          font3x5.print(composerVars.range + 1);
        }

        font3x5.setCursor(60, 31);
        font3x5.print("Clear Tune\nExport to Serial\n");

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos[menu.secondIndex]), 5);
        break;

      case 2:
        font3x5.print("A+L  Shrink note\n");
        font3x5.print("A+R  Extend note\n");
        font3x5.print("A+U  Move up\n");
        font3x5.print("A+D  Move down\n");
        font3x5.print("B+L  Delete note\n");
        font3x5.print("B+R  Insert a note\n\n");
        break;

      case 3:
        font3x5.print("To include a rest\nnote, use A+L to\nshrink a normal\nnote down to zero\nlength to toggle\nbetween modes.");
        break;

    }

  }

  arduboy.fillRect(-1, 56, 30, 8, BLACK);
  arduboy.fillRect(0, 57, 28, 7, WHITE);

  font3x5.setCursor(1, 57);
  font3x5.setTextColor(BLACK);
  if (composerVars.noteX + 1 < 10) font3x5.print("0");
  if (composerVars.noteX + 1 < 100) font3x5.print("0");
  font3x5.print(composerVars.noteX + 1);
  font3x5.setCursor(16, 57);
  font3x5.print(NUMBER_OF_NOTES);
  arduboy.drawPixel(14, 60, BLACK);

}

