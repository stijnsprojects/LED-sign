#include "FastLED.h"

#define DATA_PIN 2
#define COLOR_ORDER GRB
// total number of leds
#define NUM_LEDS 39
#define LED_TYPE WS2812B
// number of columns
#define COLUMNS 21
// number of rows
#define ROWS 4
// number of letters
#define LETTERS 5
// max leds in one letter
#define MAXLETTER 9
#define RAINBOWLENGTH 23
#define RANDLISTS 10
CRGB leds[NUM_LEDS];

long prevcolor;

// button pin
const int button = 0;

// animation settings
int modes = 39;
int lastpressed = 0;
int animationmode = 1;

// min, default and max brightness
int minbrightness = 30;
int brightness = 118;
int maxbrightness = 250;

// min, default and max speed (higher is slower)
int minspeed = 23;
int SPEED = 12;
int maxspeed = 6;

uint8_t reverseleds[NUM_LEDS] = {38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

uint8_t randomsequence[][NUM_LEDS] = {
  {18, 1, 0, 2, 12, 31, 24, 5, 6, 27, 4, 32, 28, 19, 21, 7, 34, 38, 37, 35, 25, 20, 8, 26, 3, 14, 11, 17, 33, 15, 13, 29, 23, 16, 10, 9, 22, 36, 30},
  {21, 28, 22, 0, 30, 34, 33, 16, 3, 8, 1, 27, 26, 5, 14, 31, 4, 18, 24, 37, 6, 32, 13, 19, 2, 23, 17, 15, 11, 25, 35, 9, 10, 12, 38, 20, 36, 29, 7},
  {11, 1, 18, 22, 31, 38, 2, 7, 37, 33, 36, 23, 21, 12, 5, 20, 32, 15, 6, 27, 29, 26, 16, 35, 25, 28, 30, 19, 8, 0, 10, 34, 9, 17, 24, 3, 4, 13, 14},
  {26, 27, 33, 38, 29, 24, 9, 12, 21, 31, 5, 3, 36, 6, 18, 0, 11, 37, 13, 1, 34, 25, 22, 4, 19, 2, 16, 20, 17, 7, 15, 14, 28, 32, 10, 35, 23, 8, 30},
  {34, 28, 2, 22, 12, 23, 5, 38, 30, 25, 27, 10, 8, 17, 26, 7, 37, 31, 35, 36, 19, 32, 13, 15, 1, 29, 24, 4, 11, 33, 6, 3, 0, 9, 16, 18, 14, 20, 21},
  {10, 4, 9, 1, 2, 17, 27, 36, 20, 23, 11, 3, 31, 19, 6, 28, 8, 37, 22, 18, 13, 30, 33, 16, 14, 21, 26, 24, 15, 35, 38, 25, 29, 5, 34, 7, 32, 12, 0},
  {32, 11, 3, 24, 38, 13, 20, 5, 2, 12, 16, 14, 17, 4, 10, 27, 28, 29, 0, 1, 23, 35, 33, 15, 37, 7, 9, 8, 25, 34, 22, 18, 31, 19, 6, 21, 26, 36, 30},
  {30, 2, 0, 28, 12, 7, 3, 27, 1, 18, 10, 11, 25, 34, 38, 6, 31, 24, 15, 16, 22, 23, 4, 29, 5, 36, 26, 8, 35, 21, 32, 19, 13, 14, 20, 33, 17, 9, 37},
  {37, 10, 14, 35, 21, 33, 12, 32, 8, 9, 20, 25, 4, 13, 0, 36, 29, 27, 34, 26, 17, 24, 3, 30, 7, 38, 28, 15, 11, 6, 5, 18, 16, 22, 19, 2, 23, 1, 31},
  {17, 7, 6, 8, 25, 5, 3, 29, 37, 14, 31, 33, 30, 28, 19, 12, 20, 4, 38, 26, 16, 10, 36, 32, 0, 13, 11, 21, 35, 9, 34, 23, 1, 24, 27, 18, 2, 15, 22}
};

// led id's for columns (every row must have the same dimension, enter 99 if there are less leds)
uint8_t columns[COLUMNS][ROWS] = {
  {0,  5,  99, 99},
  {4,  99, 99, 99},
  {1,  6,  99, 99},
  {3,  99, 99, 99},
  {2,  7,  99, 99},
  {8,  10, 99, 99},
  {11, 12, 99, 99},
  {9,  99, 99, 99},
  {13, 14, 99, 99},
  {15, 16, 99, 99},
  {17, 18, 19, 20},
  {24, 21, 99, 99},
  {23, 22, 99, 99},
  {25, 26, 27, 28},
  {29, 32, 99, 99},
  {31, 30, 99, 99},
  {36, 99, 99, 99},
  {35, 99, 99, 99},
  {33, 34, 99, 99},
  {38, 99, 99, 99},
  {37, 99, 99, 99}
};

// columns in the reverse order
uint8_t reversecolumns[COLUMNS][ROWS] = {
  {37, 99, 99, 99},
  {38, 99, 99, 99},
  {33, 34, 99, 99},
  {35, 99, 99, 99},
  {36, 99, 99, 99},
  {31, 30, 99, 99},
  {29, 32, 99, 99},
  {25, 26, 27, 28},
  {23, 22, 99, 99},
  {24, 21, 99, 99},
  {17, 18, 19, 20},
  {15, 16, 99, 99},
  {13, 14, 99, 99},
  {9,  99, 99, 99},
  {11, 12, 99, 99},
  {8,  10, 99, 99},
  {2,  7,  99, 99},
  {3,  99, 99, 99},
  {1,  6,  99, 99},
  {4,  99, 99, 99},
  {0,  5,  99, 99}
};

// led id's for letters (every row must have the same dimension, enter 99 if there are less leds)
uint8_t letters[LETTERS][MAXLETTER] = {
  {0,  1,  2,  3,  4,  5,  6,  7,  99},
  {8,  9,  10, 11, 12, 13, 14, 15, 16},
  {17, 18, 19, 20, 21, 22, 23, 24, 99},
  {25, 26, 27, 28, 29, 30, 31, 32, 99},
  {33, 34, 35, 36, 37, 38, 99, 99, 99}
};

// letters in the reverse order
uint8_t reverseletters[LETTERS][MAXLETTER] = {
  {33, 34, 35, 36, 37, 38, 99, 99, 99},
  {25, 26, 27, 28, 29, 30, 31, 32, 99},
  {17, 18, 19, 20, 21, 22, 23, 24, 99},
  {8,  9,  10, 11, 12, 13, 14, 15, 16},
  {0,  1,  2,  3,  4,  5,  6,  7,  99}
};

long colorpalette[] = {
  0xFF0000,
  0xFF4D00,
  0xFF6F00,
  0xFF9500,
  0xFFBB00,
  0xFFE100,
  0xF7FF00,
  0xD0FF00,
  0x90FF00,
  0x50FF00,
  0x00FF00,
  0x00FF20,
  0x00FF80,
  0x00F0A0,
  0x0070DC,
  0x0000FF,
  0x1000FF,
  0x3000FF,
  0x6400FF,
  0xF000FF,
  0xFF0080,
  CRGB::White,
  CRGB::Aqua,
  CRGB::DarkOliveGreen,
  CRGB::Teal,
  CRGB::Navy,
  CRGB::Fuchsia,
  CRGB::Purple,
  CRGB::Green,
  CRGB::Gray,
  CRGB::DeepPink,
  CRGB::HotPink,
  CRGB::SeaGreen,
  CRGB::Pink,
  CRGB::SeaGreen,
  CRGB::MediumSpringGreen,
  CRGB::GreenYellow,
  CRGB::DarkGreen,
  CRGB::Cyan,
  CRGB::Gold,
  CRGB::Chocolate,
  CRGB::SandyBrown,
  CRGB::Purple,
  CRGB::ForestGreen,
  CRGB::Magenta,
  CRGB::Violet,
  CRGB::SlateGray,
  CRGB::Orange,
  CRGB::AliceBlue,
  CRGB::Maroon,
  CRGB::Brown,
  CRGB::LawnGreen
};

void setup() {
  pinMode(button, INPUT_PULLUP);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  Serial.begin(9600);
  FastLED.clear();
}

void loop() {
  label:
  if (animationmode == 1) {
    rainbowsnake();
    if (animationmode != 1) {goto label;}
    colorlettersweep();
    if (animationmode != 1) {goto label;}
    rainbowcolumnfill();
    if (animationmode != 1) {goto label;}
    ledsoutin();
    if (animationmode != 1) {goto label;}
    lettersinoutrandom();
    if (animationmode != 1) {goto label;}
    rainbowsweep();
    if (animationmode != 1) {goto label;}
    ledsoutinrandom();
    if (animationmode != 1) {goto label;}
    colorreversesnake();
    if (animationmode != 1) {goto label;}
    randomrain();
    if (animationmode != 1) {goto label;}
    columnsoutinrandom();
    if (animationmode != 1) {goto label;}
    fadecolor();
    if (animationmode != 1) {goto label;}
    colorsnake();
    if (animationmode != 1) {goto label;}
    columnsinout();
    if (animationmode != 1) {goto label;}
    lettersinout();
    if (animationmode != 1) {goto label;}
    ledsinoutrandom();
    if (animationmode != 1) {goto label;}
    lettersoutin();
    if (animationmode != 1) {goto label;}
    columnsoutin();
    if (animationmode != 1) {goto label;}
    randomreversesnake();
    if (animationmode != 1) {goto label;}
    ledsinout();
    if (animationmode != 1) {goto label;}
    randomfill();
    if (animationmode != 1) {goto label;}
    columnsinoutrandom();
    if (animationmode != 1) {goto label;}
    colorfill();
    if (animationmode != 1) {goto label;}
    lettersoutinrandom();
    if (animationmode != 1) {goto label;}
    colorrain();
  }
  
  else if (animationmode == 2) {
    FastLED.clear();
    FastLED.setBrightness(brightness);
    CRGB lastcolor = CRGB::Black;
    while (1) {
      CRGB newcolor = returncolor();
      for (uint8_t i = 0; i < 255; i = i + 3) {
        CRGB color = CRGB(lerp8by8(lastcolor.r, newcolor.r, i), lerp8by8(lastcolor.g, newcolor.g, i), lerp8by8(lastcolor.b, newcolor.b, i));
        fill_solid(leds, NUM_LEDS, color);
        FastLED.show();
        delay(3.5*SPEED-16);
      }
      delay(0.5*SPEED*SPEED);
      lastcolor = newcolor;
      settings();
      if (animationmode != 2) {goto label;}
    }
  }

  else if (animationmode == 3) {
    ledsinout();
    if (animationmode != 3) {goto label;}
    ledsoutin();
    if (animationmode != 3) {goto label;}
    columnsinout();
    if (animationmode != 3) {goto label;}
    columnsoutin();
    if (animationmode != 3) {goto label;}
    lettersinout();
    if (animationmode != 3) {goto label;}
    lettersoutin();
  }

  else if (animationmode == 4) {
    ledsinoutrandom();
    if (animationmode !=4) {goto label;}
    ledsoutinrandom();
    if (animationmode !=4) {goto label;}
    columnsinoutrandom();
    if (animationmode !=4) {goto label;}
    columnsoutinrandom();
    if (animationmode !=4) {goto label;}
    lettersinoutrandom();
    if (animationmode !=4) {goto label;}
    lettersoutinrandom();
  }

  else if (animationmode == 5) {
    colorsnake();
    if (animationmode != 5) {goto label;}
    colorreversesnake();
    if (animationmode != 5) {goto label;}
    randomsnake();
    if (animationmode != 5) {goto label;}
    randomreversesnake();
  }

  else if (animationmode == 6) {
    colorcolumnfill();
    if (animationmode != 6) {goto label;}
    randomcolumnfill();
    if (animationmode != 6) {goto label;}
    rainbowcolumnfill();
  }

  else if (animationmode == 7) {
    rainbowsweep();
    if (animationmode != 7) {goto label;}
    rainbowsnake();
    if (animationmode != 7) {goto label;}
    rainbowcolumnfill();
  }

  else if (animationmode == 8) {
    colorfill();
    if (animationmode != 8) {goto label;}
    randomfill();
  }

  else if (animationmode == 9) {
    colorcolumnfill();
    if (animationmode != 9) {goto label;}
    randomcolumnfill();
  }

  else if (animationmode == 10) {
    colorrain();
    if (animationmode != 10) {goto label;}
    randomrain();
  }

  else if (animationmode == 11) {
    ledsinout();
    if (animationmode != 11) {goto label;}
    ledsoutin();
  }

  else if (animationmode == 12) {
    columnsinout();
    if (animationmode != 12) {goto label;}
    columnsoutin();
  }

  else if (animationmode == 13) {
    lettersinout();
    if (animationmode != 13) {goto label;}
    lettersoutin();
  }

  else if (animationmode == 14) {
    colorlettersweep();
  }

  else if (animationmode == 15) {
    ledsinoutrandom();
    if (animationmode != 15) {goto label;}
    ledsoutinrandom();
  }

  else if (animationmode == 16) {
    columnsinoutrandom();
    if (animationmode != 16) {goto label;}
    columnsoutinrandom();
  }

  else if (animationmode == 17) {
    rainbowcolumnfill();
  }

  else if (animationmode == 18) {
    lettersinoutrandom();
    if (animationmode != 18) {goto label;}
    lettersoutinrandom();
  }

  else if (animationmode == 19) {
    rainbowsweep();
  }

  else if (animationmode == 20) {
    colorfill();
  }

  else if (animationmode == 21) {
    randomfill();
  }

  else if (animationmode == 22) {
    colorsnake();
    if (animationmode != 22) {goto label;}
    colorreversesnake();
  }

  else if (animationmode == 23) {
    randomsnake();
    if (animationmode != 23) {goto label;}
    randomreversesnake();
    
  }

  else if (animationmode == 24) {
    rainbowsnake();
  }

  else if (animationmode == 25) {
    colorrain();
  }

  else if (animationmode == 26) {
    randomrain();
  }

  else if (animationmode == 27) {
    fadecolor();
  }

  else if (animationmode == 28) {
    setrainbow();
  }

  else if (animationmode == 29) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, CRGB::White);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 29) {goto label;}
  }

  else if (animationmode == 30) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0xFF0000);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 30) {goto label;}
  }

  else if (animationmode == 31) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0xFF4400);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 31) {goto label;}
  }

  else if (animationmode == 32) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0xFFBB00);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 32) {goto label;}
  }

  else if (animationmode == 33) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x90FF00);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 33) {goto label;}
  }

  else if (animationmode == 34) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x00FF00);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 34) {goto label;}
  }

  else if (animationmode == 35) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x00F0A0);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 35) {goto label;}
  }

  else if (animationmode == 36) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x0070DC);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 36) {goto label;}
  }

  else if (animationmode == 37) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x0000FF);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 37) {goto label;}
  }

  else if (animationmode == 38) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0x6400FF);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 38) {goto label;}
  }

  else if (animationmode == 39) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, 0xFF0080);
    FastLED.show();
    delay(500);
    settings();
    if (animationmode != 39) {goto label;}
  }
}

