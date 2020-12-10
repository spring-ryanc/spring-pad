void processLayer(int layer, int key) {
  switch (layer) {
    case 0:
      layer0(key);
      break;
    case 1:
      layer1(key);
      break;
    case 2:
      layer2(key);
      break;
  }
}

void layer0(int key) {
  if (kpd.isKeyDown('B')) {
    key += 100;
  }
  switch (key) {
    case 1:
    case 101:
      displayCurrentKey(0, "Preview");
      break;
    case 2:
      startAnimation();
      break;
    case 102:
      displayCurrentKey(0, "Screen Saver");
      break;
    case 3:
      if (ledState) {
        digitalWrite(15, LOW);
      } else {
        digitalWrite(15, HIGH);
      }
      ledState = !ledState;
    case 103:
      displayCurrentKey(0, "Green" + String(!ledState));
      break;
    case 9:
      startScreenTest();
      break;
    case 109:
      displayCurrentKey(0, "Screen\nTest");
      break;
    case 4:
      System.write(SYSTEM_SLEEP);
    case 104:
      displayCurrentKey(0, "Sleep");
      break;
    case 5:
      Consumer.write(MEDIA_PREVIOUS);
    case 105:
      displayCurrentKey(0, "Previous");
      break;
    case 6:
      Consumer.write(MEDIA_NEXT);
    case 106:
      displayCurrentKey(0, "Next");
      break;
    case 7:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_F9);
    case 107:
      displayCurrentKey(0, "Audio Output");
      break;
    case 10:
      Consumer.write(MEDIA_VOLUME_DOWN);
    case 110:
      displayCurrentKey(0, "Vol Down");
      break;
    case 11:
      Consumer.write(MEDIA_VOLUME_UP);
    case 111:
      displayCurrentKey(0, "Vol Up");
      break;
    case 12:
      Consumer.write(MEDIA_VOLUME_MUTE);
    case 112:
      displayCurrentKey(0, "Mute");
      break;
    case 13:
      Consumer.write(MEDIA_PLAY_PAUSE);
    case 113:
      displayCurrentKey(0, "Play/Pause");
      break;
    default:
      if (key < 100) {
        Keyboard.press(key + A_OFFSET);
      } else {
        key -= 100; // Reset for actual preview
      }
      displayCurrentKey(0, key + A_OFFSET);
  }
}

void layer1(int key) {
  if (kpd.isKeyDown('B')) {
    key += 100;
  }
  switch (key) {
    case 1:
    case 101:
      displayCurrentKey(0, "Preview");
      break;
    case 5:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('d');
    case 105:
      displayCurrentKey(1, "GVC Mic");
      break;
    case 6:
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('/');
      Keyboard.press('r');
    case 106:
      displayCurrentKey(1, "Rename");
      break;
    case 13:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('/');
      Keyboard.press('g');
    case 113:
      displayCurrentKey(1, "Goto Line");
      break;
    default:
      if (key < 100) {
        Keyboard.press(key + ONE_OFFSET);
      } else {
        key -= 100; // Reset for actual preview
      }
      displayCurrentKey(1, key + ONE_OFFSET);
  }
}

void layer2(int key) {
  if (kpd.isKeyDown('B')) {
    key += 100;
  }
  switch (key) {
    case 1:
    case 101:
      displayCurrentKey(0, "Preview");
      break;
    default:
      if (key < 100) {
        Keyboard.press(key);
      } else {
        key -= 100; // Reset for actual preview
      }
      displayCurrentKey(1, key);
  }
}
