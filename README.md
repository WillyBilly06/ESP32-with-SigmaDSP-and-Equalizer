# ESP32 with SigmaDSP and Equalizer


The purpose of this project is to replace the classic equalizer circuit (NE5532A/AP, OPA2604, etc), and be more versatile in order to suit personal preferences. 


# ADAU1701/1401
Here is the information on how to wire the ADAU1701/1401 board to the ESP32:
| ADAU1701/1401  | ESP32 |
|------|----------------|
| 5V | VIN or External Power Supply |
| SCL | GPIO 22 |
| SDA | GPIO 21 |
| GND | GND |

# Rotary Encoder
The default amount of encoders for this project is 4 (Bass, Middle, Treble, and Volume), and these are customizable based on your project.
For additional information, here is the current wiring for 4 encoders:

| Rotary Encoder (Bass) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| SCL | GPIO 21 |
| SDA | GPIO 22 |
| GND | GND |

| Rotary Encoder (Middle) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| SCL | GPIO 21 |
| SDA | GPIO 22 |
| GND | GND |

| Rotary Encoder (Treble) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| SCL | GPIO 21 |
| SDA | GPIO 22 |
| GND | GND |

| Rotary Encoder (Volume) | ESP32 |
|------|----------------|
| VIN | 3.3V |
| SCL | GPIO 21 |
| SDA | GPIO 22 |
| GND | GND |
