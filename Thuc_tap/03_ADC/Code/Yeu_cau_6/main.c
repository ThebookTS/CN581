// BookTS
#include "io430.h"

// define for PWM
#define F_CPU 1000000UL
#define PWM_PERIOD F_CPU/490
#define PWM_DUTY(x)((unsigned long)x*PWM_PERIOD /(unsigned long)(0xff))

// Khai bao port I/O
#define K0 P2IN_bit.P2IN3
#define K1 P2IN_bit.P2IN5
#define K2 P2IN_bit.P2IN4
#define K3 P4IN_bit.P4IN3
#define COM P4OUT_bit.P4OUT0
#define DLED P3OUT
#define CTR P6OUT

#define PHOTO 5
#define LED_R 4         // p1.4 : LED RED
#define LED_G 3         // p1.3 : LED GREEN
#define LED_B 2         // p1.2 : LED BLUE

// Khai bao prototype ham
void delayms(int ms);
unsigned int ReadADC12(int channel);
void SetPWM(unsigned char pin, int val);

// Khai bao bien toan cuc
unsigned int val;
unsigned char status;

void main( void ) {
  WDTCTL = WDTPW + WDTHOLD;
  
  P4DIR = BIT0;
  P4REN = BIT3; 
  P4OUT = BIT3;
  
  P2REN = BIT3 + BIT4 + BIT5;
  P2OUT = BIT3 + BIT4 + BIT5;
  COM = 0; // KY0 = 0 (GND)
  
  // Khoi dong timer for PWM
  TA0CCR0 = PWM_PERIOD;         // PWM Period
  TA0CTL = TASSEL_2 + MC_1+ ID_0;
  
  status = 0;

  while(1) {
    // getkey
    if (K1==0) status = 1;
    if (K2==0) status = 2;
    if (K3==0) status = 3;
    switch(status) {
    case 1: val = (25*ReadADC12(PHOTO))/1000;
            SetPWM(LED_R,val);
            break;
    case 2: val = 50*(500-ReadADC12(PHOTO))/1000;
            SetPWM(LED_G,val);
            break;
    case 3: val = (30*(ReadADC12(PHOTO)^2))/1000;
            SetPWM(LED_B,val);
            break;
    }
    delayms(50);
  }
}
// Ham set PWM
void SetPWM(unsigned char pin, int val) {
  P1DIR |= (1<<pin);
  P1SEL |= (1<<pin);
  switch(pin) {
    case 2: TA0CCTL1 = OUTMOD_7;
    TA0CCR1 = PWM_DUTY(val);
    break;
    case 3: TA0CCTL2 = OUTMOD_7;
    TA0CCR2 = PWM_DUTY(val);
    break;
    case 4: TA0CCTL3 = OUTMOD_7;
    TA0CCR3 = PWM_DUTY(val);
    break;
  }
}
// Ham delay miliseconds
void delayms(int ms) {
  for(int i=0; i<ms ; i++)        __delay_cycles(1000);
}
// Ham doc gia tri ADC 12 bit
unsigned int ReadADC12(int channel) {
  REFCTL0 &= ~REFMSTR;
  ADC12CTL0 = ADC12SHT0_9 + ADC12REFON +
  ADC12REF2_5V + ADC12ON;
  ADC12CTL1 = ADC12SHP;
  ADC12MCTL0 = ADC12SREF_1 + channel;
  
  if (channel<8) P6SEL |= (1<<channel);
  if (channel==12) P7SEL |= BIT0;
  
  ADC12CTL0 &= ~ADC12SC;
  ADC12CTL0 |= ADC12SC + ADC12ENC;
  
  //Waiting for conversion to complete
  while (ADC12CTL1 & ADC12BUSY)
  __no_operation();
  return (ADC12MEM0 & 0x0FFF);
}