//Pham Phú Quí làm code này!
#include "io430fr5969.h"

//   Khai bao port vao ra
#define dir     BIT0
#define step    BIT6

// Do rong xung
int duty;

//Khai bao prototype ham
void delayms(int ms);

void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Cau hinh port IO
  P1DIR = 0x04 + 0x10;
  
  P1OUT |= 0x10;
  //BOOMBAYAH! OH OH OH OHHHH~
  // Main Loop
  while(1){
    P1OUT |= 0x04;
     __delay_cycles(500);
    P1OUT &= ~0x04;
     __delay_cycles(500);
  }
}

void delayms(int ms){
  for(int i = 0; i < ms; i++)
    __delay_cycles(100); //delay 1ms
}