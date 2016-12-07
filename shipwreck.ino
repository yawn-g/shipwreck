#include <SPI.h>
#include <Gamebuino.h>

Gamebuino gb;

//
// useless vars
int useless1 = 0;
int useless2 = 0;
int useless3 = 0;
int useless4 = 0;
int useless5 = 0;
int useless6 = 0;
int useless7 = 0;
int useless8 = 0;
int useless9 = 0;
int useless10 = 0;
int useless11 = 0;
/*int useless12 = 0;
int useless13 = 0;
int useless14 = 0;
int useless15 = 0;
int useless16 = 0;
int useless17 = 0;
int useless18 = 0;
int useless19 = 0;
int useless20 = 0;
int useless21 = 0;
int useless22 = 0;
int useless23 = 0;
int useless24 = 0;
int useless25 = 0;
int useless26 = 0;
int useless27 = 0;
int useless28 = 0;
int useless29 = 0;
int useless30 = 0;
int useless31 = 0;*/
int useless32 = 0;
int useless33 = 0;
int useless34 = 0;
int useless35 = 0;
int useless36 = 0;
int useless37 = 0;
int useless38 = 0;
int useless39 = 0;

bool playing;
bool game_over = false; // playing remains true until anim is over
byte game_over_anim_fc; // frame counter for game over anim
byte nb_shots[2];
byte last_cur_x[2] = { 4, 4 };
byte last_cur_y[2] = { 4, 4 };

// text variables
char p_name[2][9] = { "Yann", "Player2" };

// text constants
const char boat_name[5][9] = { "Cruiser", "Submarin", "Destroyr", "Bat.Ship", "Carrier" };

// boat positions x, y, dir (255 for x means not placed)
byte boat_pos[2][5][3] =
{
  {
    { 255, 0, 0 },    // cruiser
    { 255, 0, 0 },
    { 255, 0, 0 },
    { 255, 0, 0 },
    { 255, 0, 0 }     // carrier
  },
  {
    { 255, 0, 0 },
    { 255, 0, 0 },
    { 255, 0, 0 },
    { 255, 0, 0 },
    { 255, 0, 0 }
  }
};

/* 
 *  shots maps
 *  255 = not aimed at
 *  254 = miss
 *  0-5 = hit boat number
 */
// 255 = not shot at
byte shots[2][9][9] =
{
  {
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 }   
  },
  {
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 },
    { 255,255,255,255,255,255,255,255,255 }    
  }
};

// graphics
const byte carrier[] PROGMEM = {32,4,
B11111111,B11111111,B11111111,B00000000,
B11111111,B11111111,B11111111,B11000000,
B11111111,B11111111,B11111111,B11000000,
B11111111,B11111111,B11111111,B00000000,
};
const byte submarine[] PROGMEM = {16,4,
B01111111,B10000000,
B11111111,B11100000,
B11111111,B11100000,
B01111111,B10000000,
};
const byte cruiser[] PROGMEM = {8,4,
B01111000,
B01111100,
B01111100,
B01111000,
};
const byte battle_ship[] PROGMEM = {24,4,
B01111111,B11111111,B11100000,
B11111111,B11111111,B11111000,
B11111111,B11111111,B11111000,
B01111111,B11111111,B11100000,
};
const byte destroyer[] PROGMEM = {16,4,
B01111111,B11111100,
B11111111,B11111111,
B11111111,B11111111,
B01111111,B11111100,
};

// logo
const byte logo[] PROGMEM = {64,30,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000100,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000011,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000100,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000001,B00000110,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000111,B00001011,B10000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000111,B01111101,B00001110,B00000000,B00000000,B00000000,
B00000000,B00000010,B10001111,B11111111,B11111000,B00010010,B00110001,B11111111,
B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111100,
B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111000,
B00001111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111000,
B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,B01010101,
B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,B10101010,
};

