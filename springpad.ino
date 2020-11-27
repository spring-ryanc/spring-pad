#include <Keypad_Matrix.h> //https://github.com/nickgammon/Keypad_Matrix
#include "HID-Project.h" // https://github.com/NicoHood/HID
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte ROWS = 2;
const byte COLS = 7;

#define LAYER_KEY  0
#define TOTAL_LAYERS 2

// Define how the keypad is laid out
const char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D', 'E', 'F', 'G'}, // A = 65
  {'H', 'I', 'J', 'K', 'L', 'M', 'N'}
  //  {CONSUMER_CALCULATOR, MEDIA_VOLUME_MUTE, MEDIA_VOL_DOWN, MEDIA_VOL_UP, MEDIA_PAUSE, MEDIA_PLAY_PAUSE}
};

// These are the pin declarations for the Qwiic Keyboard Explorera2
const byte rowPins[ROWS] = {A0, A1};
const byte colPins[COLS] = {A2, 4, 5, 6, 7, 8, 9};

// Keep track of which mode we're currently in
boolean kbdMode = false;
boolean pressed = false;

// Animation
boolean animate = false;

// Current pad layer
int layer = 0;

// Create the Keypad object
Keypad_Matrix kpd = Keypad_Matrix( makeKeymap (keys), rowPins, colPins, ROWS, COLS );

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  SerialUSB.begin(9600); // baud rate for Serial mode

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  kpd.begin(); // initialise keyboard matrix
  kpd.setKeyDownHandler(keyDown); // declare keydown function
  kpd.setKeyUpHandler(keyUp); // declare keyup function
  pinMode(A3, INPUT_PULLUP); // initialize mode switch pin
}

void loop()
{
  kpd.scan(); // Get keyswitch statuses
  checkMode(); // Check mode switch
  runAnimation();
}

// Key Press Handler
void keyDown (const char which)
{
  int key = int(which) - 65;
  pressed = true;
  animate = false;
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

// Key Release Handler
void keyUp (const char which)
{
  pressed = false;
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
