// BooKTS
// Read HCSR05
#include "msp430g2253.h"

int miliseconds;
int distance;
int sensor;

void main (void) {
 
  // Khoi dong chip
  WDTCTL = WDTPW + WDTHOLD;
  
  // Chon che do SMCLK va Upmode 
  TACTL = TASSEL_2 + ID_0 + MC_1;
  // Cho phep CCR0 ngat
  CCTL0 = CCIE;
  CCR0 = 1000;
  // Xoa tat ca cac co ngat
  P1IFG = 0x00;
  
  P2DIR |=  0x01;                 // P2.0 la LED
  P2OUT &= ~0x01;                 // LED muc 0
  
  _BIS_SR(GIE);                   // Cho phep ngat toan cuc
  
  while (1) {
    P1IE &= ~0x01;                // Ngat P1.0
    P1DIR |= 0x01;                // P1.0 la chan Trig
    P1OUT |= 0x01;                // Trig muc 1
    __delay_cycles(10);           // delay 10 (us)
    P1OUT &= ~0x01;               // Trig muc 0
    P1DIR &= ~0x04;               // P1.2 la chan Echo
    P1IFG = 0x00;                 // Xoa tat ca co ngat
    P1IE |= 0x04;                 // Cho phep ngat 
    P1IES &= ~0x04;
    __delay_cycles(30000);        // Delay 30 (ms)
    
    distance = sensor/58;         // Tinh toan khoang cach
    
    if (distance < 10 && distance != 0)         P2OUT |= 0x01;
    else        P2OUT &= ~0x01;
  }
}

// Chuong trinh con ngat
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
  if(P1IFG&0x04) {              // Cho xu ly
    if(!(P1IES&0x04)) {         // Co ngat hay khong
      TACTL |= TACLR;
      miliseconds = 0;
      P1IES |= 0x04;
    }
    else sensor = (long)miliseconds*1000 + (long)TAR;           // Do do rong xung
    P1IFG &= ~0x04;             // Xoa co ngat
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  miliseconds++;
}