void settings() {
  if (digitalRead(button) == LOW) {
    FastLED.clear();
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(300);
    fill_solid(leds, NUM_LEDS, CRGB::Orange);
    FastLED.show();
    delay(300);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    FastLED.show();
    delay(300);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    lastpressed = millis();
    while (millis() - lastpressed < 5000) {
      fill_solid(leds, NUM_LEDS, CRGB::White);
      FastLED.setBrightness(brightness);
      FastLED.show();
      if (digitalRead(button) == LOW) {
        delay(250);
        if (digitalRead(button) == LOW) {
          fill_solid(leds, NUM_LEDS, CRGB::Red);
          FastLED.show();
          delay(300);
          fill_solid(leds, NUM_LEDS, CRGB::Orange);
          FastLED.show();
          delay(300);
          fill_solid(leds, NUM_LEDS, CRGB::Yellow);
          FastLED.show();
          delay(300);
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          while (millis() - lastpressed < 5000) {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            fill_solid(leds, animationmode, CRGB::White);
            FastLED.show();
            if (digitalRead(button) == LOW) {
              delay(250);
              if (digitalRead(button) == LOW) {
                fill_solid(leds, NUM_LEDS, CRGB::Red);
                FastLED.show();
                delay(300);
                fill_solid(leds, NUM_LEDS, CRGB::Orange);
                FastLED.show();
                delay(300);
                fill_solid(leds, NUM_LEDS, CRGB::Yellow);
                FastLED.show();
                delay(300);
                fill_solid(leds, NUM_LEDS, CRGB::Black);
                while (millis() - lastpressed < 5000) {
                  fill_solid(leds, NUM_LEDS, CRGB::Black);
                  fill_solid(leds, minspeed-SPEED, CRGB::White);
                  FastLED.show();
                  if (digitalRead(button) == LOW) {
                    delay(300);
                    lastpressed = millis();
                    SPEED = SPEED - 1;
                    if (SPEED < maxspeed) {
                      SPEED = minspeed - 1;
                    }
                    FastLED.clear();
                    fill_solid(leds, minspeed -SPEED, CRGB::White);
                    FastLED.show();
                  }
                  yield();
                }
              }
              else {
                lastpressed = millis();
                animationmode = animationmode + 1;
                if (animationmode > modes) {
                  animationmode = 1;
                }
                FastLED.clear();
                fill_solid(leds, animationmode, CRGB::White);
                FastLED.show();
              }
            }
            yield();
          }
        }
        else {
          lastpressed = millis();
          brightness = brightness + 22;
          if (brightness > maxbrightness) {
            brightness = minbrightness;
          }
          fill_solid(leds, brightness/10, CRGB::White);
          FastLED.setBrightness(brightness);
          FastLED.show();
        }
      }
      yield();
    }
  }
}

