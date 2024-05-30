#define ANIMATION_DELAY 20000
int animationDelay = ANIMATION_DELAY;  // To slow down refresh
#define SCREENSAVERDELAY 600000
#define HIBERNATEDELAY 1200000
#define DEADSLEEPDELAY 120000000

#define XPOS 0  // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 1
#define NUMFLAKES 6
int8_t icons[NUMFLAKES][3];

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 30
static const unsigned char PROGMEM logo_bmp[] = {
    0x1f, 0xe0, 0x0,  0x0,  0x60, 0x38, 0x0, 0x0,  0x60, 0x6, 0x0,  0x0,  0x30, 0x2,  0x0, 0x0,
    0xe,  0x3,  0xff, 0xc0, 0x6,  0x3,  0x0, 0x38, 0xc,  0x0, 0x0,  0xc,  0x18, 0x0,  0x0, 0xc,
    0x10, 0x0,  0x0,  0x18, 0x20, 0x18, 0x0, 0xc,  0xc0, 0x7, 0x0,  0x1c, 0xc0, 0x0,  0x0, 0x30,
    0x60, 0x0,  0x0,  0x30, 0x30, 0x0,  0x0, 0x70, 0xe,  0xf, 0xff, 0x80, 0x1,  0xf8, 0x0, 0x0};

#define BIG_LOGO_HEIGHT 52
#define BIG_LOGO_WIDTH 100

static const unsigned char PROGMEM big_logo_bmp[] = {
    0x0,  0x0,  0x1f, 0x6c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3f, 0xff,
    0xff, 0xf0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff, 0xff, 0xff, 0xfe, 0x80,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0xe0, 0x0,  0x0,  0xff, 0xe0, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0xf,  0xc0, 0x0,  0x0,  0x1f, 0xf8, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x1f, 0x80, 0x0,  0x0,  0x3,  0xfe, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1f, 0x80,
    0x0,  0x0,  0x0,  0xff, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1f, 0x80, 0x0,  0x0,  0x0,
    0x3f, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xf,  0xc0, 0x0,  0x0,  0x0,  0x1f, 0xc0, 0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0xe0, 0x0,  0x0,  0x0,  0xf,  0xe0, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x3,  0xe0, 0x0,  0x0,  0x0,  0x7,  0xe0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0xfc, 0x0,  0x0,  0x0,  0x3,  0xe0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff, 0xc0, 0x0,
    0x0,  0x3,  0xf0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1f, 0xf8, 0x0,  0x0,  0x3,  0xf8,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0xfe, 0x0,  0x0,  0x3,  0xf9, 0x7f, 0xff, 0xff,
    0xef, 0x0,  0x0,  0x0,  0x0,  0x7f, 0x0,  0x0,  0x1,  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0,
    0x0,  0x0,  0x7f, 0x0,  0x0,  0x1,  0xfc, 0x0,  0x0,  0x0,  0x1f, 0xff, 0x0,  0x0,  0x0,  0x3e,
    0x0,  0x0,  0x1,  0xf0, 0x0,  0x0,  0x0,  0x0,  0xff, 0xc0, 0x0,  0x0,  0x7c, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0xf,  0xe0, 0x0,  0x0,  0x78, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0xf,  0xf0, 0x0,  0x1,  0xf0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,
    0xf0, 0x0,  0x3,  0xe0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0xf0, 0x0,  0x7,
    0xc0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0xf0, 0x0,  0xf,  0x80, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0xf0, 0x0,  0x1f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0xf,  0xe0, 0x0,  0x3e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x7f, 0xc0, 0x0,  0x3c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0xff, 0x0,  0x0,
    0x78, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x7f, 0xc0, 0x0,  0xf0, 0x0,  0x0,
    0x0,  0x10, 0x0,  0x0,  0x0,  0x0,  0x0,  0x1f, 0xc0, 0x0,  0xe0, 0x0,  0x0,  0x0,  0x78, 0x0,
    0x0,  0x0,  0x0,  0x0,  0xf,  0xe0, 0x3,  0xe0, 0x0,  0x0,  0x7,  0xf0, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x7,  0xe0, 0x1f, 0xc0, 0x0,  0x0,  0x1f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0xf0,
    0x7f, 0xc0, 0x0,  0x0,  0x60, 0x0,  0x8,  0x0,  0x0,  0x0,  0x0,  0x7,  0xf0, 0x7c, 0x0,  0x0,
    0x1,  0x0,  0x1,  0xbc, 0x0,  0x0,  0x0,  0x0,  0x7,  0xe0, 0xfc, 0x0,  0x0,  0x0,  0x0,  0x1f,
    0xfc, 0x0,  0x0,  0x0,  0x0,  0x1f, 0xe0, 0xfc, 0x0,  0x0,  0x0,  0x7,  0xfc, 0x40, 0x0,  0x0,
    0x0,  0x0,  0x7f, 0xc0, 0xfc, 0x0,  0x0,  0x0,  0x0,  0x20, 0x0,  0x0,  0x0,  0x0,  0x1,  0xff,
    0x0,  0x7e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0xfe, 0x0,  0x3f, 0x0,
    0x0,  0x0,  0x0,  0x10, 0xd7, 0x0,  0x0,  0x0,  0x1,  0xf8, 0x0,  0x1f, 0xc0, 0x0,  0x0,  0x5,
    0x7,  0xff, 0x0,  0x0,  0x0,  0x1,  0xfc, 0x0,  0x1f, 0xe0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x1,  0xf8, 0x0,  0x7,  0xf0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0xf8, 0x0,  0x3,  0xfc, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0xf0, 0x0,  0x0,
    0xfe, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0xf0, 0x0,  0x0,  0x7f, 0x80, 0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xf,  0xe0, 0x0,  0x0,  0x1f, 0xf0, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x7f, 0x80, 0x0,  0x0,  0x7,  0xfc, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xbf,
    0xfe, 0x0,  0x0,  0x0,  0x1,  0xfe, 0x80, 0x1,  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0,  0x0,
    0x0,  0x0,  0x7f, 0xe0, 0x1,  0xff, 0xff, 0xff, 0xff, 0xf6, 0x0,  0x0,  0x0,  0x0,  0x0,  0xf,
    0xff, 0xff, 0xf8, 0xf,  0xef, 0x60, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0xff, 0xff, 0xe0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xf,  0xff, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0};

