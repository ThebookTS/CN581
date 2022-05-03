// BookTS
#include <io430.h>

// Khai bao port I/O
#define DLED P3OUT
#define CTR P6OUT
#define K0 P2IN_bit.P2IN3
#define K1 P2IN_bit.P2IN5
#define K2 P2IN_bit.P2IN4
#define K3 P4IN_bit.P4IN3
#define COM P4OUT_bit.P4OUT0

// Khai bao ptototype ham
void delayms(int ms);
void scanled();
void showClock();

// Khai bao bien toan cuc
char tbl7seg[] = {0xC0,0xF9,0xA4,0xB0,0x99,
0x92,0x82,0xF8,0x80,0x90};
char buff[4] = {0,0,0,0};                               // vung dem led
int idx = 0;                                            // con tro quet led
int second, msecond, ticker;
int status = 0;

void main() {
  
  // Khoi dong chip
  WDTCTL = WDTPW + WDTHOLD;             // tat watchdog
  P3DIR = 0xff;                         // P2 la OUTPUT
  P6DIR = 0xff;                         // P2 la OUTPUT
  P4DIR = BIT0;
  P4REN = BIT3; P4OUT = BIT3;
  P2REN = BIT3 + BIT4 + BIT5;
  P2OUT = BIT3 + BIT4 + BIT5;
  COM = 0;                              // KY0 = 0 (GND)
  
  // Khoi dong timer
  TA0CTL = TASSEL_2 + ID_3 + MC_1;      // Mode up
  TA0CCTL0 = CCIE;
  TA0CCR0 = 625;                        // Cai dat timer ngat 5ms
  __bis_SR_register(GIE);
  
  // Khoi dong bien
  ticker = 0; second = 0; msecond = 0; status = 0;

  // Vong lap chuong trinh chinh
  while(1){
    // Nhan va xy ly phim an
    if(K1==0) { 
      delayms(100); status = 1;
    }
    if(K2==0) { 
      delayms(100); status = 0;
    }
    if(K3==0) {
      delayms(100);
      status = 0;
      second = 0; msecond = 0; ticker = 0;
      showClock();
    }
    delayms(5);
  }
/*
  while(1) {
    // Nhan va xu ly phim
    if(K1==0) {
      status =! status;
    }
    if(K2==0) {
      delayms(100);
      status = 0;
      second = 0; msecond = 0; ticker = 0;
      showClock();
    }
    delayms(5);
  }
*/
}

// Vector ngat
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void) {
  
  // quet led 5ms
  scanled();
  
  // count for 10ms
  if (status==1) {
    ticker++;
    if (ticker>=1) { // reach 10ms => count for msecond
      ticker = 0;
      msecond++;
      if (msecond>=100) {
        msecond = 0;
        second++;
      }
    showClock();
    }
  }
}

// Ham delay
void delayms(int ms){
  for(int i=0;i<ms;i++)
  __delay_cycles(1000);
}
//--------------------------
void scanled() {
  char ch;
  ch = buff[idx];
  DLED = tbl7seg[ch];
  CTR = ~(1<<idx);
  idx++;
  if (idx>=4) idx = 0;
}
//-----------------------------
void showClock() {
  buff[3] = second/10;
  buff[2] = second%10;
  buff[1] = msecond/10;
  buff[0] = msecond%10;
}
  