/**********************************************|
| SigmaDSP library                             |
| https://github.com/MCUdude/SigmaDSP          |
|                                              |
| 3_Second_order_EQ.ino                        |
| This example controls a four band EQs and    |
| a master volume slider.                      |
|                                              |
| See the SigmaStudio project file if you want |
| to learn more, tweak or do modifications.    |
|**********************************************/

// Include Wire and SigmaDSP library
#include <Wire.h>
#include <SigmaDSP.h>
#include "EEPROM.h"
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include "EEPROM.h"
#include <Adafruit_NeoPixel.h>
// Include generated parameter file
#include "SigmaDSP_parameters.h"

#define EEPROM_SIZE 1024

int bass = 0;
int middle = 0;
int treble = 0;
int volume = 0;

int8_t bass_volume;
int8_t bass_volume1;

int8_t middle_volume;

int8_t treble_volume;
int8_t treble_volume1;

int8_t main_volume;


#define ROTARY_ENCODER_A_PIN1 16
#define ROTARY_ENCODER_B_PIN1 4
#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

#define ROTARY_ENCODER_A_PIN2 5
#define ROTARY_ENCODER_B_PIN2 17

#define ROTARY_ENCODER_A_PIN3 27
#define ROTARY_ENCODER_B_PIN3 14

#define ROTARY_ENCODER_A_PIN4 25
#define ROTARY_ENCODER_B_PIN4 26

#define BASS_LED 33
#define MIDDLE_LED 32
#define TREBLE_LED 18
#define VOLUME_LED 19

#define NUMPIXELS1 11  //Number of pixels in WS2812B
#define NUMPIXELS2 11  //Number of pixels in WS2812B
#define NUMPIXELS3 11  //Number of pixels in WS2812B
#define NUMPIXELS4 11  //Number of pixels in WS2812B

#define ROTARY_ENCODER_STEPS 7

AiEsp32RotaryEncoder rotaryEncoder1 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN1, ROTARY_ENCODER_B_PIN1, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder2 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN2, ROTARY_ENCODER_B_PIN2, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder3 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN3, ROTARY_ENCODER_B_PIN3, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder4 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN4, ROTARY_ENCODER_B_PIN4, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

