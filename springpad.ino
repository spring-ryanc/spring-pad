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
boolean screenSaverActive = false;
boolean hibernateActive = false;
boolean ledState = false;
long screenSaverDelay = 0;
int layer = 0;

// Slide pot
#define SIDE_PIN A3
int current_vol = -1;
boolean disableSlider = false;

// Deej
const int NUM_SLIDERS = 1;
const int analogInputs[NUM_SLIDERS] = {A3};
int analogSliderValues[NUM_SLIDERS];

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
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = map(analogRead(analogInputs[i]), 0, 1024, 0, 100);
  }

  Keyboard.begin();
  Consumer.end();
}

void loop()
{
  if (layer == 2 && !disableSlider) {
    processSlider();
  } else if (!disableSlider) { // Deej
    processDeej();
  }
  processEncoder();
  kpd.scan();
  runScreenSaver();
}

void processEncoder() {
  encoderVal += encoder->getValue();
  if (encoderVal != lastEncoderVal) {
    recordEvent();
    if (layer == 0) {
      if (lastEncoderVal > encoderVal) {
        Consumer.write(MEDIA_VOLUME_UP);
        displayCurrentKey(layer, "Vol Up");
      } else {
        Consumer.write(MEDIA_VOLUME_DOWN);
        displayCurrentKey(layer, "Vol Down");
      }
    } else {
      displayCurrentKey(9, encoderVal);
    }
    lastEncoderVal = encoderVal;
  }
  resetScreenSaver();
}

void processDeej() {
  bool changed = false;
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int rawVal = analogRead(analogInputs[i]);
    int val = map(rawVal, 0, 1024, 0, 100);
    if (abs(val - analogSliderValues[i]) > 2) {
      analogSliderValues[i] = val;
      changed = true;
      displayCurrentKey(layer, "Spotify:" + String(val));

      int x = map(val, 0, 100, 5, 120);
      display.fillTriangle(5, 50, x, 50, x, 50 - map(val, 0, 100, 0, 20), SH110X_WHITE);
      display.drawTriangle(5, 50, 120, 50, 120, 30, SH110X_WHITE);
      display.display();
    }
  }

  if (!changed) {
    return;
  }
  recordEvent();
  String builtString = String("");
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int val = map(analogSliderValues[i], 0, 100, 0, 1024);
    builtString += String(val);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
  resetScreenSaver();
}

void processSlider() {
  int sliderValue = analogRead(SIDE_PIN);
  int vol = map(sliderValue, 0, 1024, 0, 100);

  if (vol != current_vol) {
    Serial.print("Slide Pot value: ");
    Serial.println(sliderValue);
    drawtext("Vol " + String(vol));
    for (int i = current_vol; i > vol; i = i - 2) {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    for (int i = current_vol; i < vol; i = i + 2) {
      Consumer.write(MEDIA_VOLUME_UP);
    }
    current_vol = vol;

  }

  resetScreenSaver();
}

void resetSliderVolume() {
  int sliderValue = analogRead(SIDE_PIN);
  int vol = map(sliderValue, 0, 1024, 0, 100);
  if (vol != current_vol) {
    current_vol = vol;
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
    if (kpd.isKeyDown('B')) {
      disableSlider = !disableSlider;
      drawtext("Slider\n" + String(disableSlider ? "Off" : "On"));      
      return;
    }
    
    layer = (layer + 1) % TOTAL_LAYERS;
    displayCurrentKey(layer, "");
    if (layer == 2) {
      resetSliderVolume();
    }
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
  screenSaverActive = false;
  hibernateActive = false;
  if (screenSaverDelay > 0) {
    screenSaverDelay = -1; // Turn off screen saver until event completes.
  } else {
    screenSaverDelay--;
  }
}

void resetScreenSaver() {
  screenSaverDelay++; // Reset screen saver timer.
}
