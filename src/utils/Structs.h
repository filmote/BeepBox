#pragma once

#include "Constants.h"
#include "Arduboy2Ext.h"



enum GameState : uint8_t {
  SplashScreen,
  Composer
};

enum MenuMode : uint8_t {
  Tempo,
  Range,
  Nothing
};

struct Note {
  uint16_t freq;
  uint16_t duration;
  uint8_t getDurationInterval(uint16_t noteLength) { return (duration / noteLength); }
  uint8_t getDurationIndex(uint16_t noteLength) { return (duration / noteLength) - 1; }
};

struct ComposerVars {
  uint8_t hasBeenSaved;
  Note notes[NUMBER_OF_NOTES] = {};
  uint8_t noteX = 0;
  uint16_t noteY = 0;
  uint8_t menuCounter = 0;
  uint16_t noteLength = 125;
  uint8_t range = 3;
};

struct MenuVars {
  uint8_t page = 0;
  uint8_t firstIndex = 0;
  uint8_t secondIndex = 0;
  MenuMode mode = MenuMode::Nothing;
};
