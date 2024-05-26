/*
   These are from the examples that came with the display.
*/

void startScreenTest() {
  testdrawrect();      // Draw rectangles (outlines)
  testfillrect();      // Draw rectangles (filled)

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(500);
  display.invertDisplay(false);
  delay(500);
  startAnimation(); // Animate bitmaps
}

void testdrawrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SH110X_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(250);
}

void testfillrect(void) {
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, SH110X_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(250);
}
