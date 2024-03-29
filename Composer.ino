#include "src/utils/Arduboy2Ext.h"

#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Structs.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Font3x5 font3x5;

MusicVars musicVars;
ImageVars imageVars;
MenuVars menu;

GameState gameState = GameState::SplashScreen;


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  arduboy.boot();
  arduboy.flashlight();

  arduboy.initRandomSeed();
  arduboy.audio.on();

  resetAll();

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */

void loop() {

  if (!(arduboy.nextFrame())) return;

  arduboy.clear();
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::SplashScreen:
      splashScreen_Update();
      splashScreen_Render();
      break;

    case GameState::TitleScreen:
      titleScreen_Update();
      titleScreen_Render();
      break;

    case GameState::Composer:
      composer_Update();
      composer_Render();
      break;

    case GameState::Designer:
      bitmapScreen_Update();
      bitmapScreen_Render();
      break;
      
  }

  arduboy.display(true);

}
