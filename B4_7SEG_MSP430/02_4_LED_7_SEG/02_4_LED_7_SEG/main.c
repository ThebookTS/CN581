// BookTS
// Doc phim an K1 tang K2 giam

#include "io430.h"

// khai bao IO
#define DLED    P1OUT
#define L1      P2OUT_bit.P0
#define L2      P2OUT_bit.P1
#define L3      P2OUT_bit.P2
#define L4      P2OUT_bit.P3

#define K1      P2IN_bit.P4
#define K2      P2IN_bit.P5

//prototype
void delayms(int ms);
void scanLED(int tx);
void print2LED(int num);

//Khai bao bien--------------
//                         0     1    2   3    4    5    6    7    8    9    A     B    C   D    E    F
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E}; // bang ma LED 7SEG ANODE chung
unsigned char buff[4] = {0,0,0,0};

int count = 0;
//-----------------------------------
void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // cai hinh IO
  P1DIR = 0xFF;
  P2DIR = 0x0F;
  P2REN = BIT4 + BIT5;
  P2OUT = BIT4 + BIT5;
    
  count = 0;
  print2LED(count);
  
  // main loop
  while(1){
    if(K1==0){
       delayms(150);
       count++;
       print2LED(count); 
    }
    if(K2==0){
       delayms(150);
       count--;
       print2LED(count); 
    }

    scanLED(5);  // phai duoc goi lien tuc
  }
}
// Ham scanLED
void scanLED(int tx){
    P2OUT = 0x00;               // tat het cac LED 
    DLED  = tbl7segA[buff[0]];  // hien thij so hang nghin
    L1    = 1;                  // cho L1 sang
    delayms(tx);

    P2OUT = 0x00;               // tat het cac LED 
    DLED  = tbl7segA[buff[1]];  // hien thi sp hang tram
    L2    = 1;                  // cho L1 sang
    delayms(tx);

    P2OUT = 0x00;               // tat het cac LED 
    DLED  = tbl7segA[buff[2]];  // hien thi so hang chuc
    L3    = 1;                  // cho L1 sang
    delayms(tx);

    P2OUT = 0x00;               // tat het cac LED 
    DLED  = tbl7segA[buff[3]];  // hien thi so hang don vi
    L4    = 1;                  // cho L1 sang
    delayms(tx);         
}
// Chuyen doi so LED 7 doan
void print2LED(int num){
  buff[0] =  num/1000;
  buff[1] = (num%1000)/100;
  buff[2] = ((num%1000)%100)/10;
  buff[3] = ((num%1000)%100)%10;  
}
// Ham delaymiliseconds
void delayms(int ms){
   for(int i=0;i<ms;i++)
     __delay_cycles(100);       // 1ms --thuc te 1000; mo phong --100
}