// clouds
const byte clouds[] PROGMEM = {88,10,
B01000000,B00000000,B00000001,B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B11000000,B00001000,
B01000000,B00000000,B00000000,B10000000,B00000100,B00000000,B00000000,B00000000,B00000001,B00110000,B00110000,
B00100000,B00000000,B00000000,B01111000,B01111100,B00000000,B00000000,B00000000,B00000001,B00001111,B11000000,
B00010000,B00000000,B00000000,B01000111,B10000010,B00000000,B00000000,B00000000,B00000010,B00000000,B00000000,
B00001100,B00000000,B00000001,B10000000,B00000001,B10000000,B00000000,B00000000,B00001100,B00000000,B00000000,
B00000011,B10000000,B00001110,B00000000,B00000000,B01000000,B00000000,B00000000,B00010000,B00000000,B00000000,
B00000000,B01111111,B11110000,B00000000,B00000000,B00111000,B00000000,B00000000,B11100000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000111,B10000000,B00001111,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B11110000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};
const byte cloudsW[] PROGMEM = {88,10,
B00111111,B11111111,B11111110,B11111111,B11110111,B11111111,B11111111,B11111111,B11111111,B00111111,B11110000,
B00111111,B11111111,B11111111,B01111111,B11111011,B11111111,B11111111,B11111111,B11111110,B00001111,B11000000,
B00011111,B11111111,B11111111,B10000111,B10000011,B11111111,B11111111,B11111111,B11111110,B00000000,B00000000,
B00001111,B11111111,B11111111,B10000000,B00000001,B11111111,B11111111,B11111111,B11111100,B00000000,B00000000,
B00000011,B11111111,B11111110,B00000000,B00000000,B01111111,B11111111,B11111111,B11110000,B00000000,B00000000,
B00000000,B01111111,B11110000,B00000000,B00000000,B00111111,B11111111,B11111111,B11100000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000111,B11111111,B11111111,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B11110000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};
const byte clouds2[] PROGMEM = {88,14,
B00100001,B00000000,B00000001,B00000000,B00100000,B00000000,B00100000,B00000000,B10000000,B00000000,B00000100,
B00100001,B00000000,B00000001,B00000000,B01000000,B00000000,B01000000,B00000000,B10000000,B00000000,B00000100,
B00010000,B10000000,B00000000,B10000000,B01000000,B00000000,B01000000,B00000000,B10000000,B00000000,B00000010,
B00001000,B01000000,B00000000,B01000000,B01000000,B00000000,B01000000,B00000000,B01000000,B00000000,B00000001,
B10000111,B10110000,B00000000,B00110000,B00100000,B00000000,B00100000,B00000000,B01000000,B00000000,B00010000,
B01000000,B11111111,B00000000,B00111110,B00010000,B00000000,B00100000,B00000000,B00100000,B00000000,B00101000,
B00111111,B01010000,B11111111,B11100001,B11111000,B00000000,B00010000,B00000000,B00011000,B00000000,B11000111,
B00000000,B00100000,B11001100,B01000000,B00000111,B00000000,B01111100,B00000000,B00110110,B00000011,B11000000,
B00000000,B00110000,B01110000,B01000000,B00011100,B11111111,B10100011,B00000000,B11000001,B11111100,B00100000,
B00000000,B01001111,B10001100,B01000000,B00100000,B00000100,B00100000,B11111111,B00000001,B00000000,B00100000,
B00000000,B01000000,B00000011,B11100000,B00010000,B00000100,B00011000,B00110001,B10000011,B11000000,B11000000,
B11100011,B10000000,B00000000,B00011100,B00111100,B00011011,B00110111,B11000000,B01111100,B00111111,B00111100,
B00011100,B00000000,B00000000,B00000011,B11000011,B11100000,B11000000,B00000000,B00000000,B00000000,B00000011,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};