Adafruit_NeoPixel pixels1(NUMPIXELS1, BASS_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMPIXELS1, TREBLE_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3(NUMPIXELS1, MIDDLE_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels4(NUMPIXELS1, VOLUME_LED, NEO_GRB + NEO_KHZ800);


// The first parameter is the Wire object we'll be using when communicating wth the DSP
// The second parameter is the DSP i2c address, which is defined in the parameter file
// The third parameter is the sample rate
// An optional fourth parameter is the pin to physically reset the DSP
SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f /*,12*/);

// Only needed if an external i2c EEPROM is present + the DSP is in selfboot mode
// The first parameter is the Wire object we'll be using when communicating wth the EEPROM
// The second parameter is the EEPROM i2c address, which is defined in the parameter file
// The third parameter is the EEPROM size in kilobits (kb)
// An optional fourth parameter is the pin to toggle while writing content to EEPROM
DSPEEPROM ee(Wire, EEPROM_I2C_ADDRESS, 256, 2);


// Create an instance for each EQ block
secondOrderEQ eqBand1;
secondOrderEQ eqBand2;
secondOrderEQ eqBand3;
secondOrderEQ eqBand4;
secondOrderEQ eqBand5;
secondOrderEQ eqBand6;
secondOrderEQ eqBand7;





#define EEPROM_SIZE 64


uint32_t Wheel1(byte WheelPos1) {
  WheelPos1 = 255 - WheelPos1;
  if (WheelPos1 < 85) {
    return pixels1.Color(255,195,0);
  }
  if (WheelPos1 < 170) {
    WheelPos1 -= 85;
    return pixels1.Color(255,195,0);
  }
  WheelPos1 -= 170;
  return pixels1.Color(255,195,0);
}

uint32_t Wheel2(byte WheelPos2) {
  WheelPos2 = 255 - WheelPos2;
  if (WheelPos2 < 85) {
    return pixels2.Color(255,195,0);
  }
  if (WheelPos2 < 170) {
    WheelPos2 -= 85;
    return pixels2.Color(255,195,0);
  }
  WheelPos2 -= 170;
  return pixels2.Color(255,195,0);
}

uint32_t Wheel3(byte WheelPos3) {
  WheelPos3 = 255 - WheelPos3;
  if (WheelPos3 < 85) {
    return pixels3.Color(255,195,0);
  }
  if (WheelPos3 < 170) {
    WheelPos3 -= 85;
    return pixels3.Color(255,195,0);
  }
  WheelPos3 -= 170;
  return pixels3.Color(255,195,0);
}

uint32_t Wheel4(byte WheelPos4) {
  WheelPos4 = 255 - WheelPos4;
  if (WheelPos4 < 85) {
    return pixels4.Color(255,0,0);
  }
  if (WheelPos4 < 170) {
    WheelPos4 -= 85;
    return pixels4.Color(255,0,0);
  }
  WheelPos4 -= 170;
  return pixels1.Color(255,0,0);
}

void rotary_loop1() {
  EEPROM.commit();
  //dont print anything unless value changed
  for (byte i = 0; i < 11; i++) {
    if (bass == 0) {
      pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
    } else if (bass > 0) {
      uint32_t pixelColor = (bass > i) ? Wheel1((10 - i) * 11) : 0;
      pixels1.setPixelColor(i, pixelColor);
    } else {
      uint32_t pixelColor = (bass < -i) ? Wheel1((i + 1) * 11) : 0;
      pixels1.setPixelColor(11 - i, pixelColor);
    }
  }
  pixels1.show();
  int16_t encoderDelta1 = rotaryEncoder1.encoderChanged();
  //optionally we can ignore whenever there is no change
  if (encoderDelta1 == 0)
    return;

  //for some cases we only want to know if value is increased or decreased (typically for menu items)
  if (encoderDelta1 > 0) {
    bass++;
    if (bass > 11)
      bass = 11;
    EEPROM.write(0, bass);
    EEPROM.commit();
    Serial.print("Bass:");
    Serial.println(bass);
    bass_volume += 2;
    if (bass_volume > 4)
      bass_volume = 4;
    EEPROM.write(5, bass_volume);
    EEPROM.commit();
    bass_volume1 +=2;
    if (bass_volume1 > 0)
    bass_volume1 = 0;
    EEPROM.write(1, bass_volume1);
    EEPROM.commit();
  }
  if (encoderDelta1 < 0) {
    bass--;
    if (bass < 1)
      bass = 1;
    EEPROM.write(0, bass);
    EEPROM.commit();
    Serial.print("Bass:");
    Serial.println(bass);
    bass_volume -= 0;
    if (bass_volume < -16)
      bass_volume = -16;
    EEPROM.write(5, bass_volume);
    EEPROM.commit();
    bass_volume1 -=2;
    if (bass_volume1 < -20)
    bass_volume1 = -20;
    EEPROM.write(1, bass_volume1);
  }
  //if value is changed compared to our last read
  Serial.print(bass_volume);
  Serial.println(bass_volume1);
  eqBand1.freq = 45;
  eqBand1.boost = bass_volume;
  eqBand1.state = parameters::state::on;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE0_B0_ADDR, eqBand1);
  eqBand2.freq = 70;
  eqBand2.boost = bass_volume1;
  // EQ is on by default, so no need to explicit turn it on
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE1_B0_ADDR, eqBand2);
}
/*----------------------------------------*/
void rotary_loop2() {
  EEPROM.commit();
  //dont print anything unless value changed
  for (byte i = 0; i < 11; i++) {
    if (middle == 0) {
      pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
    } else if (middle > 0) {
      uint32_t pixelColor = (middle > i) ? Wheel2((10 - i) * 11) : 0;
      pixels2.setPixelColor(i, pixelColor);
    } else {
      uint32_t pixelColor = (middle < -i) ? Wheel2((i + 1) * 11) : 0;
      pixels2.setPixelColor(11 - i, pixelColor);
    }
  }
  pixels2.show();
  int16_t encoderDelta1 = rotaryEncoder2.encoderChanged();
  //optionally we can ignore whenever there is no change
  if (encoderDelta1 == 0)
    return;

  //for some cases we only want to know if value is increased or decreased (typically for menu items)
  if (encoderDelta1 > 0) {
    middle++;
    if (middle > 11)
      middle = 11;
    EEPROM.write(10, middle);
    EEPROM.commit();
    Serial.print("Middle:");
    Serial.println(middle);
    middle_volume += 2;
    if (middle_volume > 4)
      middle_volume = 4;
    EEPROM.write(15, middle_volume);
    EEPROM.commit();
  }
  if (encoderDelta1 < 0) {
    middle--;
    if (middle < 1)
      middle = 1;
    EEPROM.write(10, middle);
    EEPROM.commit();
    Serial.print("Middle:");
    Serial.println(middle);
    middle_volume -= 2;
    if (middle_volume < -16)
      middle_volume = -16;
    EEPROM.write(15, middle_volume);
    EEPROM.commit();
  }
  //if value is changed compared to our last read
  Serial.print(middle_volume);
  eqBand3.freq = 475;
  eqBand3.boost = middle_volume;
  // EQ is on by default, so no need to explicit turn it on
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE2_B0_ADDR, eqBand3);
}
/*----------------------------------------*/
void rotary_loop3() {
  EEPROM.commit();
  //dont print anything unless value changed
  for (byte i = 0; i < 11; i++) {
    if (treble == 0) {
      pixels3.setPixelColor(i, pixels3.Color(0, 0, 0));
    } else if (treble > 0) {
      uint32_t pixelColor = (treble > i) ? Wheel3((10 - i) * 11) : 0;
      pixels3.setPixelColor(i, pixelColor);
    } else {
      uint32_t pixelColor = (treble < -i) ? Wheel3((i + 1) * 11) : 0;
      pixels3.setPixelColor(11 - i, pixelColor);
    }
  }
  pixels3.show();
  int16_t encoderDelta1 = rotaryEncoder3.encoderChanged();
  //optionally we can ignore whenever there is no change
  if (encoderDelta1 == 0)
    return;

  //for some cases we only want to know if value is increased or decreased (typically for menu items)
  if (encoderDelta1 > 0) {
    treble++;
    if (treble > 11)
      treble = 11;
    EEPROM.write(20, treble);
    EEPROM.commit();
    Serial.print("Treble:");
    Serial.println(treble);
    treble_volume += 2;
    if (treble_volume > 0)
      treble_volume = 0;
    EEPROM.write(25, treble_volume);
    EEPROM.commit();
    treble_volume1 +=2;
    if (treble_volume1 > 2)
    treble_volume1 = 2;
    EEPROM.write(26, treble_volume1);
    EEPROM.commit();
  }
  if (encoderDelta1 < 0) {
    treble--;
    if (treble < 1)
      treble = 1;
    EEPROM.write(20, treble);
    EEPROM.commit();
    Serial.print("Treble:");
    Serial.println(treble);
    treble_volume -= 2;
    if (treble_volume < -20)
      treble_volume = -20;
    EEPROM.write(25, treble_volume);
    EEPROM.commit();
    if (treble_volume1 < -18)
    treble_volume1 = -18;
    EEPROM.write(26, treble_volume1);
    EEPROM.commit();
  }
  //if value is changed compared to our last read
  Serial.print(treble_volume);
  eqBand4.freq = 2300;
  // EQ is on by default, so no need to explicit turn it on
  eqBand4.boost = treble_volume;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE3_B0_ADDR, eqBand4);
  eqBand5.freq = 20000;
  eqBand5.boost = treble_volume1;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE4_B0_ADDR, eqBand5);
  eqBand6.freq = 15000;
  eqBand6.boost = treble_volume1;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE5_B0_ADDR, eqBand6);
  eqBand7.freq = 8600;
  eqBand7.boost = treble_volume1;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE6_B0_ADDR, eqBand7);
}
/*----------------------------------------*/
void rotary_loop4() {
  EEPROM.commit();
  //dont print anything unless value changed
  for (byte i = 0; i < 11; i++) {
    if (volume == 0) {
      pixels4.setPixelColor(i, pixels4.Color(0, 0, 0));
    } else if (volume > 0) {
      uint32_t pixelColor = (volume > i) ? Wheel4((10 - i) * 11) : 0;
      pixels4.setPixelColor(i, pixelColor);
    } else {
      uint32_t pixelColor = (volume < -i) ? Wheel4((i + 1) * 11) : 0;
      pixels4.setPixelColor(11 - i, pixelColor);
    }
  }
  pixels4.show();
  int16_t encoderDelta1 = rotaryEncoder4.encoderChanged();
  //optionally we can ignore whenever there is no change
  if (encoderDelta1 == 0)
    return;

  //for some cases we only want to know if value is increased or decreased (typically for menu items)
  if (encoderDelta1 > 0) {
    volume++;
    if (volume > 11)
      volume = 11;
    EEPROM.write(30, volume);
    EEPROM.commit();
    Serial.print("Volume:");
    Serial.println(volume);
    main_volume += 2;
    if (main_volume > -2)
      main_volume = -2;
    if (main_volume < -20)
      main_volume = -20;
    EEPROM.write(35, main_volume);
    EEPROM.commit();
  }
  if (encoderDelta1 < 0) {
    volume--;
    if (volume < 1)
      volume = 1;
    EEPROM.write(30, volume);
    EEPROM.commit();
    Serial.print("Volume:");
    Serial.println(volume);
    main_volume -= 2;
    if (main_volume < -20)
      main_volume = -60;
    EEPROM.write(35, main_volume);
    EEPROM.commit();
  }
  //if value is changed compared to our last read
  Serial.print(main_volume);
  dsp.volume_slew(MOD_SWVOL1_ALG0_TARGET_ADDR, main_volume);
  dsp.volume_slew(MOD_SWVOL1_ALG1_TARGET_ADDR, main_volume);
}
/*-----------------------------------------*/
void IRAM_ATTR readEncoderISR() {
  rotaryEncoder1.readEncoder_ISR();
  rotaryEncoder2.readEncoder_ISR();
  rotaryEncoder3.readEncoder_ISR();
  rotaryEncoder4.readEncoder_ISR();
}


