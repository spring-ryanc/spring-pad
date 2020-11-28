#include <Keypad_Matrix.h> // https://github.com/nickgammon/Keypad_Matrix
#include "HID-Project.h"   // https://github.com/NicoHood/HID
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Setup SSD1306 display connected with I2C (https://www.sparkfun.com/products/17153)
#define OLED_RESET     4 // Reset pin
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte ROWS = 2;
const byte COLS = 7;

const char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D', 'E', 'F', 'G'},
  {'H', 'I', 'J', 'K', 'L', 'M', 'N'}
};
const byte rowPins[ROWS] = {A0, A1};
const byte colPins[COLS] = {A2, 4, 5, 6, 7, 8, 9};

// Setup macropad input (https://www.sparkfun.com/products/17251)
Keypad_Matrix kpd = Keypad_Matrix( makeKeymap (keys), rowPins, colPins, ROWS, COLS );

// ASCII characters
#define A_OFFSET 65
#define a_OFFSET 97
#define ONE_OFFSET 49 //1

#define LAYER_KEY  0
#define TOTAL_LAYERS 2

// Current states
boolean kbdMode = false;
boolean animateState = false;
int screenSaverDelay = 0;
int layer = 0;

void setup()
{
  SerialUSB.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  
  kpd.begin();
  kpd.setKeyDownHandler(keyDown);
  kpd.setKeyUpHandler(keyUp);
  pinMode(A3, INPUT_PULLUP); // TODO: find an actual use for this switch
}

void loop()
{
  kpd.scan();
  checkMode();
  runAnimation();
}

void keyDown (const char which)
{
  int key = int(which) - A_OFFSET;
  animateState = false;
  screenSaverDelay = 0;
  if (kbdMode) {
    if (key == LAYER_KEY) {
      layer = (layer + 1) % TOTAL_LAYERS;
      displayCurrentKey(layer, "");
      return;
    }
    processLayer(layer, key);
  } else {
    SerialUSB.print (F("Key down: "));
    SerialUSB.println (which);
  }
}

void keyUp (const char which)
{
  if (kbdMode) {
    Keyboard.release(which);
  } else {
    SerialUSB.print (F("Key up: "));
    SerialUSB.println (which);
  }
}

// Check the mode switch position and change between USB-HID mode and Serial mode
void checkMode() {
  if (kbdMode) {
    if (!digitalRead(A3)) {
      kbdMode = false;
      Keyboard.end();
      Consumer.begin();
    }
  } else {
    if (digitalRead(A3)) {
      kbdMode = true;
      Keyboard.begin();
      Consumer.end();
    }
  }
}
