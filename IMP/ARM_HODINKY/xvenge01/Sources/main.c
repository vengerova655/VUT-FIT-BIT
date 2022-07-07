

/**
 * author: xvenge01(Veronika Vengerová)
 */

#include "MKL05Z4.h"

/* Activation of particular LED display (DS1 - DS4) */
#define D1 0x0700
#define D2 0x0B00
#define D3 0x0D00
#define D4 0x0E00


/* Encoding of digits as active segments on specific LED display (DS1 - DS4) */
#define N0 0x0707
#define N1 0x0006
#define N2 0x0B03
#define N3 0x0907
#define N4 0x0C06
#define N5 0x0D05
#define N6 0x0F05
#define N7 0x0007
#define N8 0x0F07
#define N9 0x0D07



/* Bit-level masks that help to enable/disable DP segment on LED display */
#define MASK_DOT_ON 0x0008
#define MASK_DOT_OFF 0xFFF7


#define PB4_ISF_MASK 0x10


int show_dot = 0;

//turn on/off display
int isActive=1;

//display number
int d1=0;
int d2=0;
int d3=0;
int d4=0;

/* Just an ordinary delay loop */
void delay(long long bound) {

  long long i;
  for(i=0;i<bound;i++);
}


/* Let's turn off individual segments on the whole display */
void off() {

  PTB->PDOR = GPIO_PDOR_PDO(0x0000);
  PTA->PDOR = GPIO_PDOR_PDO(D1);
  PTA->PDOR = GPIO_PDOR_PDO(D2);
  PTA->PDOR = GPIO_PDOR_PDO(D3);
  PTA->PDOR = GPIO_PDOR_PDO(D4);

}

void RTC_Seconds_IRQHandler( void ) {

	//shows dot every other second
	if(show_dot){
		show_dot=0;
	}else{
		show_dot=1;
	}
	//sets digits to show
	if(RTC_TSR%60==0){
		d4 = (d4+1);
		if(d4>9){
			d3=(d3+1);
			d4=d4%10;
		}
		if(d3>5){
			d3=d3%6;
			d2=(d2+1);
		}
		if(d2>9){
			d2=(d2%10);
			d1=d1+1;
		}
		if(d2>2){
			d2=d2%3;
		}
	}
}

//void sleep(){
//
//	SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
//	SMC_PMCTRL = 1<<6;
//	__WFI;
//}

void RTCInit() {

    RTC_CR = RTC_CR_SWR_MASK;  // SWR = 1, reset all RTC's registers
    RTC_CR &= ~RTC_CR_SWR_MASK; // SWR = 0

    RTC_TCR = 0x0000; // reset CIR and TCR

    RTC_CR |= RTC_CR_OSCE_MASK; // enables oscillator


    RTC_SR &= ~RTC_SR_TCE_MASK;//turn off rtc

    //sets TSR register to zero
    RTC_TSR = 0;

    RTC_SR = RTC_SR_TCE_MASK; //turn on rtc

    //interrupt every second
    RTC_IER = RTC_IER_TSIE_MASK;

    NVIC_ClearPendingIRQ(RTC_Seconds_IRQn );
    NVIC_EnableIRQ(RTC_Seconds_IRQn );

}



/* Basic initialization of GPIO features on PORTA and PORTB */
void ports_init (void)
{
  SIM->SCGC5 = (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);  // Turn on clocks for PORTA and PORTB
  SIM->SCGC6 = SIM_SCGC6_RTC_MASK;								//turn on clock for RTC

  /* Set corresponding PORTA pins for GPIO functionality */
  PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // display DS4
  PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );  // display DS3
  PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // display DS2
  PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // display DS1

  /* Set corresponding PORTA port pins as outputs */
  PTA->PDDR = GPIO_PDDR_PDD( 0x0F00 );  // "1" configures given pin as an output


  NVIC_DisableIRQ(31);  // Disable the eventual generation of the interrupt caused by the control button

  /* Set corresponding PORTB pins for GPIO functionality */
  PORTB->PCR[0] = ( 0|PORT_PCR_MUX(0x01) );   // seg A
  PORTB->PCR[1] = ( 0|PORT_PCR_MUX(0x01) );   // seg B
  PORTB->PCR[2] = ( 0|PORT_PCR_MUX(0x01) );   // seg C
  PORTB->PCR[3] = ( 0|PORT_PCR_MUX(0x01) );   // seg DP
  PORTB->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );   // seg D
  PORTB->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // seg E
  PORTB->PCR[10] = ( 0|PORT_PCR_MUX(0x01) );  // seg F
  PORTB->PCR[11] = ( 0|PORT_PCR_MUX(0x01) );  // seg G

  /* Set corresponding PORTB port pins as outputs */
  PTB->PDDR = GPIO_PDDR_PDD( 0x0F0F ); // "1" configures given pin as an input
  PORTB->PCR[4] = ( 0 | PORT_PCR_ISF(1) | PORT_PCR_IRQC(0x0A) | PORT_PCR_MUX(0x01) |
					    PORT_PCR_PE(1) | PORT_PCR_PS(1)); // display SW1

  /* Let's clear any previously pending interrupt on PORTB and allow its subsequent generation */
 NVIC_ClearPendingIRQ(31);
 NVIC_EnableIRQ(31);
}

/* Service routine invoked upon the press of a control button */
void PORTB_IRQHandler( void )
{
	delay(100);

	if (PORTB->ISFR & GPIO_PDIR_PDI(PB4_ISF_MASK)) {
	  if (!(PTB->PDIR & GPIO_PDIR_PDI(PB4_ISF_MASK))) {
	    if(isActive){
	    	isActive=0;
	    }
	    else{
	    	isActive=1;
	    }
	  }

	  PORTB->PCR[4] |= PORT_PCR_ISF(0x01);  // Confirmation of interrupt after button press
	}
}


/* Single digit shown on a particular section of the display  */
void sn(int number, uint32_t display) {

  uint32_t n;

  switch (number) {
    case 0:
      n = N0; break;
    case 1:
      n = N1; break;
    case 2:
      n = N2; break;
    case 3:
      n = N3; break;
    case 4:
      n = N4; break;
    case 5:
      n = N5; break;
    case 6:
      n = N6; break;
    case 7:
      n = N7; break;
    case 8:
      n = N8; break;
    case 9:
      n = N9; break;
    default:
      n = N0;
  }

  if(display==D2){
	  if (show_dot)
		n |= MASK_DOT_ON;
	  else
		n &= MASK_DOT_OFF;
  }
  PTA->PDOR = GPIO_PDOR_PDO(display);
  PTB->PDOR = GPIO_PDOR_PDO(n);

  delay(1000); //10
}





void MCUInit() {
    SIM->COPC = SIM_COPC_COPT(0x00);							   // Just disable the usage of WatchDog feature
}


int main(void)
{
	MCUInit();
	ports_init();
	RTCInit();
	delay(500);
	for (;;) {

		if(isActive){
			sn(d1,D1);
			sn(d2,D2);
			sn(d3,D3);
			sn(d4,D4);
		}
		else{
			off();
		}

	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