void setup() {
  Serial.begin(115200);



  Wire.begin();
  dsp.begin();
 // ee.begin();
  Serial.println(F("Pinging i2c lines...\n0 -> deveice is present\n2 -> device is not present"));
  Serial.print(F("DSP response: "));
  Serial.println(dsp.ping());
  Serial.print(F("EEPROM ping: "));
  Serial.println(ee.ping());
  EEPROM.begin(EEPROM_SIZE);

  bass = EEPROM.read(0);
  bass_volume = EEPROM.read(5);
  bass_volume1 = EEPROM.read(1);

  middle = EEPROM.read(10);
  middle_volume = EEPROM.read(15);

  treble = EEPROM.read(20);
  treble_volume = EEPROM.read(25);
  treble_volume1 = EEPROM.read(26);

  volume = EEPROM.read(30);
  main_volume = EEPROM.read(35);

  // Use this step if no EEPROM is present
 // 
 // 
  ///
loadProgram(dsp);

  pixels1.begin();
  pixels2.begin();
  pixels3.begin();
  pixels4.begin();
  // Comment out the three code lines above and use this step instead if EEPROM is present
  // The last parameter in writeFirmware is the FW version, and prevents the MCU from overwriting on every reboot
  //Serial.print(F("\nLoading DSP program... "));
  //ee.writeFirmware(DSP_eeprom_firmware, sizeof(DSP_eeprom_firmware), 0);
  //dsp.reset();
  //delay(2000);
  //Serial.println("Done!\n"); // Wait for the FW to load from the EEPROM
  // Initilize EQ band 1
  eqBand1.filterType = parameters::filterType::lowShelf;
  eqBand1.S = 1;
  eqBand1.Q = 0.7071;

  // Initilize EQ band 2
  eqBand2.filterType = parameters::filterType::peaking;
  eqBand2.Q = 1.41;

  // Initilize EQ band 3
  eqBand3.filterType = parameters::filterType::peaking;
  eqBand3.Q = 1.41;

  // Initilize EQ band 4
  eqBand4.filterType = parameters::filterType::peaking;
  eqBand4.Q = 1.41;

  // Initilize EQ band 5
  eqBand5.filterType = parameters::filterType::peaking;
  eqBand5.Q = 1.41;

  // Initilize EQ band 6
  eqBand6.filterType = parameters::filterType::peaking;
  eqBand6.Q = 1.41;

  eqBand7.filterType = parameters::filterType::peaking;
  eqBand7.Q = 1.41;

  // Set volume to 0dB




  eqBand1.freq = 45;
  eqBand1.boost = bass_volume;
  eqBand1.state = parameters::state::on;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE0_B0_ADDR, eqBand1);
  eqBand2.freq = 70;
  eqBand2.boost = bass_volume1;
  // EQ is on by default, so no need to explicit turn it on
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE1_B0_ADDR, eqBand2);

  eqBand3.freq = 475;
  eqBand3.boost = middle_volume;
  // EQ is on by default, so no need to explicit turn it on
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE2_B0_ADDR, eqBand3);

  eqBand4.freq = 2300;
  // EQ is on by default, so no need to explicit turn it on
  eqBand4.boost = treble_volume;
  dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE3_B0_ADDR, eqBand4);
             
 eqBand5.freq = 20000;
 eqBand5.boost = treble_volume1;
 dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE4_B0_ADDR, eqBand5);

 eqBand6.freq = 15000;
 eqBand6.boost = treble_volume1;
 dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE5_B0_ADDR, eqBand6);

 eqBand7.freq = 8600;
 eqBand7.boost = treble_volume1;
 dsp.EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE6_B0_ADDR, eqBand7);

  dsp.volume_slew(MOD_SWVOL1_ALG0_TARGET_ADDR, main_volume);
  dsp.volume_slew(MOD_SWVOL1_ALG1_TARGET_ADDR, main_volume);
  
  rotaryEncoder1.begin();
  rotaryEncoder1.setup(readEncoderISR);
  rotaryEncoder2.begin();
  rotaryEncoder2.setup(readEncoderISR);
  rotaryEncoder3.begin();
  rotaryEncoder3.setup(readEncoderISR);
  rotaryEncoder4.begin();
  rotaryEncoder4.setup(readEncoderISR);

  bool circleValues = false;
  rotaryEncoder1.setBoundaries(-1000, 1000, circleValues);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder1.setAcceleration(0);                        //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  rotaryEncoder2.setBoundaries(-1000, 1000, circleValues);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder2.setAcceleration(0);                        //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  rotaryEncoder3.setBoundaries(-1000, 1000, circleValues);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder3.setAcceleration(0);                        //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  rotaryEncoder4.setBoundaries(-1000, 1000, circleValues);  //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder4.setAcceleration(0);                        //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
}

void loop() {
  EEPROM.commit();
  rotary_loop1();
  rotary_loop2();
  rotary_loop3();
  rotary_loop4();
  EEPROM.commit();
}
