#include <io430g2553.h>

#define L5 P1OUT_bit.P5
#define L6 P1OUT_bit.P6
#define DLED  P2OUT
#define CTR   P1OUT

// khai bao chuong trinh con
void delayms(int ms);
void bjt();
void led(int num);
unsigned int ReadADC10(int chanel);
void delayms(int ms);

// khai bao bien toan cuc
char dichled[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
char dichh[]={0x3f, 0x36, 0x79,  0x76};
char buff[4] = {0,0,0,0};
int idx = 0, idxx=0;
float   counter,volts,amps, microamps ;

// chuong trinh chinh
void main() {
  // khoi dong chip
  WDTCTL =  WDTPW + WDTHOLD; // TAT WATHCHDOG
  P2DIR = 0xff;              // KHAI BAO P2.0 -> P2.7
  P2SEL = 0x00;
  P1DIR = 0X7f;              // KHAI BAO P1.0 -> P1.6
  //cau hinh timer
  TACTL = TASSEL_2 + ID_3 + MC_1;       // CHON SMCLK CO F = 1MHZ, BO CHIA BANG 8, MODE DEM LEN
  CCTL0 = CCIE;                       // cho phep tin hieu di qua
  CCTL1 = CCIE; 
  CCR0 = 600;                            // t = 4.8ms; f=1/t=208HZ; CCR0=fck/f=600;
  CCR1 = 600;                            // t = 4.8ms; f=1/t=208HZ; CCR0=fck/f=600; 
  __bis_SR_register(GIE);               //lenh ngat toan cuc su dung trong thu vien <io430>

  // khoi dong bien
  while(1) {
      // nhan va xu ly INPUT  
      volts =( ReadADC10(7)* 3.3 ) /1023.0;
      amps = volts/10000.0; 
      microamps = amps*1000000.0;
      counter= microamps*2.0;
      led(counter);
      delayms(300);
  }
}

// ADC - chuyen tin hieu tuong tu tu cam bien sang tin hieu so cho vi dieu khien
unsigned int ReadADC10(int chanel) {
    ADC10CTL0 = SREF_2 + ADC10SHT_2 + ADC10ON + REF2_5V + REFON; // V_ref+ = 3.3v
    ADC10AE0 |= (BIT0<<chanel);
    ADC10CTL1 |= (chanel*0x1000u);
    ADC10CTL0 |= ENC +  ADC10SC; // b1: start ADC
    while(!(ADC10CTL0 & ADC10IFG)); // ADC bien doi
    ADC10CTL0 = 0; // tat ADC
    return ADC10MEM; // doc gia tri bien doi
}

// ham timer
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void) {
    bjt();
    if( counter >= 600)  L5 = 1;
    else  L5 = 0; 
    if( counter <10)  L6 = 1;
    else  L6 = 0;
}

//ham timer
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void) {
  for(int i=0; i<20000; i++) {
      for(int ii=0; ii<3; ii++) {
          CTR = 0x00;
          DLED = dichh[idxx];
          CTR = (1<<idxx);
          idxx++;
          if(idxx>=4) idxx= 0;
      }
 }
   CCTL1 &=~ CCIE;
}

// ham quet led - lay du lieu led tu buff de dua ra man hinh led
void bjt() {
      CTR = 0x00;
      DLED = dichled[buff[idx]];    
      CTR = (1<<idx);
      idx++;
      if(idx>=4) idx = 0;
}
// ham chuyen doi gia tri de truyen vao buff
void led(int counter) {
    buff[3] = counter/1000;
    buff[2] = (counter%1000)/100;
    buff[1] = ((counter%1000)%100)/10;
    buff[0] = counter%10;
}

// ham delay 
void delayms(int ms) {
    for(int i=0; i<ms; i++)
    __delay_cycles(1000);
}



