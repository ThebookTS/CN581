// BookTS
#include "io430f5529.h"

// Khai bao I/O
#define LEDS P3OUT
#define K1 P2IN_bit.P2IN3
#define K2 P2IN_bit.P2IN5
#define K3 P2IN_bit.P2IN4
#define K4 P4IN_bit.P4IN3
#define COM P4OUT_bit.P4OUT0

// Khai bao prototype ham
void delayms(int ms);
void blink(int time);
void shiftL();
void shiftR();
void brightShiftLeft();
void brightShiftRight();

void main(){
  
  // Khoi dong chip
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR = 0xFF;
  P2REN = BIT3|BIT4|BIT5;
  P2OUT = BIT3|BIT4|BIT5;
  P4REN = BIT3;
  P4OUT = BIT3;
  COM = 0;
  
  blink(5);
  while(1) {
    if (K1==0) LEDS = 1;
    if (K2==0) LEDS = 2;
    if (K3==0) LEDS = 3;
    if (K4==0) LEDS = 4;
    delayms(200);
  }
/*  
  while(1) {
    if (K1==0) {
      shiftL();
    }
    if (K2==0) {
      shiftR();
    }
    if (K3==0) {
      brightShiftRight();
    }
    if (K4==0) {  
      brightShiftLeft();
    }
    delayms(200);
  }     */ 
}

// Ham delayms
void delayms(int ms) {
  for(int i=0; i<ms ; i++)      __delay_cycles(1000);
}
// Ham blink LED
void blink(int time) {
  for(int i=0; i<time; i++) {
    LEDS = 0xFF;        delayms(500);
    LEDS = 0x00;        delayms(500);
  }
}
// Ham dich trai
void shiftL() {
  unsigned char tbl[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
  for(int i=0; i<8; i++) {
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