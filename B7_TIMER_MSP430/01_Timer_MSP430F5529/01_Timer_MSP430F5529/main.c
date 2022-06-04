// BookTS
// Scan LED ngat timer 

#include "io430f5529.h"

//----for scan led7seg
#define DLED    P3OUT
#define CTR     P6OUT
#define K0      P2IN_bit.P2IN3
#define K1      P2IN_bit.P2IN5
#define K2      P2IN_bit.P2IN4
#define K3      P4IN_bit.P4IN3
#define COM     P4OUT_bit.P4OUT0

// Khai bao ham
void delayms(int ms);
void scanled();
void count2buff();

// Khai bao bien
//--bi?n cho hàm scanled
char tbl7seg[] ={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
char buff[4] = {0,0,0,0};       // vung dem led
int idx = 0;                    // con tro quet led
int timer = 0, counter = 0;

void main( void ){
  WDTCTL = WDTPW + WDTHOLD;
  
  // cau hinh IO
  P3DIR = 0xff;                 // P3 la OUTPUT
  P6DIR = 0xff;                 // P6 la OUTPUT
  P4DIR = BIT0;
  P4REN = BIT3; P4OUT = BIT3;
  P2REN = BIT3 + BIT4 + BIT5;
  P2OUT = BIT3 + BIT4 + BIT5;
  COM = 0;                      // KY0 = 0 (GND)
  
  // cau hinh Timer
  TA1CTL   = TASSEL_2 + ID_3 + MC_1;    // Mode up, SMCLK =1MHz, DIV8
  TA1CCTL0 = CCIE;
  TA1CCR0  = 624;               // T = 5ms
  __bis_SR_register(GIE);  

  while(1){  
    if(K1==0) {
      delayms(100); counter++;
    }
    if(K2==0) {
      delayms(100); counter--;
    }
    if(K3==0) {
      delayms(100); counter = 0;
    }
    count2buff();
  }
}
 // Khai bao vector ngat
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer0_A0 (void) {
  scanled();
}

// Ham quet LED
void scanled(){
char ch;
  ch = buff[idx];
  DLED = tbl7seg[ch];
  CTR = ~(1<<idx);
  idx++;
  if (idx>=4) idx = 0;
}

// Ham cap nhat hien thi counter=> buff
void count2buff(){
  buff[3] = counter/1000;
  buff[2] = (counter%1000)/100;
  buff[1] = ((counter%1000)%100)/10;
  buff[0] = counter%10;
}
// Ham delay miliseconds
void delayms(int ms) {
 for(int i=0; i<ms ; i++)
   __delay_cycles(1000);        // 1ms
}