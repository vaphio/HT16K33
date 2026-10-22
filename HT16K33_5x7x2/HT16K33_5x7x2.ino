// test routine for HT16K33 dot LED driver
// for 8x8dot LED (ex. MOA20UB019G)
// made by Vaphio @ Oct.17, 2026
// 

#include <Wire.h>
#include "hankaku_v.h"

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA};
const int dotWidth = 10;    // 5x7 dot x 2pcs

char msg[] = "KBDFGK ";
int msgSize = sizeof(msg) / sizeof(msg[0]) - 1;
const long codeSize = msgSize * charWidth;
uint8_t msgCode[256];
uint8_t digCode1[8];
uint8_t digCode2[8];
uint8_t dspCode1[8];
uint8_t dspCode2[8];
int index = 0;

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
  Serial.begin(9600);
  getCharCode(digCode1, 'K');
  getCharCode(digCode2, 'G');
//  getStrCode(msgCode, msg);
//  cutCode(msgCode, dspCode, index);
  rotCode(digCode1, dspCode1);
  rotCode(digCode2, dspCode2);
  setLED(dspCode1,2);
  delay(1000);
//  prnCode(dspCode1);
}

void loop() {
  char str[] = "ABCDEFGK";
  int size = sizeof(str) / sizeof(str[0]) -1;
  for (int i=0; i<size; i++) {
    getCharCode(digCode1, str[i]);
    rotCode(digCode1, dspCode1);
    setLED(dspCode1, 2);
    delay(500);
  }
  delay(1000);

}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void setLED(uint8_t* code, int offset) {
  for (int i=0; i<charHeight; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2+offset);
    Wire.write(code[i]);
    Wire.endTransmission();
  }
}

void prnCode(uint8_t* code) {
  int stack;
  char s[charWidth];
  for (int i=0; i<charHeight; i++) {
    Serial.println(code[i], HEX);
  }
}

void getNumCode(uint8_t* code, int num) {
  for (int i=0; i<charWidth; i++) {
    code[i] = HANV[num+numOffset][i];
  }
}

void getCharCode(uint8_t* code, char alfa) {
  for (int i=0; i<charWidth; i++) {
    code[i] = HANV[alfa-codeOffset][i];
  }
}

void getStrCode(uint8_t* code, uint8_t* str) {
  uint8_t buf[charWidth];
  int offset = 0;
  for (int i=0; i<msgSize; i++) {
    getCharCode(buf, str[i]);
    for (int j=0; j<charWidth; j++) {
      code[j+offset] = buf[j];
    }
    offset += charWidth;
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

void cutCode(uint8_t* org, uint8_t* target, int index) {
  for (int i; i<dotWidth; i++) {
    target[i] = org[(i + index) % codeSize];
  }
}

void rotCode(uint8_t* org, uint8_t* target) {
  uint8_t stack;
  for (int i=0; i<8; i++) {
    target[i] = 0x00;
  }
  for (int i=0; i<charWidth; i++) {
    for (int j=0; j<charHeight; j++) {
      target[6-j] = target[6-j] | ((org[i] >> j) & 0x01);
      target[6-j] = target[6-j] << 1;
    }
  }
  for (int i=0; i<charHeight; i++) {
    target[i] = target[i] >> 1;
  }
}