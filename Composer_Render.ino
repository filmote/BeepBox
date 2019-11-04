#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include <EEPROM.h>
#include "src/utils/Constants.h"

void composer_Render() {

  arduboy.drawFastHLine(0, 16, WIDTH);
  arduboy.drawFastHLine(0, 24, WIDTH);
  arduboy.drawFastHLine(0, 32, WIDTH);
  arduboy.drawFastHLine(0, 40, WIDTH);
  arduboy.drawFastHLine(0, 48, WIDTH);

  bool flash = arduboy.getFrameCountHalf(48);

  int16_t xPos = 66;

  for (uint16_t i = musicVars.x; i < musicVars.x + 12; i++) {

    Note note = musicVars.notes[i];


    // If rendering the focus note and the freq is a 'pause' then render it at the current 'Y' position ..

    if (i == musicVars.x && note.freq == NOTE_REST) {
      note.freq = musicVars.y;
    }

    if (note.freq == TONES_END) {

      arduboy.drawRect(xPos, 14, 3, 37, WHITE);
      break;

    }
    else {


      // Draw additional lines on the staff for this note if required ..

      int8_t x = getNoteLinesAboveBelow(musicVars.range, note.freq);

      if (x > 0) {
        for (uint8_t y = 0; y <= x; y = y + 2) {
          arduboy.drawFastHLine(xPos - 2, 16 - (y*4), 9 + (6 * note.getDurationIndex(musicVars.noteLength)), WHITE);
        }
      }
      else if (x < 0) {
        for (uint8_t y = 0; y <= -x; y = y + 2) {
          arduboy.drawFastHLine(xPos - 2, 48 + (y*4), 9 + (6 * note.getDurationIndex(musicVars.noteLength)), WHITE);
        }

      }


      if (note.duration > 0) {

        // If rendering the focus node ..

        if ((i == musicVars.x && !flash) || i > musicVars.x || note.freq != musicVars.y) {

          if (note.freq < 0x8000) {   
            Sprites::drawExternalMask(xPos, getNoteYVal(musicVars.range, note.freq), Images::Note, Images::Note_Mask, note.getDurationIndex(musicVars.noteLength), note.getDurationIndex(musicVars.noteLength));
          }
          else {
            Sprites::drawExternalMask(xPos, getNoteYVal(musicVars.range, note.freq), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(musicVars.noteLength), note.getDurationIndex(musicVars.noteLength));
          }

        }

      }

      // Render flashing cursor ..

      if (i == musicVars.x && flash) {

        if (note.duration == 0) {
          Sprites::drawExternalMask(xPos, getNoteYVal(musicVars.range, musicVars.y), Images::Note_Hollow, Images::Note_Mask, 0, 0);
        }
        else {
          Sprites::drawExternalMask(xPos, getNoteYVal(musicVars.range, musicVars.y), Images::Note_Hollow, Images::Note_Mask, note.getDurationIndex(musicVars.noteLength), note.getDurationIndex(musicVars.noteLength));
        }

      }

    }

    xPos = xPos + (note.getDurationInterval(musicVars.noteLength) * 6);

  }
  

  // Render previous notes ..

  xPos = 66;

  for (int16_t i = musicVars.x - 1; i > musicVars.x - 24; i--) {

    Note note = musicVars.notes[i];


    // Draw additional lines on the staff for this note if required ..
    
    int8_t x = getNoteLinesAboveBelow(musicVars.range, note.freq);

    if (x > 0) {
      for (uint8_t y = 0; y <= x; y = y + 2) {
        arduboy.drawFastHLine(xPos - (note.getDurationInterval(musicVars.noteLength) * 6) - 2, 16 - (y*4), 9 + (6 * note.getDurationIndex(musicVars.noteLength)), WHITE);
      }
    }
    else if (x < 0) {
      for (uint8_t y = 0; y <= -x; y = y + 2) {
        arduboy.drawFastHLine(xPos - (note.getDurationInterval(musicVars.noteLength) * 6) - 2, 48 + (y*4), 9 + (6 * note.getDurationIndex(musicVars.noteLength)), WHITE);
      }

    }


    // Render note ..

    if (i >= 0) {
      if (note.freq < 0x8000) {
        Sprites::drawExternalMask(xPos - (note.getDurationInterval(musicVars.noteLength) * 6), getNoteYVal(musicVars.range, note.freq), Images::Note, Images::Note_Mask, note.getDurationIndex(musicVars.noteLength), note.getDurationIndex(musicVars.noteLength));
      }
      else {
        Sprites::drawExternalMask(xPos - (note.getDurationInterval(musicVars.noteLength) * 6), getNoteYVal(musicVars.range, note.freq), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(musicVars.noteLength), note.getDurationIndex(musicVars.noteLength));
      }
    }

    xPos = xPos - (note.getDurationInterval(musicVars.noteLength) * 6);

  }



  if (musicVars.menuCounter == MENU_DELAY) {

    arduboy.fillRect(53, 0, WIDTH - 53, HEIGHT, BLACK);
    arduboy.fillRect(54, 0, WIDTH - 54, 8, WHITE);
    arduboy.drawRect(54, 0, WIDTH - 54, HEIGHT, WHITE);

    Sprites::drawErase(117, 2, Images::Arrow_Left, 0);
    Sprites::drawErase(121, 2, Images::Arrow_Right, 0);
    
    font3x5.setCursor(56, 1);
    font3x5.setTextColor(BLACK);
    font3x5.print("BEEPBOX");
    font3x5.setCursor(60, 11);
    font3x5.setTextColor(WHITE);

    switch (menu.music.page) {

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

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos[menu.music.firstIndex]), 5);
        break;

      case 1:

        font3x5.print("Tempo: ");
        
        if (menu.music.mode == MenuMode::Tempo) {
          Sprites::drawOverwrite(87, 12, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(114, 12, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 11, 20, 7, WHITE);
  
          font3x5.setCursor(96, 11);
          font3x5.setTextColor(BLACK);
          font3x5.print(musicVars.noteLength);
          font3x5.setTextColor(WHITE);
        }
        else {
          font3x5.print(musicVars.noteLength);
          font3x5.print(" ms");
        }

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(60, 19);
        font3x5.print("Range: ");
        
        if (menu.music.mode == MenuMode::Range) {
          Sprites::drawOverwrite(87, 20, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(105, 20, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 19, 11, 7, WHITE);
  
          font3x5.setCursor(96, 19);
          font3x5.setTextColor(BLACK);
          font3x5.print(musicVars.range + 1);
          font3x5.setTextColor(WHITE);
        }
        else {
          font3x5.print(musicVars.range + 1);
        }

        font3x5.setCursor(60, 31);
        font3x5.print("Clear Tune\nExport to Serial\nReturn to Menu");

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos[menu.music.secondIndex]), 5);
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


  // Render range ..

  arduboy.fillRect(-1, -1, 30, 8, BLACK);
  arduboy.fillRect(0, 0, 28, 7, WHITE);

  font3x5.setCursor(1, 0);
  font3x5.setTextColor(BLACK);
  font3x5.print("Range ");
  font3x5.print(musicVars.range + 1);


  // Render note count ..

  arduboy.fillRect(-1, 56, 30, 8, BLACK);
  arduboy.fillRect(0, 57, 28, 7, WHITE);

  font3x5.setCursor(1, 57);
  font3x5.setTextColor(BLACK);
  if (musicVars.x + 1 < 10) font3x5.print("0");
  if (musicVars.x + 1 < 100) font3x5.print("0");
  font3x5.print(musicVars.x + 1);
  font3x5.setCursor(16, 57);
  font3x5.print(NUMBER_OF_NOTES);
  arduboy.drawPixel(14, 60, BLACK);

}

