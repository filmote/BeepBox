#include "src/utils/Arduboy2Ext.h"
#include "src/images/Splash.h"


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void titleScreen_Update() { 

	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0) {

    gameState = GameState::Composer; 

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void titleScreen_Render() {

  Sprites::drawOverwrite(0, 0, Images::Title, 0);

}
