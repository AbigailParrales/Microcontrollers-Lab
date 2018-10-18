#include "derivative.h" /* include peripheral declarations */
unsigned long z;
void calcula (void){
}

void delay(unsigned long a, unsigned long b, unsigned long c){
	z=a+b+c;
	calcula(); 
}

int main(void){
	delay(1,2,3);
	for(;;){
		
	}
	
	return 0;
}
