#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
uint8_t maskAction = 0;

void bitmapScreen_Update() { 

  if (imageVars.menuCounter < MENU_DELAY) {

    if (arduboy.justPressed(A_BUTTON)) {

      if ((imageVars.image[imageVars.y / 8][imageVars.x] & (1 << (imageVars.y % 8))) > 0) {

        imageVars.image[imageVars.y / 8][imageVars.x] = imageVars.image[imageVars.y / 8][imageVars.x] & ~(1 << (imageVars.y % 8));
        maskAction = 0;

      }
      else {

        imageVars.image[imageVars.y / 8][imageVars.x] = imageVars.image[imageVars.y / 8][imageVars.x] | (1 << (imageVars.y % 8));
        maskAction = 1;

      }

    }

    if (arduboy.pressed(A_BUTTON)) {

      if (arduboy.justPressed(UP_BUTTON) && imageVars.y > 0) {
        imageVars.y--;
        bitmapScreen_Update_Toggle(maskAction);
      }

      if (arduboy.justPressed(DOWN_BUTTON) && imageVars.y < imageVars.yDim - 1) {
        imageVars.y++;
        bitmapScreen_Update_Toggle(maskAction);
      }

      if (arduboy.justPressed(LEFT_BUTTON) && imageVars.x > 0) {
        imageVars.x--;
        bitmapScreen_Update_Toggle(maskAction);
      }

      if (arduboy.justPressed(RIGHT_BUTTON) && imageVars.x < imageVars.xDim - 1) {
        imageVars.x++;
        bitmapScreen_Update_Toggle(maskAction);
      }

    }
    else {
              
      if (arduboy.justPressed(UP_BUTTON) && imageVars.y > 0) {
        imageVars.y--;
      }

      if (arduboy.justPressed(DOWN_BUTTON) && imageVars.y < imageVars.yDim - 1) {
        imageVars.y++;
      }

      if (arduboy.justPressed(LEFT_BUTTON) && imageVars.x > 0) {
        imageVars.x--;
      }

      if (arduboy.justPressed(RIGHT_BUTTON) && imageVars.x < imageVars.xDim - 1) {
        imageVars.x++;
      }

    }

    // -----------------------------------------------------------------------------------
    //  B  Pressed Together

    if (arduboy.justPressed(B_BUTTON)) {
      imageVars.menuCounter = 1;
    }

    if (imageVars.menuCounter < MENU_DELAY && arduboy.pressed(B_BUTTON)) {
      imageVars.menuCounter++;
    }

    if (arduboy.notPressed(B_BUTTON)) {
      imageVars.menuCounter = 0;
    }

  }
  else {

    switch (menu.image.page) {

      case 0:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.image.page = 1;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.image.page = 1;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.image.firstIndex > 0) {

          if (menu.image.firstIndex == 3) {

            uint8_t c;
            EEPROM.get(IMAGE_EEPROM, c);

            if (c == 'c') { 
              menu.image.firstIndex--;
            }
            else {
              menu.image.firstIndex = 0;
            }

          }
          else {
            menu.image.firstIndex--;
          }

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.image.firstIndex < 4) {

          if (menu.image.firstIndex == 0) {

            uint8_t c;
            EEPROM.get(IMAGE_EEPROM, c);

            if (c == 'c') { 
              menu.image.firstIndex++;
            }
            else {
              menu.image.firstIndex = 3;
            }

          }
          else {
            menu.image.firstIndex++;
          }

        }
        
        if (arduboy.justPressed(B_BUTTON)) {
          imageVars.menuCounter = 0;
        }

        if (arduboy.justPressed(A_BUTTON)) {

          switch (menu.image.firstIndex) {

            case 0:
              imageVars.hasBeenSaved = 'c';
              EEPROM.put(IMAGE_EEPROM, imageVars);
              break;

            case 1:
              {
                uint8_t c;
                EEPROM.get(IMAGE_EEPROM, c);
                if (c == 'c') EEPROM.get(IMAGE_EEPROM, imageVars);
              }
              break;

            case 2:
              EEPROM.put(IMAGE_EEPROM, 'd');
              imageVars.hasBeenSaved = ' ';
              menu.image.firstIndex = 0;
              break;

            case 3:
              for (uint8_t y = 0; y < 2; y++) {
                for (uint8_t x = 0 ; x < 16; x++) {
                  imageVars.image[y][x] = 0;
                }
              }
              break;

            case 4:
              for (uint8_t y = 0; y < 2; y++) {
                for (uint8_t x = 0 ; x < 16; x++) {
                  imageVars.image[y][x] = ~imageVars.image[y][x];
                }
              }
              break;

          }

        }

        break;

      case 1:

        if (arduboy.justPressed(LEFT_BUTTON) && (menu.image.mode == MenuMode::Nothing)) {

          menu.image.page = 0;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON) && menu.image.mode == MenuMode::Nothing) {

          menu.image.page = 0;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.image.secondIndex > 0 && menu.image.mode == MenuMode::Nothing) {

          menu.image.secondIndex--;

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.image.secondIndex < 3 && menu.image.mode == MenuMode::Nothing) {

          menu.image.secondIndex++;

        }

        if (menu.image.mode != MenuMode::Nothing && arduboy.justPressed(LEFT_BUTTON)) {

          switch (menu.image.secondIndex) {

            case 0:
              if (imageVars.xDim > 1) {
                
                imageVars.xDim--;

              }
              if (imageVars.x == imageVars.xDim) imageVars.x--;
              break;

            case 1:
              if (imageVars.yDim > 1) {
                
                imageVars.yDim--;

              }
              if (imageVars.y == imageVars.yDim) imageVars.y--;
              break;            

          }
                    
        }

        if (menu.image.mode != MenuMode::Nothing && arduboy.justPressed(RIGHT_BUTTON)) {

          switch (menu.image.secondIndex) {

            case 0:
              if (imageVars.xDim < 16) {

                imageVars.xDim++;

              }
              break;

            case 1:
              if (imageVars.yDim < 16) {

                imageVars.yDim++;

              }
              break;            

          }
                    
        }

        if (arduboy.justPressed(A_BUTTON)) {

          if (menu.image.mode == MenuMode::Nothing) {

            switch (menu.image.secondIndex) {

              case 0 ... 1:
                menu.image.mode = static_cast<MenuMode>(menu.image.secondIndex + 2);
                break;

              case 2:
                exportToSerial();
                break;

              case 3:
                menu.image.mode = MenuMode::Nothing;
                menu.image.page = 0;
                menu.image.secondIndex = 0;
                imageVars.menuCounter = 0;
                gameState = GameState::TitleScreen;

            }

          }
          else {

            menu.image.mode = MenuMode::Nothing;

          }

        }
    
        if (arduboy.justPressed(B_BUTTON)) {
          imageVars.menuCounter = 0;
          menu.image.mode = MenuMode::Nothing;
        }

        break; 
    
    }

  }

}

void bitmapScreen_Update_Toggle(uint8_t maskAction) {

  if (maskAction == 0) {
    imageVars.image[imageVars.y / 8][imageVars.x] = imageVars.image[imageVars.y / 8][imageVars.x] & ~(1 << (imageVars.y % 8));
  }
  else {
    imageVars.image[imageVars.y / 8][imageVars.x] = imageVars.image[imageVars.y / 8][imageVars.x] | (1 << (imageVars.y % 8));
  }

}
