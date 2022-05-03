#include "io430.h"

#define DLED P3OUT
#define CTR P6OUT

#define K0 P2IN_bit.P2IN3
#define K1 P2IN_bit.P2IN5
#define K2 P2IN_bit.P2IN4
#define K3 P4IN_bit.P4IN3
#define COM P4OUT_bit.P4OUT0

#define VR1 6
#define VR2 12
#define PHOTO 5
#define TEMP 10

// Khai bao prototype ham
void delayms(int ms);
void scanled();
void toDisplay(int dat);
unsigned int ReadADC12(int channel);

char tbl7seg[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
char buff[4] = {0,0,0,0};
int idx = 0;

unsigned int val;
unsigned char status;

void main( void ) {
  
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR = 0xFF;         // P3 la OUTPUT
  P6DIR = 0xFF;         // P6 la OUTPUT

  P4DIR = BIT0;

  P4REN = BIT3; 
  P4OUT = BIT3;

  P2REN = BIT3 + BIT4 + BIT5;
  P2OUT = BIT3 + BIT4 + BIT5;
  COM = 0;              // KY0 = 0 (GND)
  status = 0;
  val = 0;

  TA0CTL = TASSEL_2 + ID_3 + MC_1; // Mode up
  TA0CCTL0 = CCIE;
  TA0CCR0 = 625;
  __bis_SR_register(GIE);

  while(1) {
    // getkey
    if (K0==0) status = 1;
    if (K1==0) status = 2;
    if (K2==0) status = 3;
    if (K3==0) status = 4;
    switch(status) {
      case 1: val = ReadADC12(VR1);       // Doc gia tri bien tro VR1
              break;
      case 2: val = ReadADC12(VR2);       // Doc gia tri bien tro VR2
              break; 
      case 3: val = ReadADC12(TEMP);      // Doc gia tri cam bien nhiet noi
              break; 
      case 4: val = ReadADC12(PHOTO);     // Doc gia tro cam bien quan
              break;
    }
    toDisplay(val);
    delayms(5);
  }
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

  while (ADC12CTL1 & ADC12BUSY)
    __no_operation();
  return (ADC12MEM0 & 0x0FFF);
}
// Ham delay miliseconds
void delayms(int ms) {
  for(int i=0; i<ms ; i++)              __delay_cycles(1000);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void) {
  scanled();
}
// Ham scan led
void scanled() {
  char ch;
  ch = buff[idx];
  DLED = tbl7seg[ch];
  CTR = ~(1<<idx);
  idx++;
  if (idx>=4) idx = 0;
}
// Ham chuyen doi gia tri LED 7 doan
void toDisplay(int dat){
  buff[3] = val /1000;
  buff[2] = (val %1000)/100;
  buff[1] = ((val %1000)%100)/10;
  buff[0] = val %10;
}