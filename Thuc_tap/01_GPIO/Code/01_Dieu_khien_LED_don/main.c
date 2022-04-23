// BOOKTS 
#include "io430f2350.h"

#define LEDS P3OUT              // Khai bao I/O

// Khai bao prototype ham
void delayms(int ms);
void blink(int time);
void shiftL();
void shiftR();
void brightShiftLeft();
void brightShiftRight();
void xenKe(int n);

// Dinh nghia chuong trinh chinh
int main( void ) {
  WDTCTL = WDTPW + WDTHOLD;      // Khoi dong chip
  
  P3DIR = 0xFF;
  blink(5);
  
  // Vong lap chuong trinh chinh
  while (1) {
      shiftL();                 // Dich trai LED
      delayms(500);             // delay 500ms
      shiftR();                 // Dich phai LED
      delayms(1000);
      brightShiftLeft();
      delayms(1000);
      brightShiftRight();
      delayms(1000);
      xenKe(5);
      delayms(1000);
  }
}

// Ham delayms
void delayms(int ms) {
  for(int i = 0; i<ms; i++) 
      __delay_cycles(100);
}
// Ham blink LED
void blink(int time) {
  for(int i=0; i<time; i++) {
  LEDS = 0xFF;  delayms(500);
  LEDS = 0x00;  delayms(500);
  }
}
// Ham dich trai
void shiftL() {
  unsigned char tbl[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
  for(int i=0; i<8; i++){
    LEDS = tbl[i];
    delayms(500);
  }
}
// Ham dich phai
void shiftR() {
  unsigned char tbl[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  
  for(int i=0; i<8; i++) {
    LEDS = tbl[i];
    delayms(500);
  }
}
// Ham LED sang dan tu phai sang trai
void brightShiftLeft() {
    LEDS = 0x01;
    delayms(500);
    unsigned char i;
    for (i = 0; i<8; i++) {
      LEDS = (LEDS<<1) + 0x01;
      delayms(500);
    }  
}
// Ham LED sang dan tu trai sang phai
void brightShiftRight() {
    LEDS = 0x80;
    delayms(500);
    unsigned char i;
    for (i = 0; i<8; i++) {
      LEDS = (LEDS>>1) + 0x80;
      delayms(500);
    }  
}
// Ham LED sang xen ke
void xenKe(int n) {
  for(int i = 0; i<n; i++) {
    LEDS = 0x55;        delayms(400);
    LEDS = 0xAA;        delayms(500);
  }
}