#include "derivative.h" /* include peripheral declarations */
#define DELAY_T 100000
#define TIMER_CNTS 1000

unsigned int i=0;
//unsigned char sw='0';
unsigned long leds[]={14,13,11,7};

//PTE 24-26
void init(void){
	//clks
	SIM_SCGC5=0x00001001;		//Enable clock in page 323

	//Page 288 -> PCR
	//Page 244 -> 100 pins table
	PORTD_PCR0=0x00000100;		//Select GPIO
	PORTD_PCR1=0x00000100;
	PORTD_PCR2=0x00000100;
	PORTD_PCR3=0x00000100;

	GPIOD_PDDR=0x0000000F;		//GPIO Output
	//LPTMR
	LPTMR0_PSR=5;

	//249
	//////////////////SW////////////////////
	SIM_SCGC5|=1<<11;		//Enable PTC clock in page 323

	PORTC_PCR6=1<<8;		//Select GPIO ptc6>>sw2
	GPIOC_PDDR|=0<6;		//GPIO PTC6 configured as Input

}

void delay(void){
	LPTMR0_CMR=TIMER_CNTS;
	LPTMR0_CSR=1;
	do{}while(!(LPTMR0_CSR&(1<<7)));
	LPTMR0_CSR|=(1<<7);
	//GPIOD_PDDR=(1<<(i--%4));
	//GPIOD_PDOR=(1<<0);
	//GPIOD_PDOR=0x0000000F;		//Onn
}

int main(void){
	unsigned int index;
	init();
	while(1){
		if(!(GPIOC_PDIR & (1<<6))){
			//if(sw == '0'){
			index = i++%4;
			GPIOD_PDOR=leds[index];
		}
		else if((GPIOC_PDIR & (1<<6))){
			//else if (sw == '1'){
			index = i--%4;
			GPIOD_PDOR=leds[index];
		}
		delay();
	}
	return 0;
}

