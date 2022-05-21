#include "msp430g2553.h"

//------------khai bao I/O------------------------
#define D7SEG   P2OUT
#define C6      BIT4
#define C8      BIT5
#define C9      BIT6
#define C12     BIT7

//-------------chuong trinh con-------------------
void delayms(int ms);
void scanled();
void countbuff();

//-------------khai bao bien--------------------
int  idx = 0;
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; // bang ma LED 7SEG ANODE chung
char buff[4]={1,2,3,4};
int interval = 0;
int counter = 0;

//-------------chuong trinh chinh---------------
void main()
{
  //----Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  //-------------cau hinh IO-------------------------------
  P2DIR = 0xff;         // 1111 1111
  P1DIR |= BIT4 + BIT5 + BIT6 + BIT7;

  //--------Khoi dong timer------------------------------
  TA0CTL = TASSEL_2 + ID_3 + MC_1;
  TA0CCTL0 = CCIE;
  
  __bis_SR_register(GIE);

  while(1){
    if (interval++>10) {
      interval = 0;
      counter++;
    }   
    countbuff(counter);
    delayms(50);
  }
}

// Vector ngat
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void) {
  // quet led 5ms
  scanled();
}

void delayms(int ms){
  for(int i=0; i<ms; i++)
    __delay_cycles(1000);
}

void scanled(){
  D7SEG = tbl7segA[buff[idx]];
  // Dieu khien LED idx sang
  switch(idx){          
    case 0: 
        P1OUT = 0x80;                   
      break;	        // LED 1
    case 1: 
        P1OUT = 0x40;    
      break; 	        // LED 2
    case 2: 
        P1OUT = 0X20;     
      break; 	        // LED 3
    case 3: 
        P1OUT = 0x10; 
      break;            // LED 4
  }
  idx++;
  if (idx>=4) idx = 0;
}

// Tach so
void countbuff(int counter){
  buff[3] = counter/1000;
  buff[2] = (counter%1000)/100;
  buff[1] = ((counter%1000)%100)/10;
  buff[0] = counter%10;
}