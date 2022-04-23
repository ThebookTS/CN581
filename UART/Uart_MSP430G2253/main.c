// BookTS

#include "io430g2253.h"

#define SMCLK_F    1000000
#define	BAUDRATE   9600 
#define LED1   BIT6
#define LED2   BIT7

// Khai bao prototype ham
void delayms(int ms);
void UART_Init();    
void ChrOut(char data);
void StrOut(char *str);
char ChrIn();


void main( void ) {
  
  WDTCTL = WDTPW + WDTHOLD;
  
  // Cau hinh DCO – 1MHZ
  BCSCTL1  = CALBC1_1MHZ;
  DCOCTL   = CALDCO_1MHZ;    
  BCSCTL3 |= LFXT1S_2 + XCAP_3;
  BCSCTL2 |= SELM_0 + DIVM_0;
  
  // Cau hinh IO
  P2SEL = 0;
  P2DIR = BIT6 + BIT7;
  P2OUT|= BIT6 + BIT7;  
 
  UART_Init();                  // Khoi dong Uart
  StrOut("Hello world !");      // Gui chuoi "Hello world"
  
  // vong lap chuong trinh chinh
  while(1) {
    char ch=ChrIn();
    if(ch!=0) {
      ChrOut(ch);                               // Gui lai ky tu vua nhan
      if(ch=='a')  P2OUT &= ~LED1;
      if(ch=='b')  P2OUT |= LED1;
      if(ch=='c')  P2OUT &= ~LED2;
      if(ch=='d')  P2OUT |= LED2;
    }   
  } 
}

// Cau hinh Uart
void UART_Init(){
  unsigned int tempfactor;
  // P1.1 = RXD, P1.2=TXD
  P1SEL   |= BIT1 + BIT2;
  P1SEL2  |= BIT1 + BIT2;  

  UCA0CTL1|= UCSWRST; 
  UCA0CTL0 = 0 ; //8bit,none Parity,1 stop
  UCA0CTL1|= UCSSEL_2; // SMCLK
    
  tempfactor = SMCLK_F/BAUDRATE;
  UCA0BR0 = (unsigned char) tempfactor&0x00FF;
  tempfactor >>= 8;
  UCA0BR1 = (unsigned char) (tempfactor&0x00FF);
    
  UCA0MCTL |= UCBRF_0 + UCBRS_0;

  UCA0CTL1 &=~ UCSWRST; 
}

// Ham gui ky tu
void ChrOut(char data) {
  while (!(IFG2&UCA0TXIFG));
   UCA0TXBUF= data;  
}

// Ham gui chuoi
void StrOut(char* str) {
  while(*str) {
     ChrOut(*str);
      str++;          // Lay ky tu tiep theo
  }
  ChrOut(0);
}
//------------------------------------------
char ChrIn() {
  if((IFG2&UCA0RXIFG)!=0)
     return UCA0RXBUF;
  else 
     return 0;
}
