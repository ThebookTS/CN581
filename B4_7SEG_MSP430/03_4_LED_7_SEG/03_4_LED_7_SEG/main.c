// BooKTS
// Doc phim ma tran hien thi ra 4 LED 7 doan

#include "io430.h"

// khai bao IO
#define DLED    P1OUT
#define LA      P2OUT_bit.P0
#define LB      P2OUT_bit.P1
#define EN      P2OUT_bit.P2

// khai bao Matrix key
#define ROW0    P2IN_bit.P5
#define ROW1    P2IN_bit.P6
#define ROW2    P2IN_bit.P7

#define COL0    P2OUT_bit.P4
#define COL1    P2OUT_bit.P3

//-prototype
void delayms(int ms);
void scanLED(int tx);
void print2LED(int num);
unsigned char scanKey();

//Khai bao bien--------------
//                         0     1    2   3    4    5    6    7    8    9    A     B    C   D    E    F
unsigned char tbl7segA[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E}; // bang ma LED 7SEG ANODE chung
unsigned char buff[4] = {0,0,0,0};

int count = 0;
unsigned char kcode;

void main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  // cai hinh IO
  P1DIR = 0xFF;
  
  P2SEL = 0x00;                 // thiet lap all P2 pin as IO
  P2DIR = 0x1F;                 // P2.0 --- P2.4 : OUTPUT; P2.5 ->P2.7: INPUT
  P2REN = BIT5 + BIT6 + BIT7;   // bat R keo
  P2OUT = BIT5 + BIT6 + BIT7;   // Keo len Vcc
    
  count = 0;
  print2LED(count);  
  // main loop
  while(1){    
    // doc phim matran   
    kcode = scanKey();  // phai duoc goi lien tuc voi chu ky T <=100ms
    
    //--------    hien thi mas phim ra LED 7 seg----
    if(kcode!=0){
       print2LED(kcode);
    }
    //-----Quet hien thij LED
    scanLED(10);        // phai duoc goi lien tuc T = 10ms
  }
}
// Quet phim+-
unsigned char scanKey(){
  unsigned char key;  
  // doc phim ma tran
  key = 0; // 
  COL0 = 0; COL1 = 1;
  if(ROW0==0)  key = 1; // gan ma cho phim       
  if(ROW1==0)  key = 2; // gan ma cho phim       
  if(ROW2==0)  key = 3; // gan ma cho phim           
    
  COL0 = 1; COL1 = 0;
  if(ROW0==0)  key = 4; // gan ma cho phim       
  if(ROW1==0)  key = 5; // gan ma cho phim       
  if(ROW2==0)  key = 6; // gan ma cho phim       
    
  return key;
}
// Hien thi ra LED
void scanLED(int tx){
    EN    = 0;                          // Disable 74HC238      
    DLED  = tbl7segA[buff[0]];          // Hien thi so hang ngan
    LA    = 0;  LB = 0;                 // Cho L1 sang
    EN    = 1;                          // Enable 74HC238      
    delayms(tx);

    EN    = 0;                          // Disable 74HC238      
    DLED  = tbl7segA[buff[1]];          // Hien thi so hang tram
    LA    = 1;  LB = 0;                 // Cho L1 sang
    EN    = 1;                          // Enable 74HC238      
    delayms(tx);
    
    EN    = 0;                          // Disable 74HC238      
    DLED  = tbl7segA[buff[2]];          // Hien thi so hang chuc
    LA    = 0;  LB = 1;                 // Cho L1 sang
    EN    = 1;                          // Enable 74HC238      
    delayms(tx);

    EN    = 0;                          // Disable 74HC238      
    DLED  = tbl7segA[buff[3]];          // Hien thi so hang don vi
    LA    = 1;  LB = 1;                 // cho L1 sang
    EN    = 1;                          // enable 74HC238      
    delayms(tx);    
}
// Chuyen doi so sang LED-
void print2LED(int num){
  buff[0] =  num/1000;
  buff[1] = (num%1000)/100;
  buff[2] = ((num%1000)%100)/10;
  buff[3] = ((num%1000)%100)%10;  
}
// Ham delay miliseconds
void delayms(int ms){
   for(int i=0;i<ms;i++)
     __delay_cycles(100); // 1ms --thuc te 1000; mo phong --100
}