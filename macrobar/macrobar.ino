/******************************************************
    https://github.com/spring-ryanc/spring-pad????
******************************************************/

#include <Adafruit_GFX.h>  //#include <Adafruit_SSD1306.h> //Doesnt work with 128x64 screens
#include <Adafruit_SH110X.h>
#include <ClickEncoder.h>  // https://github.com/0xPIT/encoder
#include <SPI.h>
#include <TimerOne.h>
#include <Wire.h>

#include "HID-Project.h"  // https://github.com/NicoHood/HID

// Setup SSD1306 display connected with I2C (https://www.sparkfun.com/products/17153)
#define OLED_RESET 4         // Reset pin
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET 4         // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Rotary Encoder
#define GREEN_PIN 15
#define RED_PIN 18
ClickEncoder *encoder;
int16_t lastEncoderVal, encoderVal;

// Buttons
#define B1_PIN 14
#define B2_PIN 16
#define B3_PIN 10

// Current states
boolean screenSaverActive = false;
boolean hibernateActive = false;
boolean deadSleepActive = false;
boolean ledState = false;
boolean debugMode = false;
long screenSaverDelay = 0;
int layer = 0;  // TODO: use or remove.

void timerIsr() { encoder->service(); }

void setup() {
  SerialUSB.begin(9600);

  // A -> 9, B -> 8, SW -> VCC... need to cut and rewire
  // 18 is currently unused
  encoder = new ClickEncoder(9, 8, 18);  // A, B, SW
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  lastEncoderVal = encoder->getValue();

  // Encoder LED
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);

  // Buttons
  pinMode(B1_PIN, INPUT_PULLUP);
  pinMode(B2_PIN, INPUT_PULLUP);
  pinMode(B3_PIN, INPUT_PULLUP);

  // Screen
  if (!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  display.display();
}

void loop() {
  processEncoder();
  runScreenSaver();
  processButtons();
}

void processButtons() {
  boolean b1 = digitalRead(B1_PIN) == LOW;
  boolean b2 = digitalRead(B2_PIN) == LOW;
  boolean b3 = digitalRead(B3_PIN) == LOW;
  if (b1 || b2 || b3) {
    // Wait a bit and read again to check for multiple presses.
    delay(1000);
    boolean b1 = b1 || digitalRead(B1_PIN) == LOW;
    boolean b2 = b2 || digitalRead(B2_PIN) == LOW;
    boolean b3 = b3 || digitalRead(B3_PIN) == LOW;
  }

  if (b3 && b2) {
    Consumer.write(MEDIA_NEXT);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    buttonExtras("Next");
  } else if (b1 && b2) {
    debugMode = !debugMode;
    buttonExtras("Debug Mode\n" + String(debugMode));
  } else if (b3) {
    Consumer.write(MEDIA_PLAY_PAUSE);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    buttonExtras("Play/Pause");
  } else if (b1) {
    Keyboard.press(KEY_LEFT_WINDOWS);
    Keyboard.press(HID_KEYBOARD_L_AND_L);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    buttonExtras("Lock");
  } else if (b2) {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_WINDOWS);
    Keyboard.press(HID_KEYBOARD_S_AND_S);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    buttonExtras("Print\nScreen");
  }

  Keyboard.releaseAll();
  resetScreenSaver();
}

void buttonExtras(String text) {
  displayCurrentKey(0, text);
  delay(1000);  // Debounce
  resetScreenSaver();
  recordEvent();
  Keyboard.releaseAll();
}

void processEncoder() {
  encoderVal += encoder->getValue();
  if (encoderVal != lastEncoderVal) {
    recordEvent();

    if (lastEncoderVal > encoderVal) {
      Consumer.write(MEDIA_VOLUME_UP);
      displayCurrentKey(layer, "Vol Up");
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(RED_PIN, LOW);
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN);
      displayCurrentKey(layer, "Vol Down");
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
    }

    lastEncoderVal = encoderVal;
  }
  resetScreenSaver();
}

void recordEvent() {
  screenSaverActive = false;
  hibernateActive = false;
  if (screenSaverDelay > 0) {
    screenSaverDelay = -1;  // Turn off screen saver until event completes.
  } else {
    screenSaverDelay--;
  }
}

void resetScreenSaver() {
  screenSaverDelay++;  // Reset screen saver timer.
}