// game text bitmaps
const byte game_text[] PROGMEM = {56,11,
B00001111,B11000000,B01100000,B01111100,B00011111,B01111111,B11000000,
B00111000,B11000000,B11100000,B00111100,B00111100,B00111000,B11000000,
B01100000,B01000000,B11110000,B00111100,B00111100,B00111000,B01000000,
B01100000,B01000001,B11110000,B00101100,B01111100,B00110000,B00000000,
B11100000,B00000001,B00110000,B00101110,B01011000,B00110011,B00000000,
B11100000,B00000010,B00110000,B01101110,B10011000,B01111110,B00000000,
B11000111,B11100111,B11110000,B01101110,B10011000,B01110010,B00000000,
B11100001,B10000100,B00111000,B01000111,B00111000,B01110000,B00000000,
B11100001,B10001100,B00111000,B01000111,B00111000,B01100000,B10000000,
B01110011,B10011000,B00111000,B11000110,B00111000,B11100001,B10000000,
B00111111,B00111100,B01111101,B11100110,B11111101,B11111111,B10000000,
};
const byte game_outline[] PROGMEM = {56,13,
B00001111,B11110000,B01111000,B01111111,B00011111,B11111111,B11110000,
B00111000,B00010000,B11001000,B01000001,B00110000,B01000000,B00010000,
B01100011,B10010000,B10001100,B01100001,B00100001,B11100011,B10010000,
B01001110,B11010001,B10000100,B00100001,B01100001,B00100010,B11010000,
B11001000,B01010001,B00000100,B00101001,B11000001,B00100111,B11110000,
B10001000,B01110011,B01100100,B01101000,B11010011,B01100110,B01000000,
B10001111,B11111110,B11100100,B01001000,B10110010,B01000000,B11000000,
B10011100,B00001100,B00000110,B01001000,B10110010,B01000110,B10000000,
B10001111,B00111101,B11100010,B01011100,B01100010,B01000111,B11100000,
B10001111,B00111001,B00100010,B11010100,B01100010,B11001101,B10100000,
B11000110,B00110011,B01100011,B10011100,B11100011,B10001111,B00100000,
B01100000,B01100001,B01000001,B00001100,B10000001,B00000000,B00100000,
B00111111,B11111111,B01111111,B11111111,B11111111,B11111111,B11100000,
};
// over txt bitmaps
const byte over_text[] PROGMEM = {48,11,
B00001111,B10001111,B10001111,B01111111,B11001111,B11110000,
B00110001,B11000111,B00000110,B00111000,B11000111,B00111000,
B01110000,B11100111,B00001100,B00111000,B01000111,B00111000,
B01100000,B11100111,B00001000,B00110000,B00000110,B00111000,
B11100000,B11100011,B00011000,B00110011,B00000110,B00110000,
B11100000,B11100011,B00110000,B01111110,B00001111,B11100000,
B11100000,B11100011,B00110000,B01110010,B00001110,B11000000,
B11100000,B11000011,B11100000,B01110000,B00001100,B11100000,
B11100001,B11000011,B11000000,B01100000,B10001100,B01100000,
B01100011,B10000001,B11000000,B11100001,B10011100,B01110000,
B00111110,B00000001,B10000001,B11111111,B10111110,B00111100,
};
const byte over_outline[] PROGMEM = {48,13,
B00001111,B11101111,B11101111,B11111111,B11111111,B11111100,
B00111000,B00111000,B00101000,B01000000,B00011000,B00000110,
B01100111,B00011100,B01101100,B11100011,B10011100,B01100010,
B01000101,B10001100,B01001001,B10100010,B11010100,B01100010,
B11001100,B10001100,B01011011,B00100111,B11110100,B11100010,
B10001000,B10001110,B01110010,B01100110,B01001100,B11100110,
B10001000,B10001010,B01100110,B01000000,B11001000,B00001100,
B10001000,B10001010,B01100100,B01000110,B10001000,B10011000,
B10001001,B10011010,B00001100,B01000111,B11101001,B10001000,
B10001011,B00010010,B00011000,B11001101,B10111001,B11001100,
B11001110,B00110011,B00010001,B10001111,B00110001,B11000110,
B01100000,B11100001,B00110001,B00000000,B00100000,B11100010,
B00111111,B10000001,B11100001,B11111111,B11111111,B10111110,
};

