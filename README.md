# Snake_Game
#### This repository contains the code for my fast-paced Arduino-run _Snake_ as well as the circuit for it.  It is faster than most other games like it.
#### You can find the main file in [`Snake_Game.ino`](Snake_Game.ino)
<br>

This is the schematic:
![Unable to load image](images/schematic.svg)


## Dependencies

### Libraries

| Name | By | 
|--|--|
| [`U8g2`](https://github.com/olikraus/u8g2)| *olikraus* |
| `EEPROM` | Arduino |
| `Wire` | Arduino |


### Hardware

- An *Arduino UNO* compatible microcontroller board or any microcontroller with 6+ digital ports and I2C compatibility
- A 128×64 I2C single-color OLED display
- Passive buzzer

Everything else is shown in the [schematic](images/schematic.svg) and can be a generic type.

### Notes

- If you use a microcontroller with ≤2kB of RAM, it will likely crash or maybe not even compile.
- If you are using another microcontroller besides an *Arduino UNO*, make sure you connect **SCK** and **SDA** to its **SCL** and **SDA** pins, which may or may not be on **A5** and **A4**.
