/******************************************************
    https://github.com/spring-ryanc/spring-pad
******************************************************/
#include "HID-Project.h"  // https://github.com/NicoHood/HID

// Imports for OLED screen
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>
#include <Wire.h>

// Screen setup (connect SDA to pin 2, and SCK to pin 3, GND to ground, VDD to 5V).
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// NOTE: my original running code uses 0x3C for my 64pixel screen.. try both if having issues.
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Slide pot
#define SLIDER_PIN A3
int current_vol = -1;

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.println("Hello world!");

  // Initialize OLED screen.
  if (!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {
      delay(100);
    }  // Don't proceed, loop forever
  }
  display.display();

  pinMode(SLIDER_PIN, INPUT);
  syncSliderVolume();

  Keyboard.begin();
  Consumer.end();
}

void loop() {
  processSlider();
  delay(1000);
}

void processSlider() {
  int sliderValue = analogRead(SLIDER_PIN);
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
}

// Sets current volume to 0 then increases to current value.
void syncSliderVolume() {
  int sliderValue = analogRead(SLIDER_PIN);
  int vol = map(sliderValue, 0, 1024, 0, 100);
  if (vol != current_vol) {
    current_vol = vol;
    for (int i = 0; i < 50; i++) {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    for (int i = 0; i < (vol / 2); i++) {
      Consumer.write(MEDIA_VOLUME_UP);
    }
  }
}

// Writes text to OLED screen.
void drawtext(String text) {
  display.clearDisplay();

  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE);  // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
  display.println(text);

  display.display();
}