/*
 * 
 *        SETUP ==================================================================
 * 
 */

void setup(){
  Serial.begin(9600);
  gb.begin();
  gb.titleScreen(F("Shipwreck"), logo);
  //gb.pickRandomSeed();
  gb.battery.show = false;
}

/*
 * 
 *        FONCTIONS
 * 
 */


void title_screen() {
  gb.sound.playCancel();
  gb.titleScreen(F("Shipwreck"), logo);
  gb.battery.show = false;
}

void draw_board() {
  gb.display.fillScreen(BLACK);
  // text zone
  gb.display.setColor(WHITE);
  gb.display.fillRect(49, 7, 34, 40);
  gb.display.cursorY = 1;
  // board
  gb.display.drawFastHLine(1, 0, 46);
  gb.display.drawFastVLine(47, 1, 46);
  gb.display.drawFastHLine(1, 47, 46);
  gb.display.drawFastVLine(0, 1, 46);
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      gb.display.drawPixel(x*5 + 1, y*5 + 1);
    }
  }
}

void draw_boat(byte x, byte y, byte b, bool dir, byte c) {
  gb.display.setColor(c);
  if (dir) {
    switch (b) {
      case 4:
        gb.display.drawBitmap(x*5 + 1, y*5 + 2 , carrier);
        break;
      case 3:
        gb.display.drawBitmap(x*5 + 1, y*5 + 2 , battle_ship);
        break;
      case 2:
        gb.display.drawBitmap(x*5 + 1, y*5 + 2 , destroyer);
        break;
      case 1:
        gb.display.drawBitmap(x*5 + 1, y*5 + 2 , submarine);
        break;
      case 0:
        gb.display.drawBitmap(x*5 + 1, y*5 + 2 , cruiser);
    }
  }
  else {
    switch (b) {
      case 4:
        gb.display.drawBitmap(x*5 + 2, y*5 + 1, carrier, ROTCW, NOFLIP);
        break;
      case 3:
        gb.display.drawBitmap(x*5 + 2, y*5 + 1, battle_ship, ROTCW, NOFLIP);
        break;
      case 2:
        gb.display.drawBitmap(x*5 + 2, y*5 + 1, destroyer, ROTCW, NOFLIP);
        break;
      case 1:
        gb.display.drawBitmap(x*5 + 2, y*5 + 1, submarine, ROTCW, NOFLIP);
        break;
      case 0:
        gb.display.drawBitmap(x*5 + 2, y*5 + 1, cruiser, ROTCW, NOFLIP);
    }
  }
}

void draw_boats(bool p) {
  for (byte b = 0; b <5; b++) {
    byte x = boat_pos[p][b][0];
    byte y = boat_pos[p][b][1];
    bool dir = boat_pos[p][b][2];
    if (x < 255) {
      draw_boat(x, y, b, dir, GRAY);
    }
  }
}

/*
 *  DRAW SHOTS
 *  ==========
 *  map values:
 *  boat number when hit
 *  255 when miss
 */
void draw_shots(bool p) {
  for (byte y = 0; y < 9; y++) {
    for (byte x = 0; x < 9; x++) {
      byte shot = shots[p][x][y];
      // if hit
      if (shot >=0 && shot < 5)
        if(!sunk(-p+1, shot)) {
          gb.display.setColor(WHITE);
          gb.display.fillRect(x*5 + 2, y*5 + 2, 4, 4);
        }
        else {
          byte x, y;
          bool dir;
          x = boat_pos[-p+1][shot][0];
          y = boat_pos[-p+1][shot][1];
          dir = boat_pos[-p+1][shot][2];
          draw_boat(x, y, shot, dir, GRAY);
        }
      // if miss
      else if (shot == 254) {
        gb.display.setColor(WHITE);
        gb.display.fillRect(x*5 + 3, y*5 + 3, 2, 2);
      }
    }
  }
}