long returncolor() {
  long color = colorpalette[micros() % (sizeof(colorpalette) / sizeof(colorpalette[0]))];
  if (color == prevcolor) {
    return returncolor();
  }
  else {
    return color;
  }
  return colorpalette[0];
}

void setrainbow() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = colorpalette[column];
    }
  }
  FastLED.show();
  delay(SPEED * 100);
  settings();
}

void fadecolor() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(0);
  long color = returncolor();
  for (uint8_t tempbrightness = 0; tempbrightness < brightness; tempbrightness++) {
    FastLED.setBrightness(tempbrightness);
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    delay(SPEED*60 / brightness);
  }
  delay(SPEED*SPEED);
  for (uint8_t tempbrightness = brightness; tempbrightness > 0; tempbrightness--) {
    FastLED.setBrightness(tempbrightness);
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    delay(SPEED*60 / brightness);
  }
  delay(micros() % 5);
  FastLED.setBrightness(0);
  FastLED.clear();
  settings();
}

void ledsinout() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[led] = color;
    FastLED.show();
    delay(SPEED * 3);
  }
  delay(SPEED*SPEED);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[led] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 3);
  }
  FastLED.clear();
  settings();
}

void columnsinout() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < COLUMNS; led++) {
      leds[columns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void lettersinout() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 25);
  }
  delay(SPEED*SPEED);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 25);
  }
  FastLED.clear();
  settings();
}

