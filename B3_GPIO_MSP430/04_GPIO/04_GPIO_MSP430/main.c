/* Dich LED*/

#include "msp430g2553.h"

// khai bao IO
#define DLED    P1OUT

#define K1      BIT0
#define K2      BIT1

// prototype ham
void delayms(int ms);
void shiftLeft();
void brightShiftLeft();

void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // cau hinh IO
  P1DIR  = 0xff;                // P1 = OUTPUT
  
  P2DIR = 0x00;
  P2REN = BIT0 + BIT1;          // Bat R keo
  P2OUT = BIT0 + BIT1;          // Keo len
  
  // Thiet lap ngat
  P2IE  = BIT0 + BIT1;          // cho phep ngat tai P2.0, P2.1
  P2IES = BIT0 + BIT1;          // cho phep ngat canh xuong
  P2IFG = 0x00;                 // xoa co ngat         
  _BIS_SR(GIE);                 // cho phep ngat toan cuc

  // Main loop
  while(1){     
    delayms(1000);  
  }
}

// Khai bao vector ngat
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
  if(P2IFG&BIT0) {              // K1 duoc an
    shiftLeft();  
  }
  if(P2IFG&BIT1){                // K2 duoc an
    brightShiftLeft();
  }
  P2IFG = 0x00;
}
// Ham delay miliseconds
void delayms(int ms){
  for(int i=0;i<ms;i++)
    __delay_cycles(100); // delay 1ms
}
void shiftLeft(){
    DLED = 0x01;
    delayms(500);
    unsigned char i;
    for (i = 0; i < 8;i++){
      DLED <<= 1;
      delayms(500);
    }  
}
void brightShiftLeft(){
    DLED = 0x01;
    delayms(500);
    unsigned char i;
    for (i = 0; i < 8;i++){
      DLED = (DLED<<1) + 0x01;
      delayms(500);
    }  
}
