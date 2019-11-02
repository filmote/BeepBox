#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include <EEPROM.h>

void composer_Render() {

  arduboy.drawFastHLine(0, 16, WIDTH);
  arduboy.drawFastHLine(0, 24, WIDTH);
  arduboy.drawFastHLine(0, 32, WIDTH);
  arduboy.drawFastHLine(0, 40, WIDTH);
  arduboy.drawFastHLine(0, 48, WIDTH);

  bool flash = arduboy.getFrameCountHalf(48);

  int16_t xPos = 64;

//  for (uint8_t i = noteX; i < (noteX + 12 > maxNoteX ? maxNoteX : noteX + 12); i++) {
  for (uint16_t i = noteX; i < noteX + 12; i++) {

    Note note = notes[i];


    // If rendering the focus note and the freq is a 'pause' then render it at the current 'Y' position ..

    if (i == noteX && note.freq == NOTE_REST) {
      note.freq = noteY;
    }

    if (note.freq == TONES_END) {
      break;
    }
    else {

      if (note.duration > 0) {

        // If rendering the focus node ..

        if ((((i == noteX && !flash) || i > noteX || note.freq != noteY)) && note.freq != NOTE_REST)
        {
          // if (i == noteX) {
          //   Serial.print("1 ");
          //   Serial.print(note.freq);
          //   Serial.print(" ");
          //   Serial.println(getNoteYVal(note.freq));
          // }

          Sprites::drawExternalMask(xPos, getNoteYVal(note.freq), Images::Note, Images::Note_Mask, note.getDurationIndex(), note.getDurationIndex());
        }

        //if ((i == noteX && !flash) || i > noteX || freq == NOTE_REST) {
        if (((i == noteX && !flash) || i > noteX) && note.freq == NOTE_REST) {
          // if (i == noteX) {
          //   Serial.print("2 ");
          //   Serial.print(getPrevFreq(i));
          //   Serial.print(" ");
          //   Serial.println(getNoteYVal(getPrevFreq(i)));
          // }
          Sprites::drawExternalMask(xPos, getNoteYVal(getPrevFreq(i)), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(), note.getDurationIndex());
        }

      }

      // Render flashing cursor ..

      if (i == noteX && flash) {
          // Serial.print("3 ");
          // Serial.print(noteY);
          // Serial.print(" ");
          // Serial.println(getNoteYVal(noteY));



        if (note.duration == 0) {
          Sprites::drawExternalMask(xPos, getNoteYVal(noteY), Images::Note_Hollow, Images::Note_Mask, 0, 0);
        }
        else {
          Sprites::drawExternalMask(xPos, getNoteYVal(noteY), Images::Note_Hollow, Images::Note_Mask, note.getDurationIndex(), note.getDurationIndex());
        }

      }

    }

    xPos = xPos + (note.getDurationInterval() * 6);

  }
  

  // Render previous notes ..

  xPos = 64;

  for (int16_t i = noteX - 1; i > noteX - 24; i--) {

    Note note = notes[i];

    if (i >= 0 && note.freq != NOTE_REST) {
      Sprites::drawSelfMasked(xPos - (note.getDurationInterval() * 6), getNoteYVal(note.freq), Images::Note, note.getDurationIndex());
    }

    if (i >= 0 && note.freq == NOTE_REST) {
      Sprites::drawExternalMask(xPos - (note.getDurationInterval() * 6), getNoteYVal(getPrevFreq(i)), Images::Note_Pause, Images::Note_Mask, note.getDurationIndex(), note.getDurationIndex());
    }

    xPos = xPos - (note.getDurationInterval() * 6);

  }


  if (menuCounter == 100) {

    const uint8_t yPos[] = { 11, 19, 29, 37, 45, 55 };

    arduboy.fillRect(54,0, WIDTH - 54, HEIGHT, BLACK);
    arduboy.fillRect(54,0, WIDTH - 54, 8, WHITE);
    arduboy.drawRect(54,0, WIDTH - 54, HEIGHT, WHITE);
    
    font3x5.setCursor(56, 1);
    font3x5.setTextColor(BLACK);
    font3x5.print("BEEPBOX");

    font3x5.setCursor(60, 10);
    font3x5.setTextColor(WHITE);
    font3x5.print("Play from start\n");
    font3x5.print("Play from here\n");
    font3x5.setCursor(60, 28);
    font3x5.print("Save to EEPROM\n");
    font3x5.print("Load from EEPROM\n");
    font3x5.print("Clear EEPROM\n");
    font3x5.setCursor(60, 54);
    font3x5.print("Export to Serial\n");

    arduboy.drawFastVLine(57, yPos[menuIndex], 6);

  }

}