void ledsoutin() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[reverseleds[led]] = color;
    FastLED.show();
    delay(SPEED * 3);
  }
  delay(SPEED*SPEED);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[reverseleds[led]] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 3);
  }
  FastLED.clear();
  settings();
}

void columnsoutin() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[reversecolumns[column][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < COLUMNS; led++) {
      leds[reversecolumns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  FastLED.setBrightness(0);
  settings();
}

void lettersoutin() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[reverseletters[letter][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 25);
  }
  delay(SPEED*SPEED);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[reverseletters[letter][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 25);
  }
  FastLED.clear();
  settings();
}

void ledsinoutrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[led] = returncolor();
    FastLED.show();
    delay(SPEED * 3 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[led] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 3 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void columnsinoutrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = returncolor();
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void lettersinoutrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = returncolor();
    }
    FastLED.show();
    delay(SPEED * 25 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 25 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void ledsoutinrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[reverseleds[led]] = returncolor();
    FastLED.show();
    delay(SPEED * 3 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[reverseleds[led]] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 3 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void columnsoutinrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[reversecolumns[column][led]] = returncolor();
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[reversecolumns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void lettersoutinrandom() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[reverseletters[letter][led]] = returncolor();
    }
    FastLED.show();
    delay(SPEED * 25 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[reverseletters[letter][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 25 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void colorfill() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  uint8_t sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[randomsequence[sequence][led]] = color;
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  delay(SPEED*SPEED);
  sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[randomsequence[sequence][led]] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void randomfill() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  uint8_t sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[randomsequence[sequence][led]] = returncolor();
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  delay(SPEED*SPEED);
  sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    leds[randomsequence[sequence][led]] = CRGB::Black;
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void colorcolumnfill() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void randomcolumnfill() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = returncolor();
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void rainbowcolumnfill() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = colorpalette[column];
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  delay(SPEED*SPEED);
  for (uint8_t column = 0; column < COLUMNS; column++) {
    for (uint8_t led = 0; led < ROWS; led++) {
      leds[columns[column][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  FastLED.setBrightness(0);
  settings();
}

void rainbowsweep() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t maxcolumn = 0; maxcolumn < 2 * COLUMNS; maxcolumn++) {
    for (uint8_t column = 0; column <= maxcolumn; column++) {
      for (uint8_t led = 0; led < ROWS; led++) {
        if (column < COLUMNS) {
          leds[columns[column][led]] = colorpalette[maxcolumn - column];
        }
        else {
          leds[columns[column - COLUMNS][led]] = CRGB::Black;
        }
      }
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void colorlettersweep() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    long color = returncolor();
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = color;
    }
    FastLED.show();
    delay(SPEED * 20);
  }
  delay(SPEED*SPEED);
  for (uint8_t letter = 0; letter < LETTERS; letter++) {
    for (uint8_t led = 0; led < MAXLETTER; led++) {
      leds[letters[letter][led]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 20);
  }
  FastLED.clear();
  settings();
}

void colorsnake() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  uint8_t del = (millis() % (NUM_LEDS - 10)) + 5;
  for (uint8_t led = 0; led < NUM_LEDS + del; led++) {
    if (led < NUM_LEDS) {
      leds[led] = color;
    }
    if (led > del - 1) {
      leds[led - del] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void randomsnake() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  uint8_t del = (millis() % (NUM_LEDS - 10)) + 5;
  for (uint8_t led = 0; led < NUM_LEDS + del; led++) {
    if (led < NUM_LEDS) {
      leds[led] = returncolor();
    }
    if (led > del - 1) {
      leds[led - del] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void rainbowsnake() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  for (uint8_t maxled = 0; maxled < NUM_LEDS + RAINBOWLENGTH; maxled++) {
    for (uint8_t led = 0; led < NUM_LEDS; led++) {
      leds[led] = colorpalette[maxled - led];
      if (maxled > (RAINBOWLENGTH - 1) and led < (maxled - RAINBOWLENGTH + 3) or led > maxled) {
        leds[led] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void colorreversesnake() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  uint8_t del = (millis() % (NUM_LEDS - 10)) + 5;
  for (uint8_t led = 0; led < NUM_LEDS + del; led++) {
    if (led < NUM_LEDS) {
      leds[reverseleds[led]] = color;
    }
    if (led > del - 1) {
      leds[reverseleds[led - del]] = CRGB::Black;
    }
    FastLED.show();
    delay(SPEED * 5);
  }
  FastLED.clear();
  settings();
}

void randomreversesnake() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  uint8_t del = (millis() % (NUM_LEDS - 10)) + 5;
  for (uint8_t led = 0; led < NUM_LEDS + del; led++) {
    if (led < NUM_LEDS) {
      leds[reverseleds[led]] = returncolor();
      FastLED.show();
    }
    if (led > del - 1) {
      leds[reverseleds[led - del]] = CRGB::Black;
      FastLED.show();
    }
    delay(SPEED * 5 + micros() % 5);
  }
  FastLED.clear();
  settings();
}

void colorrain() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  long color = returncolor();
  uint8_t sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < 15; led++) {
    leds[randomsequence[sequence][led]] = color;
    FastLED.show();
    delay(-0.13021*SPEED*SPEED+29.167*SPEED+120.313);
    leds[randomsequence[sequence][led]] = CRGB::Black;
  }
  FastLED.clear();
  settings();
}

void randomrain() {
  delay(0.5*SPEED*SPEED);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  uint8_t sequence = millis() % (RANDLISTS - 1);
  for (uint8_t led = 0; led < 15; led++) {
    long color = returncolor();
    leds[randomsequence[sequence][led]] = color;
    FastLED.show();
    delay(-0.13021*SPEED*SPEED+29.167*SPEED+120.313);
    leds[randomsequence[sequence][led]] = CRGB::Black;
  }
  FastLED.clear();
  settings();
}
