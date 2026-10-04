// test routine for HT16K33 dot LED driver
// made by Vaphio @ Sept.26, 2026
#include <Wire.h>

const uint8_t HT16K33_addr=0x70;
const uint8_t HT16K33_init[4] = {0x21, 0x81, 0xA0, 0xEA1};
const uint8_t digit_code[16] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0xDE, 0xEE, 0xF8, 0x72, 0xBC, 0xF2, 0xE2};

uint8_t dram[16];
bool col;

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
  int base = 1000;
  for (int i=0; i<50; i++) {
    base++;
    set4dig(base, 3);
    delay(1000);
    setColon(col);
    col = !col;
  }
  delay(1000);
  clearAll();
  for (int i=0; i<256; i++) {
    base++;
    setHex(base, 3);
    delay(100);
  }
  delay(1000);
  clearAll();
  for (int i=0; i<50; i++) {
    base++;
    set4dig(base, 1);
  //  set4dig(base, red);
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

void setColon(bool sw) {
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(0x08);
  if (sw) {
    Wire.write(0x06);
  } else {
    Wire.write(0x00);
  }
  Wire.endTransmission();
}

void set4dig(int num, int dec) {
  int dig[4];
  for (int i=0; i<4; i++) {
    dig[3-i] = num % 10;
    num = num / 10;
  }
  for (int i=0; i<4; i++) {
    setNum(i, dig[i], dec);
  }
}

void setNum(int dig, int num, int dec) {
  int period = 0;
  Wire.beginTransmission(HT16K33_addr);
  Wire.write(dig*2);
  if (dig==dec) {
    period = 1;
  } else {
    period = 0;
  }
  Wire.write(digit_code[num] | period);
  Wire.endTransmission();
}

void setHex(int num, int cl) {
  int dig[4];
  for (int i=0; i<4; i++) {
    dig[3-i] = num %16;
    num = num / 16;
  }
  for (int i=0; i<4; i++) {
    setNum(i, dig[i], cl);
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