void processLayer(int layer, int key) {
  switch (layer) {
    case 0:
      layer0(key);
      break;

    case 1:
      layer1(key);
      break;
  }
}

void layer0(int key) {
  switch (key) {
    case 2:
      startAnimation();
      break;
    case 9:
      startScreenTest();
      break;
    case 4:
      System.write(SYSTEM_SLEEP);
      displayCurrentKey(0, "Sleep");
      break;
    case 5:
      Consumer.write(MEDIA_PREVIOUS);
      displayCurrentKey(0, "Previous");
      break;
    case 6:
      Consumer.write(MEDIA_NEXT);
      displayCurrentKey(0, "Next");
      break;
    case 7:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_F9);
      displayCurrentKey(0, "Audio Output");
      break;
    case 10:
      Consumer.write(MEDIA_VOLUME_DOWN);
      displayCurrentKey(0, "Vol Down");
      break;
    case 11:
      Consumer.write(MEDIA_VOLUME_UP);
      displayCurrentKey(0, "Vol Up");
      break;
    case 12:
      Consumer.write(MEDIA_VOLUME_MUTE);
      displayCurrentKey(0, "Mute");
      break;
    case 13:
      Consumer.write(MEDIA_PLAY_PAUSE);
      displayCurrentKey(0, "Play/Pause");
      break;
    default:
      Keyboard.press(key + A_OFFSET);
      displayCurrentKey(0, key + A_OFFSET);
  }
}

void layer1(int key) {
  switch (key) {
    case 6:
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('/');
      Keyboard.press('r');
      displayCurrentKey(1, "Rename");
      break;
    case 13:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('/');
      Keyboard.press('g');
      displayCurrentKey(1, "Goto Line");
      break;
    default:
      displayCurrentKey(1, key + ONE_OFFSET);
      Keyboard.press(key + ONE_OFFSET);
  }
}
