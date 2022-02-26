/* - LED 3, LED 4 sang 
   - An button LED se nhay 5 lan*/

#include "io430g2553.h"

// khai bao port IO
#define L1      BIT0
#define L2      BIT2
#define L3      BIT4
#define L4      BIT5

#define K1      BIT0
#define K2      BIT1

// khai bao prototype ham
void delayms(int ms);
void blinkL1(int n);
void blinkL2(int n);
//------------------------------
void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  // cau hinh portIO
  P1DIR = BIT0 + BIT2 + BIT4 + BIT5; // 0b00110101 = 0x35
  P1OUT = L3 + L4; // L4 = L5 = 1; 

  P2DIR  = 0x00;
  P2REN |= BIT0 + BIT1; // bat R keo tai P2.0, P2.1
  P2OUT |= BIT0 + BIT1; // Keo len Vcc
  
  // main loop
  while(1) {
    if((P2IN&K1)==0){    
       blinkL1(5);
    }
    
    if((P2IN&K2)==0){    
       blinkL2(5);
    }    
  }
}
// Dinh nghia ham con-----------
void delayms(int ms){
  for(int i=0;i<ms;i++)
    __delay_cycles(100); // delay 1ms
}
//--------------------
void blinkL1(int n){
  for(int i=0;i<n;i++){
      P1OUT |= L1;   // L1 = 1; cac bit khac khong thay doi
      delayms(500);    
      P1OUT &= ~L1;   // L1 = 0; cac bit khac khong thay doi
      delayms(500); 
  }
}
//----------------------------
void blinkL2(int n){
  for(int i=0;i<n;i++){
      P1OUT |= L2;   // L1 = 1; cac bit khac khong thay doi
      delayms(500);    
      P1OUT &= ~L2;   // L1 = 0; cac bit khac khong thay doi
      delayms(500); 
  }
}
