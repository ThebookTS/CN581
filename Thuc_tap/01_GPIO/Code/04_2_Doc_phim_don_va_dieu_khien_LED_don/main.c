// BookTS
#include "io430f5529.h"

// Khai bao I/O
#define LEDS P3OUT
#define K0 BIT3 // P2.3
#define K1 BIT5 // P2.5
#define K2 BIT4 // P2.4

#define COM P4OUT_bit.P4OUT0

// Khai bao prototype ham
void delayms(int ms);
void blink(int time);
void shiftL(int n);
void shiftR(int n);

unsigned char count;

void main() {

  // Khoi dong chip  
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR = 0xFF;         // Cau hinh I/O P3 = Output
  P4DIR = BIT0; 
  
  P2REN = BIT3|BIT4|BIT5;        // Bat R keo             
  P2OUT = BIT3|BIT4|BIT5;        // Keo len
  COM = 0;
  
  // Thiet lap ngat
  P2IE  = BIT3|BIT4|BIT5;        // Cho phep ngat tai P2.3 P2.4 P2.5
  P2IES = BIT3|BIT4|BIT5;        // Cho phep ngat canh xuong 
  P2IFG = 0x00;                  // Clear co ngat
  
  __bis_SR_register(GIE);        // cho phep ngat toan cuc
  count = 0;
  
  blink(5);
  shiftL(5);
  shiftR(3);
  
  // Vong lap chuong trinh chinh
  while(1) {
    delayms(200);
  }
}

// Khai bao vector ngat Port2
#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
  
  if ( P2IFG & K0 ) {
    if (count == 255) count = 0;
    else count ++;
    LEDS = count;
  }
  if ( P2IFG & K1 ) {
    if (count == 0) count = 255;
    else count --;
    LEDS = count;
  }
  if ( P2IFG & K2 ) {
    while(1) {
      if (count == 255) count = 0;
      else count ++;
      LEDS = count;
      delayms(1000);
    }
  }
  delayms(200);
  P2IFG = 0x00;         // Xoa co ngat
}

// Ham delayms
void delayms(int ms) {
  for(int i=0; i<ms ; i++)      __delay_cycles(1000);
}
// Ham blink LED
void blink(int time) {
  for(int i=0; i<time; i++) {
    LEDS = 0xFF; delayms(500);
    LEDS = 0x00; delayms(500);
  }
}
// Ham dich trai
void shiftL(int n) {
  unsigned char tbl[8] = {0x80,0x30,0x20,0x10,0x08,0x04,0x02,0x01};
  for(int i=0; i<n; i++) {
    LEDS = tbl[i];
    delayms(500);
  }
}
// Ham dich phai
void shiftR(int n) {
  unsigned char tbl[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  
  for(int i=0; i<n; i++) {
    LEDS = tbl[i];
    delayms(500);
  }
}