void displayCurrentKey(int layer, char key) { displayCurrentKey(layer, String(key)); }

void displayCurrentKey(int layer, String key) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Layer: " + String(layer));

  if (key != "") {
    display.setTextSize(2);
    display.println(key);
  }
  display.display();
}

void runScreenSaver() {
  if (deadSleepActive) {
    return;
  }
  if (hibernateActive) {
    if (screenSaverDelay++ > DEADSLEEPDELAY) {
      deadSleepActive = true;
      display.clearDisplay();
      display.display();
    }
    return;
  }
  if (screenSaverActive) {
    if (screenSaverDelay++ > HIBERNATEDELAY) {
      screenSaverActive = false;
      hibernateActive = true;
      drawLogo();
    }
    if (animationDelay++ < ANIMATION_DELAY) {
      return;
    }
    animationDelay = 0;

    display.clearDisplay();
    for (int8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo_bmp, LOGO_WIDTH, LOGO_HEIGHT,
                         SH110X_WHITE);
    }
    if (debugMode) {
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 0);
      display.cp437(true);
      display.println(screenSaverDelay);
    }
    display.display();

    // Update coordinates of each flake
    for (int8_t f = 0; f < NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS] = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  } else if (screenSaverDelay > SCREENSAVERDELAY) {
    startAnimation();
    screenSaverDelay = 0;

    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
  } else if (screenSaverDelay > 0) {  // Dont increase while key is held down
    screenSaverDelay++;
  }
}

void startAnimation() {
  // Initialize 'snowflake' positions
  for (int8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS] = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  screenSaverActive = true;
}

void drawLogo() {
  display.clearDisplay();
  display.drawBitmap((display.width() - BIG_LOGO_WIDTH) / 2,
                     (display.height() - BIG_LOGO_HEIGHT) / 2, big_logo_bmp, BIG_LOGO_WIDTH,
                     BIG_LOGO_HEIGHT, SH110X_WHITE);
  display.display();
}