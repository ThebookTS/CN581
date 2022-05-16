// BooKTS
#include "msp430g2253.h"

#define Trig    BIT0
#define Echo    BIT3
#define LED     BIT5

int miliseconds;
int distance;
int sensor;

void main (void) {
  
  // Khoi dong chip
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  CCTL0 = CCIE;
  CCR0 = 1000;
  TACTL = TASSEL_2 + MC_1;
  
  P1IFG = 0x00;
  
  P2DIR |= LED;                 // P2.5 output LED
  P2OUT &= ~LED;                // LOW Level LED
   
  while (1) {
    P1IE &= ~Trig;
    P1DIR |= Trig;
    P1OUT |= Trig;
    __delay_cycles(10);
    P1OUT &= ~Trig;
    P1DIR &= ~Echo;
    P1IFG = 0x00;
    P1IE |= Echo;
    P1IES &= ~Echo;
    __delay_cycles(30000);
    distance = sensor/58;
    
    if (distance < 10 && distance != 0)         P2OUT |= LED;
    else        P2OUT &= ~LED;
  }
}

// Chuong trinh con ngat
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
  if(P1IFG&Echo) {
    if(!(P1IES&Echo)) {
      TACTL |= TACLR;
      miliseconds = 0;
      P1IES |= Echo;
    }
    else sensor = (long)miliseconds*1000 + (long)TAR;
    P1IFG &= ~Echo;
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  miliseconds++;
}