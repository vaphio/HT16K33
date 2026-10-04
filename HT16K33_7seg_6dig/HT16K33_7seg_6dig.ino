// test routine for HT16K33 dot LED driver
// for 6dig cursor common LED OSL60362-LR
// made by Vaphio @ Sept.26, 2026
#include <Wire.h>

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA1};
const uint8_t digit_code[16] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0xDE, 0xEE, 0xF8, 0x72, 0xBC, 0xF2, 0xE2};
const int ndig = 6;

uint8_t dram[16];

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
  Serial.begin(9600);
  setNdig(123456L);
  delay(200);
//  Serial.println("Start");
}

void loop() {
  long base = 123456;
  for (int i=0; i<50; i++) {
    base++;
    setNdig(base);
    delay(100);
  }
  delay(1000);
  clearAll();
  for (int i=0; i<256; i++) {
    base++;
    setHex(base, 2);
    delay(100);
  }
  delay(1000);
  clearAll();
  delay(1000);
}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void sendBlockCmd(uint8_t* data) {
  int size = sizeof(data) / sizeof(data[0]);
  Wire.beginTransmission(HT16K33_addr);
  for (int i=0; i<size; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
  delay(10);
}

void setLED(int dig) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(0x00);
  for (int i=0; i<dig; i++) {
    Wire.write(dram[i]);
  }
  Wire.endTransmission();
}

void setNdig(long num) {
  int digit;
  for (int i=0; i<ndig; i++) {
    digit = num % 10;
    num = num / 10;
    setNum(ndig-1-i, digit);
  }
}

void setNum(int dig, int num) {
  int period = 0;
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(dig*2);
  Wire.write(digit_code[num] | period);
  Wire.endTransmission();
}

void setHex(long num, int cl) {
  int dig[ndig];
  for (int i=0; i<ndig; i++) {
    dig[ndig-i-1] = num %16;
    num = num / 16;
  }
  for (int i=0; i<ndig; i++) {
    setNum(i, dig[i]);
  }
}

void setDram(uint8_t data) {
  for (int i=0; i<16; i++) {
    dram[i] = data;
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