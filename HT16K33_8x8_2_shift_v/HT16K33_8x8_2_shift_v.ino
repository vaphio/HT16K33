// test routine for HT16K33 dot LED driver
// for 8x8dotx2 LED
// made by Vaphio @ Oct.01, 2026
// updated @ Oct.4, 2026

#include <Wire.h>
#include "hankaku_v.h"

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA};
const uint8_t dotWidth = 16;

char msg[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefg";
const int msgSize = sizeof(msg) / sizeof(msg[0]) - 1;
unsigned long codeSize = msgSize * (charWidth + 1);
uint8_t msgCode[512];
uint8_t dotCode[dotWidth];
uint8_t dspCode[8];
int msgPos = 0;

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {    //setup command for HT16T33
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
  Serial.begin(9600);
  getStrCode(msgCode, msg);
}

void loop() {
  cutCode(dotCode, msgPos);
  rotCode(dspCode, dotCode);
  setLED(dspCode, 0);
//  setLED(dotCode, 1);
  delay(50);
  msgPos = (msgPos + 1) % codeSize;
}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void setLED(uint8_t* code, int dig) {
  for (int i=0; i<8; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2+dig);
    Wire.write(code[(i+dig*8)%codeSize]);
    Wire.endTransmission();
    delay(1);
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

void getStrCode(uint8_t* code, char* msg) {
  uint8_t buf[charWidth];
  int offset = 0;
  for (int i=0; i<msgSize; i++) {
    getCharCode(buf, msg[i]);
    for (int j=0; j<charWidth; j++) {
      code[j+offset] = buf[j];
    }
    code[charWidth+offset] = 0x00;
    offset = offset + charWidth + 1;
  }
}

void cutCode(uint8_t* result, int index) {
  for (int i=0; i<dotWidth; i++) {
    result[i] = msgCode[(i + index) % codeSize];
  }
}

void rotCode(uint8_t* result, uint8_t* base) {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      result[j] = result[j] | ((base[i] >> j) & 0x01);
      result[j] = result[j] << 1;
    }
  }
}

void setWindow(uint8_t* code, int offset, int size) {
//  Wire.beginTransmission(HT16K33_addr);
  for (int i=0; i<dotWidth; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2);
    Wire.write(code[(i+offset)%size]);
    Wire.write(code[(i+offset+8)%size]);
    Wire.endTransmission();
    delay(1);
  }
//  Wire.endTransmission();
}

void clearAll() {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(0x00);
  for (int i=0; i<16; i++) {
    Wire.write(0x00);
  }
  Wire.endTransmission();
}
