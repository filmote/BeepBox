#include <Arduino.h>
#include <Sprites.h>
#include <Print.h>
#include "Font3x5.h"

#define USE_LOWER_CASE

#define FONT3X5_WIDTH 3
#define FONT3X5_HEIGHT 6

#define CHAR_EXCLAMATION 33
#define CHAR_PERIOD 46
#define CHAR_PLUS 43
#define CHAR_COMMA 44
#define CHAR_COLON 58
#define CHAR_LETTER_A 65
#define CHAR_LETTER_Z 90
#define CHAR_LETTER_A_LOWER 97
#define CHAR_LETTER_Z_LOWER 122
#define CHAR_NUMBER_0 48
#define CHAR_NUMBER_9 57

#ifdef USE_LOWER_CASE
  #define FONT_EXCLAMATION_INDEX 62
  #define FONT_PERIOD_INDEX 63
  #define FONT_PLUS_INDEX 64
  #define FONT_COMMA_INDEX 65
  #define FONT_COLON_INDEX 66
  #define FONT_NUMBER_INDEX 52
#else
  #define FONT_EXCLAMATION_INDEX 36
  #define FONT_PERIOD_INDEX 37
  #define FONT_PLUS_INDEX 38
  #define FONT_COMMA_INDEX 39
  #define FONT_COLON_INDEX 40
  #define FONT_NUMBER_INDEX 26
#endif

const uint8_t PROGMEM font_images[] = {
3, 8,
0x1F, 0x05, 0x1F, // A
0x1F, 0x15, 0x1B, 
0x1F, 0x11, 0x11, 
0x1F, 0x11, 0x0E, 
0x1F, 0x15, 0x11, 
0x1F, 0x05, 0x01, 
0x1F, 0x11, 0x1D, 
0x1F, 0x04, 0x1F, 
0x00, 0x1F, 0x00, 
0x10, 0x10, 0x1F, 
0x1F, 0x04, 0x1B, 
0x1F, 0x10, 0x10, 
0x1F, 0x06, 0x1F, 
0x1F, 0x01, 0x1F, 
0x1F, 0x11, 0x1F, 
0x1F, 0x05, 0x07, 
0x1F, 0x31, 0x1F, 
0x1F, 0x05, 0x1B,  
0x17, 0x15, 0x1D,  
0x01, 0x1F, 0x01,  
0x1F, 0x10, 0x1F,  
0x0F, 0x10, 0x0F,  
0x1F, 0x0C, 0x1F,  
0x1B, 0x04, 0x1B,  
0x07, 0x1C, 0x07,  
0x19, 0x15, 0x13, // Z
#ifdef USE_LOWER_CASE
0x0C,	0x12,	0x1E,	// a
0x1F,	0x12,	0x0C,	
0x1E,	0x12,	0x12,	
0x0C,	0x12,	0x1F,	
0x0C,	0x1A,	0x14,	
0x04,	0x1F,	0x05,	
0x2E,	0x2A,	0x1E,	
0x1F,	0x02,	0x1C,	
0x00,	0x1D,	0x00,	
0x20,	0x1D,	0x00,	
0x1F,	0x04,	0x1A,	
0x01,	0x1F,	0x00,	
0x1E,	0x04,	0x1E,	
0x1E,	0x02,	0x1E,	
0x1E,	0x12,	0x1E,	
0x3E,	0x12,	0x0C,	
0x0C,	0x12,	0x3E,	
0x1E,	0x02,	0x06,	
0x14,	0x12,	0x0A,	
0x02,	0x0F,	0x12,	
0x1E,	0x10,	0x1E,	
0x0E,	0x10,	0x0E,	
0x1E,	0x08,	0x1E,	
0x1A,	0x04,	0x1A,	
0x2E,	0x28,	0x1E,	
0x1A,	0x12,	0x16, // z
#endif
0x1F, 0x11, 0x1F, // 0  
0x02, 0x1F, 0x00,
0x1D, 0x15, 0x17,  
0x11, 0x15, 0x1F,  
0x07, 0x04, 0x1F,  
0x17, 0x15, 0x1D,  
0x1F, 0x15, 0x1D,  
0x01, 0x01, 0x1F,  
0x1F, 0x15, 0x1F,  
0x17, 0x15, 0x1F, // 9
0x00, 0x17, 0x00, // !
0X00, 0x10, 0x00, // .
0X08, 0x1c, 0x08, // +
0X20, 0x18, 0x00, // ,
0X00, 0x14, 0x00, // :
};


Font3x5::Font3x5(uint8_t lineSpacing) {

  _lineHeight = lineSpacing;
  _letterSpacing = 1;

  _cursorX = _cursorY = _baseX = 0;
  _textColor = 1;

}

size_t Font3x5::write(uint8_t c) {

  if (c == '\n')          { _cursorX = _baseX; _cursorY += _lineHeight; }
  else if (c == ' ')      { _cursorX += 2; }
  else if (c == '~')      { _cursorX += 1; }
  else {

    printChar(c, _cursorX, _cursorY);
    _cursorX += FONT3X5_WIDTH + _letterSpacing;

  }

  return 1;


}

void Font3x5::printChar(const char c, const int8_t x, int8_t y) {

  int8_t idx = -1;

  ++y;

  switch (c) {
    
    case CHAR_LETTER_A ... CHAR_LETTER_Z:
      idx = c - CHAR_LETTER_A;
      break;

#ifdef USE_LOWER_CASE    
    case CHAR_LETTER_A_LOWER ... CHAR_LETTER_Z_LOWER:
      idx = c - CHAR_LETTER_A_LOWER + 26;
      break;
#endif

    case CHAR_NUMBER_0 ... CHAR_NUMBER_9:
      idx = c - CHAR_NUMBER_0 + FONT_NUMBER_INDEX;
      break;
      
    case CHAR_EXCLAMATION:
      idx = FONT_EXCLAMATION_INDEX;
      break;
      
    case CHAR_PERIOD:
      idx = FONT_PERIOD_INDEX;
      break;
      
    case CHAR_PLUS:
      idx = FONT_PLUS_INDEX;
      break;

    case CHAR_COMMA:
      idx = FONT_COMMA_INDEX;
      break;

    case CHAR_COLON:
      idx = FONT_COLON_INDEX;
      break;

  }

  if (idx > -1) {
    
    if (_textColor == WHITE) {
      Sprites::drawSelfMasked(x, y, font_images, idx);
    }
    else {
      Sprites::drawErase(x, y, font_images, idx);
    }

  }

}

void Font3x5::setCursor(const int8_t x, const int8_t y) {
  _cursorX = _baseX = x;
  _cursorY = y;
}

void Font3x5::setTextColor(const uint8_t color){
  _textColor = color;
}

void Font3x5::setHeight(const uint8_t color){
  _lineHeight = color;
}
