#include "msp430g2553.h"
#define SMCLK_F    1000000
#define	BAUDRATE   9600 
// Khai bao cac chuong trinh con su dung trong chuong trinh
void Setup_Clock();     // Ham cai dat xung Clock
void Setup_Uart();       // Ham cai dat UART
void Setup_Timer();    // Ham cai dat Timer
void Setup_ADC();     // Ham cai dat ADC
int GetAdcValue();     // Ham lay gia tri ADC
void delayms(int ms);

unsigned int k = 255;                     
void main( void )                        
{ 
  WDTCTL = WDTPW + WDTHOLD;        
  Setup_Clock();                     
  Setup_Uart();                    
  Setup_Timer();                    
  Setup_ADC();                       
  
  P1DIR |= BIT6;                         
  P1SEL |= BIT6;       
  _BIS_SR(GIE);                          
 while (1)                                
   {
     __no_operation();                    
   }                                     
}                                         
//------------Ham khoi tao xung Clock ------------------
void Setup_Clock(void)                    
{
if (CALBC1_1MHZ == 0xFF)             // kiem tra tan so
  {
    while (1);                          
  }
  DCOCTL = 0;                           // Xoa cai dat truoc do
  BCSCTL1 = CALBC1_1MHZ;                //thiet lap tan so 1 MHz
  DCOCTL = CALDCO_1MHZ;                 
  BCSCTL3 |= LFXT1S_2 + XCAP_3;
  BCSCTL2 |= SELM_0 + DIVM_0;  //Chon nguon xug Clock CPU MCLK la DCO
} 
//-------------------Ham khoi tao Timer A -----------------
void Setup_Timer(void)                   
{
  TA0CCTL1 = OUTMOD_7; 
  TA0CCR0 = k; 
  TA0CTL  = TASSEL_2 + ID_3 + MC_1;   // Chon nguon xung Clock cho Timer la SMCLK | up mode
  TA0CCR1 = 0;                    // Khoi tao gia tri ban dau cho thanh ghi TA0CCR1 = 0
  }                                      
//-------------------Ham khoi tao truyen thong UART-----------------

void Setup_Uart(void)                   
{
  unsigned int tempfactor;
  P1SEL |= BIT1 + BIT2;                   //Lua chon chuc nang cho module ngoai vi 1 tai chan P1.1 va P1.2 
  P1SEL2  |= BIT1 + BIT2;                 
  UCA0CTL0 = 0x00;                        //Khong kiem tra chan le | 8 data bits | 1 stop bit 
  UCA0CTL1 |= UCSSEL_2 + UCSWRST;         //SMCLK | Cho phep thiet lap, tat UART 
  tempfactor = SMCLK_F/BAUDRATE;
  UCA0BR0 = (unsigned char) tempfactor&0x00FF;
  tempfactor >>= 8;
  UCA0BR1 = (unsigned char) (tempfactor&0x00FF);
  UCA0MCTL |= UCBRF_0 + UCBRS_0;
  UCA0CTL1 &=~ UCSWRST;                   //Vo hieu hoa qua trinh thiet lap va cho phep UART hoat dong
  IE2 |= UCA0RXIE;                        //Cho phep ngat nhan UART
}                                         
//-----------------Ham ngat nhan UART--------------------

#pragma vector = USCIAB0RX_VECTOR         // Vector ngat nhan UART
__interrupt void USCI0RX_ISR(void)        // Bat dau chuong trinh ngat nhan UART
{
  switch (UCA0RXBUF)         // So sanh gia tri trong thanh ghi UCA0RXBUF 
    {
    case 'a': TA0CCR1 = k;  break;      
    case 'b': TA0CCR1 = 0;  break;
    case 'c': UCA0TXBUF = (unsigned char)((GetAdcValue()*1.5*100)/1024);
              if (((GetAdcValue()*1.5*100)/1024) > 25)
                          TA0CCR1 = k; 
              else      
                          TA0CCR1 = 0;
              break;         
    }
}                                
//----------------------Ham khoi tao ADC-------------------
void Setup_ADC(void)                   
{
  ADC10CTL1 = INCH_5;                     // Chon kenh chuyen doi A5
  ADC10AE0 |= BIT5;                       // Loai kenh A5 khoi chuc nang GPIO
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + ADC10ON + ENC + REFON;                   
}                                         
//-------------------Ham lay gia tri ADC---------------------

int GetAdcValue(void) {                    
  ADC10CTL0 |= ADC10SC;                   //Bat dau chuyen doi ADC10
  while (!(ADC10CTL0 & ADC10IFG));        //Doi den khi chuyen doi xong ADC10
  return ADC10MEM;                        // Tra ve gia tri trong thanh ghi ADC10MEM
}   
void delayms(int ms){
  for(int i=0;i<ms;i++)
     __delay_cycles(1000);
}



