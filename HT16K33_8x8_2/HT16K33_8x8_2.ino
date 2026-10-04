// test routine for HT16K33 dot LED driver
// for 8x8dotx2 LED
// made by Vaphio @ Oct.01, 2026
// updated @ Oct.4, 2026

#include <Wire.h>
#include "hankaku_v.h"

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA};
const uint8_t dotWidth = 16;

int speed = 50;
int scrl_size = 360;
char msg[] = "Hello, World! @Oct.04,2026 ";

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
  for (int i=0; i<9; i++) {
    getNumCode(code, i);
    setLED(code, 0);
    getNumCode(code, i+1);
    setLED(code, 1);
    delay(100);
  }
  clearAll();
  delay(500);
//  char alfa[] = " Hello, World! by Vaphio.";
  uint8_t strWidth = sizeof(msg) / sizeof(msg[0]) - 1;
  for (int i=0; i<strWidth-1; i++) {
    getStrCode(code, msg[i]);
    setLED(code, 0);
    delay(100);
    getStrCode(code, msg[i+1]);
    setLED(code, 1);
    delay(100);
  }
  delay(1000);
  clearAll();

  long strCodeWidth = charWidth * strWidth;
  uint8_t scode[strCodeWidth];
  setStr(msg, scode, strWidth);
  for (int i=0; i<scrl_size; i++) {
    int offset = i % strCodeWidth;
    setWindow(scode, i, strCodeWidth);
    delay(speed);
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

void setLED(uint8_t* code, int dig) {
  for (int i=0; i<charWidth; i++) {
    Wire.beginTransmission(HT16K33_addr);
    Wire.write(i*2+dig);
    Wire.write(code[i]);
    Wire.endTransmission();
    delay(1);
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