byte check_pos(bool p, byte cur_x, byte cur_y) {
  // default value meaning empty area
  byte output = 255;
  // for each boat
  for (byte b = 0; b < 5; b++) {
    byte b_x = boat_pos[p][b][0];
    byte b_y = boat_pos[p][b][1];
    bool dir = boat_pos[p][b][2];
    if ((cur_x >= b_x && cur_x < b_x + b+1 && cur_y == b_y && dir == true) ||
        (cur_y >= b_y && cur_y < b_y + b+1 && cur_x == b_x && dir == false)) {
          output = b;
    }
  }
  return output;
}

void reset_game() {
  byte v = 0;

  game_over = false;
  game_over_anim_fc = 0;
  
  for (byte p = 0; p < 2; p++) {
    // reset boat positions
    for (byte b = 0; b < 5; b++) {
      for (byte n = 0; n < 3; n++) {
        if (n == 0) v = 255;
        else v = 0;
        boat_pos[p][b][n] = v;
      }
    }

    // reset shots
    for (byte x = 0; x < 9; x++) {
      for (byte y = 0; y < 9; y++) {
        shots[p][x][y] = 255;
      }
    }
    nb_shots[p] = 0;
    last_cur_x[p] = 4;
    last_cur_y[p] = 4;
  }
}

bool sunk(bool p, byte b) {
  bool output = true;
  byte x = boat_pos[p][b][0];
  byte y = boat_pos[p][b][1];
  bool dir = boat_pos[p][b][2];
  for (byte i = 0; i <= b; i++) {
    // if horizontal
    if (dir) {
      if (i > 0) x++;
    }
    else {
      if (i > 0) y++;
    }
    if (shots[-p+1][x][y] != b) output = false;
  }
  return output;
}

// GUI
// text
void print_in_zone(char t[9]) {
  gb.display.cursorX = 50;
  gb.display.println(t);
}
// text + number
void print_in_zone_with_number(char t[9], byte n) {
  gb.display.cursorX = 50;
  gb.display.print(t);
  gb.display.println(n);
}

// anims
void update_game_over_anim(bool p) {
  
  // bitmaps x pos
  byte gx, ox;
  // rects y pos & height
  byte ry, rh;

  // if anim finished
  if (game_over_anim_fc > 41) {
    playing = false;
  }
  
  // "game over" bitmaps (drawn on top)
  if (game_over_anim_fc <= 17) {
    gx = 84 - game_over_anim_fc*4;
    ox = -50 + game_over_anim_fc*4;
  }
  else {
    gx = 16;
    ox = 18;
  }
  gb.display.setColor(WHITE);
  gb.display.drawBitmap(gx-1, 7, game_outline);
  gb.display.drawBitmap(ox-1, 22, over_outline);
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(gx, 8, game_text);
  gb.display.drawBitmap(ox, 23, over_text);

  // and the winner is
  gb.display.setColor(WHITE);
  gb.display.cursorX = 2;
  gb.display.cursorY = 41;
  gb.display.print(p_name[p]);
  gb.display.print(" wins!");

  game_over_anim_fc++;
}


/*
 * 
 *        LOOP ====================================================================
 * 
 */

