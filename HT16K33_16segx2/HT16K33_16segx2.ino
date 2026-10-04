// test routine for HT16K33 dot LED driver
// this sketch is designed for 16seg LED
// made by Vaphio @ Sept.26, 2026

#include <Wire.h>

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA};
const uint8_t digit_code[16][2] = {
  {0xFF, 0x90}, {0x31, 0x48}, {0x77, 0x03}, {0x3F, 0x02}, {0x8C, 0x03}, 
  {0xBB, 0x03}, {0xFB, 0x03}, {0x0F, 0x00}, {0xFF, 0x03}, {0xBF, 0x03},
  {0x0C, 0x92}, {0xF8, 0x03}, {0xF3, 0x00}, {0x3F, 0x48}, {0xF3, 0x01},
  {0xC3, 0x01}};
const uint8_t alfa_code[26][2] = {
  {0x0C, 0x92}, {0xF8, 0x03}, {0xF3, 0x00}, {0x3F, 0x48}, {0xF3, 0x01},
  {0xC3, 0x01}, {0xFB, 0x02}, {0xCC, 0x03}, {0x00, 0x48}, {0x7C, 0x00},
  {0xC0, 0x31}, {0xF0, 0x00}, {0xCC, 0x14}, {0xCC, 0x24}, {0xFF, 0x00},
  {0xC7, 0x03}, {0xFF, 0x20}, {0xC7, 0x23}, {0xBB, 0x03}, {0x03, 0x48},
  {0xFC, 0x00}, {0xC0, 0x90}, {0xCC, 0xA0}, {0x00, 0xB4}, {0x00, 0x54},
  {0x33, 0x90}};

uint8_t dram[16];
bool col;

void setup() {
  Wire.begin();
  for (uint8_t cmd : HT16K33_init) {
    sendCmd(cmd);
  }
  clearAll();
  delay(10);
//  Serial.begin(9600);
//  Serial.println("Start");
}

void loop() {
  for (int i=0; i<100; i++) {
    set2dig(i);
    delay(100);
  }
  delay(1000);
  clearAll();
  for (int i=0; i<256; i++) {
    set2Hex(i);
    delay(100);
  }
  delay(1000);
  
  char alfa[27] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  for (int i=0; i<25; i++) {
    setAlfa(2, alfa[i]);
    setAlfa(1, alfa[i+1]);
    delay(500);
  }
  delay(1000);
}

void sendCmd(uint8_t cmd) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(cmd);
  Wire.endTransmission();
}

void set2dig(int num) {
  setNum(1, num % 10);
  setNum(2, num / 10);
}

void set2Hex(int num) {
  setNum(1, num % 16);
  setNum(2, num / 16);
}

void setNum(int dig, int num) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(dig*2);
  Wire.write(digit_code[num][0]);
  Wire.write(digit_code[num][1]);
  Wire.endTransmission();
}

void setAlfa(int dig, char alfa) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(dig*2);
  Wire.write(alfa_code[alfa-'A'][0]);
  Wire.write(alfa_code[alfa-'A'][1]);
  Wire.endTransmission();
}

void clearAll() {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(0x00);
  for (int i=0; i<16; i++) {
    Wire.write(0x00);
  }
  Wire.endTransmission();
}