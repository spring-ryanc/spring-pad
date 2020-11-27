#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

void displayCurrentKey(int layer, char key) {
  displayCurrentKey(layer, String(key));
}

void displayCurrentKey(int layer, String key) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Layer: " + String(layer));

  if (key != "") {
    display.setTextSize(2);
    display.println(key);
  }
  display.display();
}

void drawtext(String text) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.println(text);

  display.display();
}

#define XPOS      0 // Indexes into the 'icons' array in function below
#define YPOS      1
#define DELTAY    1
#define NUMFLAKES 10

int8_t icons[NUMFLAKES][3];

void startAnimation() {
  // Initialize 'snowflake' positions
  for (int8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  animateState = true;
}

#define ANIMATION_DELAY 2000
int animationDelay = ANIMATION_DELAY; // To slow down refresh
#define SCREENSAVERDELAY 20000
int screenSaver = 0;

void runAnimation() {
  if (animateState) {
    if (animationDelay++ < ANIMATION_DELAY) {
      return;
    }
    animationDelay = 0;

    display.clearDisplay();
    for (int8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
    }
    display.display();

    // Update coordinates of each flake
    for (int8_t f = 0; f < NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  } else   if (screenSaver++ > SCREENSAVERDELAY) {
    startAnimation();
    screenSaver = 0;
  }
}
