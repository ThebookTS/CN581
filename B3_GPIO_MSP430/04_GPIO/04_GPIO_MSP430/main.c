/* Dich LED*/
#include "msp430g2553.h"
// khai bao IO
#define DLED    P1OUT

#define K1      BIT0
#define K2      BIT1
// prototype ham
void delayms(int ms);
void shiftRight();
void shiftLeft();
void shiftleft1();
void xenKe(int n);
void brightShiftLeft();
//----------------------------------------------
void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  // cau hinh IO
  P1DIR  = 0xff; // P1 = OUTPUT
  
  P2DIR = 0x00;
  P2REN = BIT0 + BIT1; // Bat R keo
  P2OUT = BIT0 + BIT1; // Keo len
  // thiet lap ngat
  P2IE  = BIT0 + BIT1; // cho phep ngat tai P2.0, P2.1
  P2IES = BIT0 + BIT1; // cho phep ngat canh xuong
  P2IFG = 0x00;        // xoa co ngat         
  _BIS_SR(GIE);        // cho phep ngat toan cuc

  // Main loop
  while(1){    
    
    delayms(1000);
    
  }
}
// Khai bao vector ngat----------------
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
 
  if(P2IFG&BIT0){
    // K1 duowc an
    shiftLeft();  
  }
  if(P2IFG&BIT1){
    // K2 duowc an
    brightShiftLeft();
  }

  P2IFG = 0x00;
}
//------------Dinh nghia chuong trinh con---------------
void delayms(int ms){
  for(int i=0;i<ms;i++)
    __delay_cycles(100); // delay 1ms
}
//-------------------------------
void shiftRight(){
unsigned char tbl[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  
  for(int i=0;i<8;i++){
    DLED = tbl[i];
    delayms(500);
  }  
}
//----
void shiftleft1(){
unsigned char tbl[8] = {0x80,0x30,0x20,0x10,0x08,0x04,0x02,0x01};
  for(int i=0; i<8; i++){
    DLED = tbl[i];
    delayms(500);
  }
}
//-------------------------------
void shiftLeft(){
    DLED = 0x01;
    delayms(500);
    unsigned char i;
    for (i = 0; i < 8;i++){
      DLED <<= 1;
      delayms(500);
    }  
}
//-------------------------------
void brightShiftLeft(){
    DLED = 0x01;
    delayms(500);
    unsigned char i;
    for (i = 0; i < 8;i++){
      DLED = (DLED<<1) + 0x01;
      delayms(500);
    }  
}
//-------------------------------
void xenKe(int n) {
  for(int i = 0; i < n; i++) {
    DLED = 0x55;
    delayms(400);
    DLED = 0xAA;
    delayms(400);
  }
}

