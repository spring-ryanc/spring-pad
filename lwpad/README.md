# Setup IDE (code editor)
Follow the instructions at this [link](https://learn.sparkfun.com/tutorials/qwiic-pro-micro-usb-c-atmega32u4-hookup-guide/setting-up-arduino)

Note: pay special attention to select the right board and processor. If you dont select the right (5V) version, it will likely end up bricked.

# Install Code Dependencies
We will use some other peoples code to control certain things so we don't have to write it outselves.

Guide to install: [Install Arduino Libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)

You need to download and install these libraries:
* https://github.com/NicoHood/HID
* https://github.com/adafruit/Adafruit-GFX-Library
* https://github.com/adafruit/Adafruit_SH110X

# Create New Project
In the Arduino IDE (that you just finished setting up), create a new project. Note that the name of the project will be the name of the folder its in and the name of the main code file (ex. MyProject creates ./MyProject/MyProject.ino).

Copy the code from lwpad.ino into that file, compile, fix any errors, ensure you have the correct board/processor/port selected. Upload!

Note: Remember the warning about the sudden volume change!