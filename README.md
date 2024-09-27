# ESP32 with SigmaDSP and Equalizer


The purpose of this project is to replace the classic equalizer circuit (NE5532A/AP, OPA2604, etc), and be more versatile to suit personal preferences. 


# ADAU1701/1401
Here is the information on how to wire the ADAU1701/1401 board to the ESP32:
| ADAU1701/1401  | ESP32 |
|------|----------------|
| 5V | VIN or External Power Supply |
| SCL | GPIO 22 |
| SDA | GPIO 21 |
| GND | GND |

# Rotary Encoder
This project's default number of encoders is 4 (bass, middle, Treble, and volume), which are customizable based on your project.
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
| CLK | GPIO 16|
| DT | GPIO 4|
| GND | GND |

| Rotary Encoder (Treble) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 5|
| DT | GPIO 17|
| GND | GND |

| Rotary Encoder (Volume) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| CLK | GPIO 27|
| DT | GPIO 14|
| GND | GND |

# WS2812B Led Strip
Here is the information on how to wire the WS2812B Led Strip to the ESP32:
| WS2812B | ESP32 |
|------|----------------|
| V+ | VIN (5V) or 3.3V |
| Data In | GPIO 18 |
| GND | GND |


<img src="https://user-images.githubusercontent.com/72125448/217063951-24d0d14b-80ca-4c57-81b6-0bb9044d79c7.jpg" width=45% height=45%>
