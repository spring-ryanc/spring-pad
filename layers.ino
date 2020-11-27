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
  displayCurrentKey(0, key + 49);
  switch (key) {
    case 9:
      startscreentest();
      break;
    case 5:
      Consumer.write(MEDIA_PREVIOUS);
      displayCurrentKey(0, "Previous");
      break;
    case 6:
      Consumer.write(MEDIA_NEXT  );
      displayCurrentKey(0, "Next");
      break;
    case 10:
      Consumer.write(MEDIA_VOLUME_DOWN);
      displayCurrentKey(0, "Vol Down");
      break;
    case 11:
      Consumer.write(MEDIA_VOLUME_UP );
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
      Keyboard.press(key + 65);
  }
}

void layer1(int key) {
  displayCurrentKey(0, key + 97);
  Keyboard.press(key + 97);
}
