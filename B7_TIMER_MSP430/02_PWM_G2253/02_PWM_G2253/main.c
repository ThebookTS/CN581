// PWM MSP430G2253
#include "io430.h"

void delayms(int ms);

int tx1 = 0, tx2 =0;

void main(){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  //Cau hinh IO  
  P1DIR  = BIT2 + BIT3 + BIT7;    // P1.2 OUTPUT
  P1SEL  = BIT2 + BIT3 + BIT7;    // P1.2 -- PWM out
  
  P3DIR  = BIT7;
  P3SEL  = BIT7;
  
  // Cau hinh timer
  TA0CTL  = TASSEL_2 + ID_3 + MC_1; 
  TA0CCR0 = 624;     // thiet lap chu ky PWM = 5ms, fPWM = 200Hz
  
  // thiet lap che do PWM
  TA0CCTL1 = CM_0 | CCIS_0 | OUTMOD_7;
  TA0CCTL2 = CM_0 | CCIS_0 | OUTMOD_7;
  
  // thiet lap gia tri PWM ban dau
  TA0CCR1  = tx1;   // PWM = 50%, ----> P1.2
  TA0CCR2  = tx2;   // PWM = 25%, ----> P3.1
  
  // Cau hinhf PWM cho Timer A1
  TA1CTL  = TASSEL_2 + ID_3 + MC_1; 
  TA1CCR0 = 624;     // thiet lap chu ky PWM = 5ms, fPWM = 200Hz
  
  // thiet lap che do PWM
  TA1CCTL1 = CM_0 | CCIS_0 | OUTMOD_7;
  TA1CCR1  = 624/8;
  
  // Vong lap chuong trinh chinh
  while(1){
    tx1 = tx1 + 10; TA0CCR1 = tx1;
    tx2 = tx2 + 50; TA0CCR2 = tx2;
    
    if(tx1>=624) tx1 = 0;
    if(tx2>=624) tx2 = 0;
    
    delayms(500);
  }  
}
//--------------
void delayms(int ms) {
for(int i=0; i<ms ; i++)
__delay_cycles(1000);
}