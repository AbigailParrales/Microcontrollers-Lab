#include "derivative.h" /* include peripheral declarations */
#define DELAY_T 1000

unsigned char sw='0';
unsigned char leds=0;
//PTE 24-26
void init(void){
	SIM_SCGC5=0x00001000;		//Enable clock in page 323

	//Page 288 -> PCR
	//Page 244 -> 100 pins table
  	PORTD_PCR0=0x00000100;		//Select GPIO
	PORTD_PCR1=0x00000100;
	PORTD_PCR2=0x00000100;
	PORTD_PCR3=0x00000100;

	GPIOD_PDDR=0x0000000F;		//GPIO Output
	
	sw='0';
}

void delay(void){
	unsigned long i;
	for(i=0;i<DELAY_T;i++);
}

void rightSh(void){
	leds=(1>>4);
	delay();
	leds=(1>>5);
	delay();
	leds=(1>>6);
	delay();
	leds=(1>>7);
	delay();
}

void leftSh(void){
	leds=(1<<0);//A
	delay();
	leds=(1<<1);//B
	delay();
	leds=(1<<2);//C
	delay();
	leds=(1<<3);
	delay();
}

void bitSpin(void){
	switch (sw) {
		case '0':
			leftSh();
			break;
		case '1':
			rightSh();		
			break;
		default:
			break;
	}

	
}

 int main(void){
	//init();
	bitSpin();
	sw='1';
	bitSpin();
	sw='0';   
	

	return 0;
}
