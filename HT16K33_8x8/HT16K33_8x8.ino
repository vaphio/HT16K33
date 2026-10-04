// test routine for HT16K33 dot LED driver
// for 8x8dot LED (ex. MOA20UB018G)
// made by Vaphio @ Sept.30, 2026
// updated Oct.4, 202

#include <Wire.h>
#include "hankaku_v.h"

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA};
const uint8_t dotWidth = 8;

int speed = 60;
int scrl_size = 300;

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
  Serial.begin(9600);
}

void loop() {
  uint8_t code[charWidth];
  for (int i=0; i<10; i++) {
    getNumCode(code, i);
    setLED(code);
    delay(100);
  }
  delay(500);
  char alfa[] = " Hello World! ABD";
  uint8_t strWidth = sizeof(alfa) / sizeof(alfa[0]) - 1;
  for (int i=0; i<strWidth; i++) {
    getStrCode(code, alfa[i]);
    setLED(code);
    delay(300);
  }
  delay(1000);
  clearAll();

  long strCodeWidth = charWidth * strWidth;
  uint8_t scode[strCodeWidth];
  for (int i=0; i<strCodeWidth; i++) {
    scode[i] = 0x00;
  }
  setStr(alfa, scode, strWidth);
  for (int i=0; i<scrl_size; i++) {
    int offset = i % strCodeWidth;
    setWindow(scode, i, strCodeWidth);
    delay(speed);
  }
  delay(1000);
  clearAll();
}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void setLED(uint8_t* code) {
  for (int i=0; i<charWidth; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2);
    Wire.write(code[i]);
    Wire.endTransmission();
  }
}

void getNumCode(uint8_t* code, int num) {
  for (int i=0; i<charWidth; i++) {
    code[i] = HANV[num+numOffset][i];
  }
}

void getStrCode(uint8_t* code, char alfa) {
  for (int i=0; i<charWidth; i++) {
    code[i] = HANV[alfa-codeOffset][i];
  }
}

void setStr(char* str, uint8_t* scode, uint8_t size) {
  uint8_t buf[charWidth];
  for (int i=0; i<size; i++) {
    getStrCode(buf, str[i]);
    for (int j=0; j<charWidth; j++) {
      scode[j + i*charWidth] = buf[j];
    }
  }
}

void setWindow(uint8_t* code, int offset, int size) {
  for (int i=0; i<dotWidth; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2);
    Wire.write(code[(i+offset)%size]);
    Wire.endTransmission();
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