void loop() {

  reset_game();

  /*
   * 
   *          BOAT SETUP
   * 
   */

  // for each player
  for (byte p = 0; p < 2; p++) {
    byte b = 4;
    bool boat_rot = false;
    byte cur_x = 4;
    byte cur_y = 4;
    // for each boat, starting from biggest
    while (b >= 0) {
      if (gb.update()) {
        draw_board();
        // draw already placed boats
        draw_boats(p);
        // display text
        gb.display.setColor(WHITE);
        print_in_zone(p_name[p]);
        gb.display.setColor(BLACK);
        print_in_zone("Place");
        print_in_zone("your");
        print_in_zone(boat_name[b]);
        gb.display.println();
        print_in_zone("A: Rotat");
        print_in_zone("B: Place");


        // BUTTON detection
        // B: rotate
        if (gb.buttons.pressed(BTN_B)) {
          // toggle boolean
          boat_rot = -boat_rot + 1;
          // move cursor if boat out of screen
          if (cur_x + b > 8 && boat_rot == true) cur_x = 8-b;
          if (cur_y + b > 8 && boat_rot == false) cur_y = 8-b;
        }
        // A: place
        if (gb.buttons.pressed(BTN_A)) {
          // test against player boat map
          bool possible = true;
          // for each cell of the boat
          for (byte i = 0; i < b+1; i++) {
            byte x_offset, y_offset;
            if (boat_rot) {
              x_offset = i;
              y_offset = 0;
            }
            else {
              x_offset = 0;
              y_offset = i;
            }
            // check
            if (check_pos(p, cur_x + x_offset, cur_y + y_offset) < 255) {
              possible = false;
              break;
            }
          }
          if (possible == true) {
            boat_pos[p][b][0] = cur_x;
            boat_pos[p][b][1] = cur_y;
            boat_pos[p][b][2] = boat_rot;
            // next ship
            gb.sound.playOK();
            // exit loop to avoid byte to become negative
            if (b == 0) break;
            b--;
          }
          else {
            gb.sound.playCancel();
            gb.popup(F("Can't overlap boats"), 20);
          }
        }
        
        // arrows
        if (gb.buttons.pressed(BTN_UP)) {
          if (cur_y > 0) cur_y--;
          else gb.sound.playCancel();
        }
        if (gb.buttons.pressed(BTN_DOWN)) {
          if (cur_y < 8 - (b)*(-boat_rot+1))
            cur_y++;
          else gb.sound.playCancel();
        }
        if (gb.buttons.pressed(BTN_LEFT)) {
          if (cur_x > 0) cur_x--;
          else gb.sound.playCancel();
        }
        if (gb.buttons.pressed(BTN_RIGHT)) {
          if (cur_x < 8 - (b)*boat_rot)
            cur_x++;
          else gb.sound.playCancel();
        }

        // B: toggle night/day
        
        // C: pause / leave
        if(gb.buttons.pressed(BTN_C)) title_screen();

        // draw boat
        gb.display.setColor(WHITE);
        byte x_offset = 0;
        byte y_offset = 0;
        for (byte i = 0; i <= b; i++) {
           if (boat_rot) {
            gb.display.drawRect((cur_x+i)*5 + 3, cur_y*5 + 3, 2, 2);
          } else {
            gb.display.drawRect(cur_x*5 + 3, (cur_y+i)*5 + 3, 2, 2);
          }
        }
      }
    }
  }

  /*
   *    GGGGG    AAA    MM MM   EEEEE
   *    G       A   A   M M M   E
   *    G  GG   AAAAA   M   M   EEE
   *    G   G   A   A   M   M   E
   *    GGGGG   A   A   M   M   EEEEE
   */

  playing = true;
  while (playing) {

    // for each player
    for (byte p = 0; p < 2; p++) {
      
      // don't run for p2 if p1 already won
      if (playing == false) break;
      
      /*
       *          ANIM SCREEN
       *          ANIM SCREEN
       *          ANIM SCREEN
       */
      int clouds_x, clouds2_x;
      byte b_y_offset = 0;
      char press_a[4] = { 21, 16, ' ' }; // "a> "
      byte interval = 1; // number of frames to count before moving clouds
      int last_move = gb.frameCount;
      int last_arrow_move = last_move;
      byte anim_frames = 15;
      byte anim_frame = anim_frames;
      
      // step 0: previous shot text, step 1: shot anim, step 2, next player's turn text
      for (byte steps = 0; steps < 3; steps++) {
        
        // skip first 2 steps if no previous shot
        if (nb_shots[-p+1] == 0) steps = 2;
        
        bool waiting = true;
        while (waiting) {
          if (gb.update()) {
            
            // move clouds
            int fc = gb.frameCount;
            if (fc - last_move > interval) {
              last_move = fc;
              clouds_x--;
              if (clouds_x < -87) clouds_x = 0;
              // back clouds
              if (clouds_x % 2 == 0) {
                clouds2_x--;
                if (clouds2_x < -87) clouds2_x = 0;
              }
            }
            
            // move arrow
            if (fc - last_arrow_move > 10) {
              last_arrow_move = fc;
              if (press_a[1] == 16) {
                press_a[1] = ' ';
                press_a[2] = 16; // right arrow
              } else {
                press_a[1] = 16;
                press_a[2] = ' ';
              }
            }
            
            // draw clouds
            gb.display.setColor(GRAY);
            gb.display.drawBitmap(clouds2_x, 0, clouds2);
            gb.display.drawBitmap(clouds2_x + 88, 0, clouds2);
            gb.display.setColor(WHITE);
            gb.display.drawBitmap(clouds_x, 0, cloudsW);
            gb.display.drawBitmap(clouds_x + 88, 0, cloudsW);
            gb.display.setColor(BLACK);
            gb.display.drawBitmap(clouds_x, 0, clouds);
            gb.display.drawBitmap(clouds_x + 88, 0, clouds);
            
            // draw text
            if (!game_over) {
              gb.display.cursorX = 14;
              gb.display.cursorY = 15;
              if (steps == 2) {
                gb.display.print(p_name[p]);
                gb.display.print("'s ");
                gb.display.print("turn!");
              } else {
                gb.display.print(p_name[-p+1]);
                gb.display.print("'s ");
                gb.display.println("shot:");
              }   
            }

            // boat & water
            gb.display.drawBitmap(10, 19 + b_y_offset, logo);
            gb.display.setColor(GRAY);
            gb.display.fillRect(0, 32, 84, 16);
            gb.display.setColor(WHITE);
            gb.display.cursorX = 70;
            gb.display.cursorY = 41;
            gb.display.print(press_a);
  
            /* change speed :)
            if (gb.buttons.pressed(BTN_LEFT) && interval < 10) interval++;
            if (gb.buttons.pressed(BTN_RIGHT) && interval > 1) interval--;*/

            // resolve and play anim
            if (steps == 1){
              // check last shot of other player
              // if exists
              if (nb_shots[-p+1] >= 1) {
                int check_shot = check_pos(p, last_cur_x[-p+1], last_cur_y[-p+1]);
                // if hit, check_shot <= 5 (any boat id)
                if (check_shot < 255) {
                  // if 1st frame of anim then show popup
                  if (anim_frame == anim_frames) {
                    // if sunk
                    if (sunk(p, check_shot)) {
                      switch (check_shot) {
                        case 0: gb.popup(F("Cruiser sunk!"), 20); break;
                        case 1: gb.popup(F("Submarine sunk!"), 20); break;
                        case 2: gb.popup(F("Destroyer sunk!"), 20); break;
                        case 3: gb.popup(F("Battle ship sunk!"), 20); break;
                        case 4: gb.popup(F("Carrier sunk!"), 20); break;
                      }
                      // check if all other boats sunk too
                      game_over = true;
                      for (int i = 0; i < 5; i++) {
                        if (!sunk(p, i)) {
                          game_over = false;
                        }
                      }
                      if (game_over) playing = false;
                    }
                    // if hit but not sunk
                    else gb.popup(F("HIT!!!"), 15);
                  }
                  // other than the 1st frame
                  else {
                    if (sunk(p, check_shot)) {
                      // sink
                      b_y_offset++;
                    }
                  }
                }
                // if miss
                else {
                  // if 1st frame of anim shot popup
                  if (anim_frame == anim_frames) {
                    gb.sound.playCancel();
                    gb.popup(F("Miss..."), 15);
                  }
                }
                if (anim_frame == 0) waiting = false;
                else anim_frame--;
              }              
            }
            // update game over anim
            if (game_over) update_game_over_anim(-p+1);
  
            // buttons
            // proceed if button A pressed
            if (gb.buttons.pressed(BTN_A)) {
              waiting = false;
            }
            // allow exit
            if (gb.buttons.pressed(BTN_C)) title_screen();
          }
        }
      }

      /*
       *    SHOOT
       */
      // restore last shooting cursor position
      byte cur_x = last_cur_x[p];
      byte cur_y = last_cur_y[p];
      
      // initalize shooting phase
      bool waiting_for_shot = true;
      
      // skip shot if game is over
      if (game_over) waiting_for_shot = false;
      while (waiting_for_shot) {
        if (gb.update()) {
          draw_board();
          draw_shots(p);
        
          // buttons
          if (gb.buttons.pressed(BTN_UP)) {
            if (cur_y > 0) cur_y--;
            else gb.sound.playCancel();
          }
          if (gb.buttons.pressed(BTN_DOWN)) {
            if (cur_y < 8)
              cur_y++;
            else gb.sound.playCancel();
          }
          if (gb.buttons.pressed(BTN_LEFT)) {
            if (cur_x > 0) cur_x--;
            else gb.sound.playCancel();
          }
          if (gb.buttons.pressed(BTN_RIGHT)) {
            if (cur_x < 8)
              cur_x++;
            else gb.sound.playCancel();
          }

          // A: shoot
          if (gb.buttons.pressed(BTN_A)) {
            // check if already shot
            if (shots[p][cur_x][cur_y] != 255) {
              gb.sound.playCancel();
              gb.popup(F("Already shot there!"), 20);
            }
            else {
              gb.sound.playTick();
              nb_shots[p]++;
              byte target = check_pos(-p+1, cur_x, cur_y);
              if (target < 255) {
                shots[p][cur_x][cur_y] = target; // shots map: set target value when hit
              } else {
                shots[p][cur_x][cur_y] = 254; // shots map: set 254 when miss
              } 
              waiting_for_shot = false;
              // save cursor position
              last_cur_x[p] = cur_x;
              last_cur_y[p] = cur_y;
            }
          }
          
          // C: pause / leave
          if(gb.buttons.pressed(BTN_C)) title_screen();

          // draw aim
          gb.display.setColor(GRAY);
          gb.display.drawFastVLine(cur_x*5 + 3, cur_y*5 + 3 - 6, 13);
          gb.display.drawFastHLine(cur_x*5 + 3 - 6, cur_y*5 + 3, 13);
          gb.display.setColor(WHITE);
          gb.display.drawCircle(cur_x*5 + 3, cur_y*5 + 3, 7);
          gb.display.drawRect(cur_x*5 + 3, cur_y*5 + 3, 2, 2);

          // text (must be drawn after aim)
          gb.display.setColor(BLACK);
          gb.display.fillRect(49, 0, 34, 7);
          gb.display.setColor(WHITE);
          gb.display.fillRect(49, 8, 34, 40);
          print_in_zone(p_name[p]);
          gb.display.setColor(BLACK);
          print_in_zone_with_number("Shot ", nb_shots[p]+1);

          // mini map
          for (byte b = 0; b <5; b++) {
            byte x = boat_pos[p][b][0];
            byte y = boat_pos[p][b][1];
            bool dir = boat_pos[p][b][2];
            if (x < 255) {
              if (dir == true) gb.display.drawFastHLine(72 + x, 37 + y, b+1);
              else gb.display.drawFastVLine(72 + x, 37 + y, b+1);
            }
          }
        }                   // end if gb.update()
      }                     // end while waiting for shoot
    }                       // end for each player
  }                         // end while not over
}                           // end loop
