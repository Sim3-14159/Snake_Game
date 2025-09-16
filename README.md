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
- A 128×64 I2C single-color OLED display (SH1106 or SSD1306)
- Passive buzzer

Everything else is shown in the [schematic](images/schematic.svg) and can be a generic type.

### Notes

- If you use a microcontroller with ≤2kB of RAM, it will likely crash or maybe not even compile.
- If you are using another microcontroller besides an *Arduino UNO*, make sure you connect **SCK** and **SDA** to its **SCL** and **SDA** pins, which may or may not be on **A5** and **A4**.
- If you are using a _ instead of a _ screen, change the constructor from either `_` or `_` to `U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);` or `U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);`, respectively.
