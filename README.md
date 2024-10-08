# ESP32 with SigmaDSP and Equalizer


This project aims to be more adaptable to people's preferences by utilizing others' libraries to replace the traditional equalizer circuit (NE5532A/AP, OPA2604, etc). 

# Table of Contents
* [How to connect ADAU14/1701 board?](https://github.com/WillyBilly06/ESP32-with-SigmaDSP-and-Equalizer?tab=readme-ov-file#adau17011401)
* [How to connect Rotary Encoder?](https://github.com/WillyBilly06/ESP32-with-SigmaDSP-and-Equalizer?tab=readme-ov-file#rotary-encoder)
* [How to connect WS2812B Strip](https://github.com/WillyBilly06/ESP32-with-SigmaDSP-and-Equalizer?tab=readme-ov-file#ws2812b-led-strip)
* [What Libraries are required for this project?](https://github.com/WillyBilly06/ESP32-with-SigmaDSP-and-Equalizer?tab=readme-ov-file#libraries-requirement)
* [LICENSE](https://github.com/WillyBilly06/ESP32-with-SigmaDSP-and-Equalizer/blob/main/LICENSE)

# ADAU1701/1401
<img src="https://github.com/user-attachments/assets/1dbf24c0-6796-4ced-b171-13d30fe7abb2" width=45% height=45%>

Here is the information on how to wire the ADAU1701/1401 board to the ESP32:
| ADAU1701/1401  | ESP32 |
|------|----------------|
| 5V | VIN or External Power Supply |
| SCL | GPIO 22 |
| SDA | GPIO 21 |
| GND | GND |

# Rotary Encoder
Four encoders (bass, center, treble, and volume) are the default amount; however, you can change this number to suit your needs.
For additional information, here is the current wiring for 4 encoders:

<img src="https://user-images.githubusercontent.com/72125448/213977013-4a5b1336-3c5c-4319-94e6-383daaa219d4.jpg" width=45% height=45%>

| Rotary Encoder (Bass) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 21 |
| DT | GPIO 22 |
| GND | GND |

| Rotary Encoder (Middle) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 16 |
| DT | GPIO 4 |
| GND | GND |

| Rotary Encoder (Treble) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 5 |
| DT | GPIO 17 |
| GND | GND |

| Rotary Encoder (Volume) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 27 |
| DT | GPIO 14 |
| GND | GND |

# WS2812B LED Strip
Regarding the LED, the Arduino file sets that there will be four strips. Each strip will include eleven LEDs in total, with the first LED of the four strips being illuminated to indicate a zero. It is also advised to use an external power source because the LEDs will take a significant amount of current.
For additional information, here is the current wiring for 4 LED strips:

<img src="https://user-images.githubusercontent.com/72125448/217063951-24d0d14b-80ca-4c57-81b6-0bb9044d79c7.jpg" width=45% height=45%>

| WS2812B (Bass) | ESP32 |
|------|----------------|
| VCC | External 5V Power Supply|
| DIN | GPIO 33 |
| GND | GND |

| WS2812B (Middle) | ESP32 |
|------|----------------|
| VCC | External 5V Power Supply |
| DIN | GPIO 32 |
| GND | GND |

| WS2812B (Treble) | ESP32 |
|------|----------------|
| VCC | External 5V Power Supply |
| DIN | GPIO 18 |
| GND | GND |

| WS2812B (Volume) | ESP32 |
|------|----------------|
| VCC | External 5V Power Supply |
| DIN | GPIO 19 |
| GND | GND |

# Libraries Requirement
https://github.com/MCUdude/SigmaDSP <br />
https://github.com/igorantolic/ai-esp32-rotary-encoder <br />
https://github.com/adafruit/Adafruit_NeoPixel
# LICENSE
Apache License

