// BooKTS
#include "msp430g2253.h"

// Khai bao chan HCSR04
#define Trig BIT0
#define Echo BIT1
#define LED BIT6

// Khai bao chan LED
#define D7SEG   P1OUT
#define C6      BIT2
#define C8      BIT3
#define C9      BIT4
#define C12     BIT5

// Khai bao bien toan cuc
int miliseconds, hcsr04;
// Khai bao bien LED 7 doan
int  idx = 0;
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; // bang ma LED 7SEG ANODE chung
char buff[4]={1,2,3,4};

// Khai bao ham
void delayms(int ms);
void initTimer();
void initIO();
int readDistance();
void scanled();
void countbuff();

void main() {
  WDTCTL = WDTPW + WDTHOLD;     // Khoi dong chip
  
  initTimer();                  // Cau hinh timer
  initIO();                     // Cau hinh chan
  
  P2IFG = 0x00;                 // Xoa tat ca cac co ngat Port1
  _BIS_SR(GIE);                 // Cho phep ngat toan cuc 
  // Vong lap chuong trinh chinh
  while(1) {
  //  int distance = readDistance();
    //delayms(100);
    //countbuff(2000);
    //delayms(100);
  
  int distance = readDistance();
    //delayms(200);
    if ((distance <= 20)&&(distance > 2))       P1OUT |= LED;
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
  CCTL1 = CCIE;
  CCR0 = 1000;
  CCR1 = 625;
}
// Cau hinh chan
void initIO() {
   P2DIR |= 0x3d;               // 0011 1101
   P1DIR = 0x7f;
}
// Ham doc khoang cach
int readDistance() {
  P2IE &= ~Trig;                // Ngat P1.0
  
  P2OUT |= Trig;                // Trig muc 1
  __delay_cycles(10);           // delay 10 (us)
  P2OUT &= ~Trig;               // Trig muc 0
  
  P2IFG = 0x00;                 // Xoa tat ca co ngat Port1
  P2IE |= Echo;                 // Cho phep ngat echo 
  P2IES &= ~Echo;               // Ngat canh len
  delayms(30);                  // Delay 30 (ms)
    
  int distance = hcsr04/58;     // Tinh toan khoang cach
  return distance;
}

// Chuong trinh con ngat
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void) {
  if(P2IFG&Echo) {              // Cho xu ly
    if(!(P2IES&Echo)) {         // Co ngat hay khong
      TACTL |= TACLR;
      miliseconds = 0;
      P2IES |= Echo;
    }
    else hcsr04 = (long)miliseconds*1000 + (long)TAR;           // Do do rong xung
    P2IFG &= ~Echo;             // Xoa co ngat Echo
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  miliseconds++;
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void) {
  //scanled();
}
void scanled(){
  D7SEG = tbl7segA[buff[idx]];
  // Dieu khien LED idx sang
  switch(idx){ 
    case 0: 
      P2OUT = 0x04;
      break;	        // LED 1
    case 1: 
      P2OUT = 0x08;    
      break; 	        // LED 2
    case 2: 
       P2OUT = 0x10;     
      break; 	        // LED 3
    case 3: 
      P2OUT = 0x20; 
      break;            // LED 4
  }
  idx++;
  if (idx>=4) idx = 0;
}

// Tach so
void countbuff(int counter){
  buff[0] = counter/1000;
  buff[1] = (counter%1000)/100;
  buff[2] = ((counter%1000)%100)/10;
  buff[3] = counter%10;
}