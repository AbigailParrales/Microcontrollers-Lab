#include "derivative.h" /* include peripheral declarations */
//PINES
//	NOMBRE				       PIN				   DESCRIPCION					
//	Pot------------------------ADC1_SE18-----------Entrada analogica para el potenciometro
//
//	Led RGB conectado a los pines PTB22(R),PTB21 (B)y PTE26 (G) de la tarjeta

#define CLOCK_VALUE 1000 //Hz
#define TIME 1000 //ms
#define VAL1 16391
#define VAL2 32782
#define VAL3 49173

void vPort_Init(void);
void vLPTMR_Init(void);
void vADC_init(void);

enum{
	RED,
	GREEN,
	BLUE,
	WHITE,
	YELLOW,
	PINK,
	CYAN,
	ON,
	OFF,
	TOOGLE
};

unsigned char compare;
unsigned char send;

unsigned int medicion;
unsigned int voltage;

//Variables para ascii sending
unsigned long centenas;
unsigned long decenas;
unsigned long unidades;
unsigned long millares;
unsigned long decenasDeMillares;
unsigned long num_sn_unidades;

void vADC_init(void){
	//Configurar ADC1
	SIM_SCGC3|= 1<<27;			//clock ADC1
	ADC1_CFG1=0x0C;				//Conversión de 16 bits
	NVICICER2=(1<<(73%32));		//Configure NVIC for ADC1
	NVICISER2|=(1<<(73%32));	//Enable NVIC
}

void vPort_Init(void){
	/* Enable Port Clock gates
	 * SIM_SCGC5 (pag. 323)
	 * PORTE(13)
	 * PORTD(12)
	 * PORTC(11)
	 * PORTB(10)
	 * PORTA(9)
	 */
	SIM_SCGC5 =   (1<<10) + (1<<13);	//Habilitar reloj

	/* Select MUX for GPIO and Pull Enable
	 * PORTx_PCRn (pag. 244, 288)
	 */
	PORTB_PCR21 = 0x00000100;		//Led Rojo	
	PORTB_PCR22 = 0x00000100;		//Led Azul
	PORTE_PCR26 = 0x00000100;		//Led Verde


	/* Select Direction of the port
	 * GPIOx_PDDR (pag. 1803)
	 */
	GPIOB_PDDR |= (1<<21) + (1<<22);//Dirección del pin (ouput)
	GPIOE_PDDR |= (1<<26);

	/* All outputs off
	 * GPIOx_PDDR (pag. 1803)
	 */
	GPIOB_PSOR = (1<<21) + (1<<22);
	GPIOE_PSOR = (1<<26);
}

void vLPTMR_Init(void){
	//Configurar LPTMR
	SIM_SCGC5|=1;
	LPTMR0_PSR = 0x05; //Configurar LPTMR
	LPTMR0_CMR = TIME;
	LPTMR0_CSR = (1<<6) + 1;; //Habilitar LPTMR e Interrupción
	NVICICER1=(1<<(58%32));		//Configure NVIC for LPTMR0
	NVICISER1|=(1<<(58%32));	//Enable NVIC	
}

void vUART_Init(void){
	SIM_SCGC4|=(1<<10);		//Hab clk UART0 
	UART0_BDH=0;
	UART0_BDL=136;			//BR 9600
	//UART0_BDL=11;			//BR 115200

	//UART0_C1=0;
	UART0_C2=12;           // bit 3: Hab Tx, bit 2: Hab Rx

	//UART pins init
	SIM_SCGC5=0x00002C00; //Hab clk PORTB (PB16 y 17 son Rx y Tx) y PORTC
	PORTB_PCR16=0x00000300; //Hab clk PB16 Rx
	PORTB_PCR17=0x00000300; //Hab clk PB17 Tx

	NVICICER0=(1<<(31%32));		//Configurar NVIC para UART0
	NVICISER0|=(1<<(31%32));
}

void vUART_send(unsigned char dato){
	do{}while(!(UART0_S1& 0x80));	//(1<<7)=0x80=TDRE=Transmit Data Register Empty Flag
	UART0_D=dato; //mandar 'A'
	//send=0;
}


void compareVoltage(unsigned int medicion){
	if(medicion < VAL1) setLEDColor(OFF);
	else if (medicion < VAL2) setLEDColor(GREEN);
	else if (medicion < VAL3) setLEDColor(BLUE);
	else setLEDColor(RED);
	compare=0;
	/*if(send&(1<<0)){
		vUART_send('B');
		send=0;
	}*/
}

void vUART_send_adcPrint (void){
	voltage =(3300*medicion)/65536;
	
	//voltage = 90000;
	
	//Prepararse para enviar Ascii
	decenasDeMillares=(voltage/10000);
	millares=((voltage-(decenasDeMillares*10000))/1000);
	centenas=((voltage-((decenasDeMillares*10000)+(millares*1000)))/100);
	decenas=((voltage-((decenasDeMillares*10000)+(millares*1000)+(centenas*100)))/10);
	num_sn_unidades=(decenasDeMillares*10000)+(millares*1000)+(centenas*100)+(decenas*10);
	unidades=voltage-num_sn_unidades;

	vUART_send(decenasDeMillares+48);
	vUART_send(millares+48);
	vUART_send(centenas+48);
	vUART_send(decenas+48);
	vUART_send(unidades+48);

	vUART_send('\r');
	vUART_send('\n');
}


void setLEDColor(unsigned char color){
	switch(color){
	case RED:
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PCOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	case BLUE:
		GPIOB_PCOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	case GREEN:
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PCOR = (1<<26); //Led Verde
		break;
	case WHITE:
		GPIOB_PCOR = (1<<21); //Led Rojo
		GPIOB_PCOR = (1<<22); //Led Azul
		GPIOE_PCOR = (1<<26); //Led Verde
		break;
	case YELLOW:
		GPIOB_PCOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PCOR = (1<<26); //Led Verde;
		break;
	case OFF:
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	case PINK:
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	case CYAN:
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	case TOOGLE:
		GPIOB_PTOR = (1<<21);
		break;
	default: //OFF
		GPIOB_PSOR = (1<<21); //Led Rojo
		GPIOB_PSOR = (1<<22); //Led Azul
		GPIOE_PSOR = (1<<26); //Led Verde
		break;
	}
}

int main(void){
	vPort_Init();
	vUART_Init();
	vADC_init();
	vLPTMR_Init();


	while(1){
		//Ciclo infinito
		if(compare&(1<<0)){				//Solo vamos a lanzar al LED la instucción si la bandera se activó en el handler
			compareVoltage(medicion);
		}
	}	
	return 0;
}

void LPTimer_IRQHandler(void){
	LPTMR0_CSR|=(1<<7);					//Apagar bandera del TIMER
	ADC1_SC1A=(1<<6)+18;				//Activar conversión del canal 18 y activar interrupción

}

void ADC1_IRQHandler(void){

	ADC1_SC1A=(1<<7)+0x1F;	     //Turn off COCO	and stop conversion
	medicion = ADC1_RA;			//RA is the data output
	compare=1;
	//Activar interrupción UART0
	 UART0_C2|=(1<<7);			//Turn on TIE (interruption for transmit)
}

void UART0_Status_IRQHandler(){
	
	if(UART0_S1&(1<<7)){				//TDRE		Sending			(This turns off TDRE according to pg 1585)
		//Turn off the flag pg 1585//
		(void)UART0_S1;		//Read S1
		UART0_D=' ';
		/////////////////////
		//send=1;							//Turn on the flag for send a msg
		vUART_send_adcPrint();
		UART0_C2=(0<<7)+(1<<3);

	}
}

