// BooKTS
#include "msp430g2253.h"

// Khai bao chan
#define Trig BIT0
#define Echo BIT2
#define LED  BIT6

// Khai bao bien toan cuc
int miliseconds, hcsr04;

// Khai bao ham
void delayms(int ms);
void initTimer();
void initIO();
int readDistance();

void main() {
  WDTCTL = WDTPW + WDTHOLD;     // Khoi dong chip
  initIO();                     // Cau hinh chan
  initTimer();                  // Cau hinh timer
  
  //P2IFG = 0x00;                 // Xoa tat ca cac co ngat Port1
  P1IFG = 0x00;
  
  _BIS_SR(GIE);                 // Cho phep ngat toan cuc
  //__bis_SR_register(GIE);
 // P1IFG = 0x00;                 // Xoa tat ca cac co ngat Port1
   
  // Vong lap chuong trinh chinh
  while(1) {
    int distance = readDistance();
    //delayms(200);
    if (distance > 5)       P1OUT |= LED;
    else        P1OUT &= ~LED;
  }
}
// Ham delay ms
void delayms(int ms) {
  for(int i=0; i<ms; i++)         __delay_cycles(1000); // delay 1ms
}
// Cau hinh timer
void initTimer() {
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  // Chon che do SMCLK va Upmode 
  TACTL = TASSEL_2 + ID_0 + MC_1;
  // Cho phep CCR0 ngat
  CCTL0 = CCIE;
  CCR0 = 1000;
  
}
// Cau hinh chan
void initIO() {
   P1DIR |= Trig;                        // 0100 0001
   P1DIR &= ~Echo;
   //  P2REN = BIT1;
 //  P2OUT = BIT1;
   //P2DIR &= ~Echo;
   P1DIR |= LED;   // 0100 0000
}
// Ham doc khoang cach
int readDistance() {
  P1IE &= ~Trig;                // Ngat P1.0
  
  P1OUT |= Trig;                // Trig muc 1
  __delay_cycles(10);           // delay 10 (us)
  P1OUT &= ~Trig;               // Trig muc 0
  
  P1IFG = 0x00;                 // Xoa tat ca co ngat Port1
  P1IE |= Echo;                 // Cho phep ngat echo 
  P1IES &= ~Echo;               // Ngat canh len
  delayms(30);                  // Delay 30 (ms)
    
  int distance = hcsr04/58;     // Tinh toan khoang cach
  return distance;
}

// Chuong trinh con ngat
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
  if(P1IFG&Echo) {              // Cho xu ly
    if(!(P1IES&Echo)) {         // Co ngat hay khong
      TACTL |= TACLR;
      miliseconds = 0;
      P1IES |= Echo;
    }
    else hcsr04 = (long)miliseconds*1000 + (long)TAR;           // Do do rong xung
    P1IFG &= ~Echo;             // Xoa co ngat Echo
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  miliseconds++;
}