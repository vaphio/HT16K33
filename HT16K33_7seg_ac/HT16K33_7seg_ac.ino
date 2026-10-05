// test routine for HT16K33 dot LED driver
// for 7seg anode common LED OSL10801-IRGB
// made by Vaphio @ Oct.5, 2026

#include <Wire.h>

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA1};
const uint8_t digit_code[16] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 
  0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};    // 7seg code without dp a:C0 ... g:C6
const uint8_t code_width = 7;

const uint8_t b_anode = 4;  // blue common pin is A2
const uint8_t g_anode = 8;  // green common pin is A3
const uint8_t r_anode = 16;  // red common pin is A4

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
  Serial.begin(9600);
//  Serial.println("Start");
}

void loop() {
  count(r_anode, 200, 16);
  delay(1000);
  count(g_anode, 200, 16);
  delay(1000);
  count(b_anode, 200, 16);
  delay(1000);
  count(r_anode | g_anode, 300, 10);
  delay(1000);
  count(g_anode | b_anode, 300, 10);
  delay(1000);
  count(0x14, 300, 10);
  delay(1000);
  count(0x0C, 300, 10);
  delay(1000);
  clearAll();
  delay(1000);
}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void count(uint8_t anode, int wait, int cmax) {
  uint8_t code;
  for (int i=0; i<cmax; i++) {
    code = digit_code[i];
    setLED(code, anode);
    delay(wait);
  }
}

void setLED(uint8_t code, uint8_t anode) {
  for (int i=0; i<code_width; i++) {
    Wire.beginTransmission(HT16K33_addr);
    if (((code >> i) & 1) == 1) {
      Wire.write(i*2);
      Wire.write(anode);
    } else {
      Wire.write(i*2);
      Wire.write(0x00);
    }
    Wire.endTransmission();
    delay(10);
  }
}

void clearAll() {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(0x00);
  for (int i=0; i<16; i++) {
    Wire.write(0x00);
  }
  Wire.endTransmission();
}