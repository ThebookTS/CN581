// BooKTS
#include "msp430g2253.h"

// Khai bao chan
#define Trig BIT0
#define Echo BIT2

#define D7SEG   P2OUT
#define Motor   BIT1
#define C1      BIT4
#define C2      BIT5
#define C3      BIT6
#define C4      BIT7

// Khai bao bien toan cuc
int  idx = 0;
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; // bang ma LED 7SEG ANODE chung
char buff[4]={1,2,3,4};
int miliseconds, hcsr04;
int Height;

// Khai bao ham
void delayms(int ms);
void initTimer();
void initIO();
int readDistance();
void scanled();
void countbuff();

void main() {
  WDTCTL = WDTPW + WDTHOLD;     // Khoi dong chip
  
  initIO();                     // Cau hinh chan
  initTimer();                  // Cau hinh timer
  
  P1IFG = 0x00;
  
  _BIS_SR(GIE);                 // Cho phep ngat toan cuc
   
  // Vong lap chuong trinh chinh
  while(1) {
    P1OUT |= Motor;
    delayms(5000);
    P1OUT &= ~Motor;
    int distance = readDistance();
    delayms(50);
    Height = 8 - distance;
    countbuff(Height);
    scanled();
    delayms(50);
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
  TACTL = TASSEL_2 + ID_0 + MC_1;                       // TASSEL 2: Chon xung clock SMCLK 1MHz
  // Cho phep CCR0 ngat
  CCTL0 = CCIE;
  CCR0 = 1000;                // chu ky ngat = f_ck / f_CCR0
}
// Cau hinh chan
void initIO() {
   P1DIR |=  Trig;
   P1DIR &= ~Echo;
   P1DIR |=  C1;
   P1DIR |=  C2;
   P1DIR |=  C3;
   P1DIR |=  C4;
   P1DIR |= Motor;
   P2DIR |=  0xff;
   
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
  scanled();
}

// Vector ngat
//#pragma vector=TIMER0_A1_VECTOR
//__interrupt void Timer0_A1 (void) {
  // quet led 5ms
//  scanled();
//}

void scanled(){
  D7SEG = tbl7segA[buff[idx]];
  // Dieu khien LED idx sang
  switch(idx){          
    case 0:             // LED 1
        P1OUT |=  C1;
        P1OUT &= ~C2; 
        P1OUT &= ~C3;
        P1OUT &= ~C4;
        break;
    case 1:             // LED 2
        P1OUT &= ~C1;         
        P1OUT |=  C2;
        P1OUT &= ~C3;
        P1OUT &= ~C4;
        break;
    case 2:             // LED 3
        P1OUT &= ~C1;
        P1OUT &= ~C2; 
        P1OUT |=  C3; 
        P1OUT &= ~C4;
        break;
    case 3:             // LED 4
        P1OUT &= ~C1;
        P1OUT &= ~C2;
        P1OUT &= ~C3;
        P1OUT |=  C4;
        break;
  }
  __delay_cycles(100);
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