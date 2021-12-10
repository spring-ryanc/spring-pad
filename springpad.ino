/******************************************************
    https://github.com/spring-ryanc/spring-pad
******************************************************/

#include <ClickEncoder.h> // https://github.com/0xPIT/encoder
#include <TimerOne.h>

#include <Keypad_Matrix.h> // https://github.com/nickgammon/Keypad_Matrix
#include "HID-Project.h"   // https://github.com/NicoHood/HID
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //#include <Adafruit_SSD1306.h> //Doesnt work with 128x64 screens
#include <Adafruit_SH110X.h>

// Setup SSD1306 display connected with I2C (https://www.sparkfun.com/products/17153)
#define OLED_RESET     4 // Reset pin
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
#define TOTAL_LAYERS 3

// Current states
boolean animateState = false;
boolean ledState = false;
int screenSaverDelay = 0;
int layer = 0;

// Slide pot
#define SIDE_PIN A3
int current_vol = 0;

// Rotary Encoder
ClickEncoder *encoder;
int16_t lastEncoderVal, encoderVal;

void timerIsr() {
  encoder->service();
}

void setup()
{
  SerialUSB.begin(9600);

  encoder = new ClickEncoder(10, 16, 14);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  lastEncoderVal = encoder->getValue();

  if (!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();

  kpd.begin();
  kpd.setKeyDownHandler(keyDown);
  kpd.setKeyUpHandler(keyUp);

  // Green LED
  pinMode(15, OUTPUT);

  // Analog input
  pinMode(SIDE_PIN, INPUT );

  Keyboard.begin();
  Consumer.end();
}

void loop()
{
  if (layer == 2) {
    processSlider();
  } else {
    processEncoder();
  }
  kpd.scan();
  runAnimation();
}

void processEncoder() {
  encoderVal += encoder->getValue();
  if (encoderVal != lastEncoderVal) {
    recordEvent();
    if (layer == 0) {
      if (lastEncoderVal > encoderVal) {
        Consumer.write(MEDIA_VOLUME_UP);
        displayCurrentKey(0, "Vol Up");
      } else {
        Consumer.write(MEDIA_VOLUME_DOWN);
        displayCurrentKey(0, "Vol Down");
      }
    } else {
      displayCurrentKey(9, encoderVal);
    }
    lastEncoderVal = encoderVal;
  }
  resetScreenSaver();
}

void processSlider() {
  int sliderValue = analogRead(SIDE_PIN);

  int vol = map(sliderValue, 0, 1024, 0, 100);
  if (vol != current_vol) {
    current_vol = vol;
    Serial.print("Slide Pot value: ");
    Serial.println(sliderValue);
    drawtext("Vol " + String(vol));
    for (int i = 0; i < 50; i++) {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    for (int i = 0; i < (vol / 2); i++ ) {
      Consumer.write(MEDIA_VOLUME_UP);
    }
  }

  resetScreenSaver();
}

void keyDown (const char which)
{
  recordEvent();
  int key = int(which) - A_OFFSET;
  if (key == LAYER_KEY) {
    layer = (layer + 1) % TOTAL_LAYERS;
    displayCurrentKey(layer, "");
    return;
  }
  processLayer(layer, key);
}

void keyUp (const char which)
{
  resetScreenSaver();
  Keyboard.releaseAll();
}

void recordEvent() {
  animateState = false;
  if (screenSaverDelay > 0) {
    screenSaverDelay = 0;
  }
  screenSaverDelay--;
}

void resetScreenSaver() {
  screenSaverDelay++;
}
