# spring-pad
![image of macropad](https://github.com/spring-ryanc/spring-pad/blob/docs/docs/macropad.jpg?raw=true)

https://github.com/spring-ryanc/spring-pad/blob/docs/docs/PXL_20201205_223118897.jpg
DIY macropad based on [Spark Fun Qwiic Keyboard Explorer](https://www.sparkfun.com/products/17251) which uses a Pro Micro to process input from mechanical keys and a [rotary encoder](https://www.sparkfun.com/products/15140) I added. A small [OLED display](https://www.sparkfun.com/products/17153) is used to show information about the current key and a fun screen saver when not in use. In hindsight I would have ordered a larger screen.

## Parts List
- [Spark Fun Qwiic Keyboard Explorer](https://www.sparkfun.com/products/17251)
- [Rotary encoder](https://www.sparkfun.com/products/15140)
- [OLED display (0.91 in, 128x32)](https://www.sparkfun.com/products/17153)
- Solder
- A bit of wire for the encoder
- JST wire for the Qwiic connection for the display ([kit](https://www.sparkfun.com/products/15081))

## Assmembly
- Solder the pro micro to the supplied PCB (I suggest doing this first, it was a tight fit when the keys were in place)
- Solder all diodes to the PCB, be sure they are in the right direction
- Place all switches in the supplied face plate
- Solder each switch to the PCB from below
- Solder the mode switch button from below
- Mount the encoder how you like (I soldered it directly to the PCB after gently encouraging the pins to fit)
- Wire the three encoder pins (A C B) to (D10, GROUND, D16)
  - That means the middle pin is ground which is available on the ground rail
  - D10 and D16 are available from the PCB
- Plug in the display using the JST Qwiic connection
