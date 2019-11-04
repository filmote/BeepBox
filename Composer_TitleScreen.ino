#include "src/utils/Arduboy2Ext.h"
#include "src/images/Splash.h"


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void titleScreen_Update() { 

	auto justPressed = arduboy.justPressedButtons();

  if (arduboy.justPressed(A_BUTTON)) {

    gameState = GameState::Composer; 
    resetAll();

  }

  if (arduboy.justPressed(B_BUTTON)) {

    gameState = GameState::Designer; 
    resetAll();

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void titleScreen_Render() {

  Sprites::drawOverwrite(0, 0, Images::Title, 0);

}
