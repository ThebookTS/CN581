#include "io430g2553.h"
// khai bao IO
#define PLED    P1OUT
#define K1      P2IN_bit.P0

// ptototype
void delayms(int ms);

//Khai bao bien
//                         0     1    2   3    4    5    6    7    8    9    A     B    C   D    E    F
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};     // bang ma LED 7SEG ANODE chung

int index;

void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // cau hinh IO
  P1DIR = 0xFF;         // P1 as OUTPUT
  P1OUT = 0xFF;
  
  P2DIR = 0x00;
  P2REN = BIT0;
  P2OUT = BIT0;
  
  index = 0;
  // main loop
  while(1){
    if(K1==0){
       delayms(200);
       index++;
       if(index>15) index = 0;
       PLED = tbl7segA[index];          // doi sang ma 7seg cua bien index
    }
    
  }
}
//------------------------------------
void delayms(int ms){
   for(int i=0;i<ms;i++)
     __delay_cycles(100); // 1ms --thuc te 1000; mo phong --100
}