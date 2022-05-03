// BookTS
// Hien thi 4 led 7 doan bang phuong phap quet (khong su dung ngat timer)

#include "io430f5529.h"

// for scan led 7seg
#define DLED P3OUT
#define CTR P6OUT
#define K0 P2IN_bit.P2IN3
#define K1 P2IN_bit.P2IN5
#define K2 P2IN_bit.P2IN4
#define K3 P4IN_bit.P4IN3
#define COM P4OUT_bit.P4OUT0
//- Khai bao prototype ham
void delayms(int ms);
void scanled();
void count2buff();

// Khai bao bien ham scanled
char tbl7seg[] ={0xC0,0xF9,0xA4,0xB0,0x99,
0x92,0x82,0xF8,0x80,0x90};
char buff[4] = {0,0,0,0};                       // Vung dem led
int idx = 0;                                    // Con tro quet led
// Khai bao bien chuong trinh chinh
int timer = 0, counter = 0;
int status, interval;

void main( void ){
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR = 0xff;                         // P3 la OUTPUT
  P6DIR = 0xff;                         // P6 la OUTPUT
  P4DIR = BIT0;
  P4REN = BIT3; P4OUT = BIT3;
  P2REN = BIT3 + BIT4 + BIT5;
  P2OUT = BIT3 + BIT4 + BIT5;
  COM = 0;                              // KY0 = 0 (GND)
  timer = 0; counter = 0;
  status = 0;
  
  // Vong lap chuong trinh chinh
  while(1){
    // Xu ly phim an
    if(K0==0) {
      delayms(100); counter++;
    }
    if(K1==0) {
      delayms(100); counter--;
    }
    if(K2==0) {
      delayms(100); counter = 0;
    }
    
    /*  sua K lai
    if( K2 == 0 ) Status = 1 ;
    if( K3 == 0 ) Status = 2 ;
    
    // Xu ly counter
    if (Interval++>20) {
      Interval = 0;
      if (Status ==1) counter++;
      if (Status ==2) counter--;
    }  
    */
    count2buff();
    // Ham scan led voi chu ky la 5ms
    scanled();
    delayms(5);
  }
}

//- Ham quet led
void scanled() {
  char ch;
  ch = buff[idx];
  DLED = tbl7seg[ch];
  CTR = ~(1<<idx);
  idx++;
  if (idx>=4) idx = 0;
}
// Ham cap nhat hien thi counter => buff
void count2buff() {
  buff[3] = counter/1000;
  buff[2] = (counter%1000)/100;
  buff[1] = ((counter%1000)%100)/10;
  buff[0] = counter%10;
}
// Ham delayms
void delayms(int ms) {
  for(int i=0; i<ms ; i++)
  __delay_cycles(1000);
}