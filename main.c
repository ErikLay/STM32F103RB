
#include "STM32f10x.h"
#include "GT_init.h"
#include <stdio.h>
#include <stdlib.h>

//Declare the functions 
void showLED(uint16_t res[]);
uint16_t compare(uint16_t res[]);



int main(void){
	uint16_t result=4;
	uint16_t resultado[4];
	GPIOx_Init();
	TIMs_Init();
	TIMs_Enable(2);
	while((TIM2->SR & 0x0001) == 0);
	TIMs_Disable(2);
	TIMs_Enable(3);
	while((TIM3->SR & 0x0001) == 0);
	TIMs_Disable(3);
	while(result==4){														//main loop 
		showLED(resultado);
		result=compare(resultado);
		Substract_Prescaler();
	}
}

void showLED(uint16_t result[]){
	uint16_t   aux=0xF, res[4], aux2=0;
	uint16_t number=rand();											//Initialize number with a random number			
		for(int cont=0;cont<4;cont++){						//This loop is to split the numbers in each posistion
			*(res+cont)=(number&aux)>>aux2;
			aux=aux<<4;
			aux2+=4;
			*(result+cont)=cont;
		}
		for(int con1=0;con1<4;con1++){						//This loop is a bubble method in order to place 
			aux2=0;																	//each number in ther right position
			for(int con2=con1;con2<4;con2++){
				if(*(res+con1)<*(res+con2)){
					aux=*(res+con1);
					*(res+con1)=*(res+con2);
					*(res+con2)=aux;
					aux=*(result+con1);
					*(result+con1)=*(result+con2);
					*(result+con2)=aux;
				}
			}
		}
		for(int cont=0;cont<4;cont++){						//This loop is to show the sequence previously created
			if(*(result+cont)==0)
				GPIOC->ODR=0b0000000000000001;
			else if(*(result+cont)==1)
				GPIOC->ODR=0b0000000000000010;
			else if(*(result+cont)==2)
				GPIOC->ODR=0b0000000000000100;
			else if(*(result+cont)==3)
				GPIOC->ODR=0b0000000000001000;
			TIMs_Enable(2);
			while((TIM2->SR & 0x0001) == 0);
			TIMs_Disable(2);
		}
		GPIOC->ODR=0;
}

uint16_t compare(uint16_t res[]){							//this Function compare the output shown with the input  
	int counter=0;
	for(int cont=0; cont<4;cont++){							//this loop compare each bit in the right position
		GPIOC->ODR |= (1<<cont);
		while((GPIOC->IDR&0x0400)==0);						//this loop is to wait until a push button gives the signal
		TIMs_Enable(3);
		while((TIM3->SR & 0x0001) == 0){}					//this timer is to avoid the rebound effect of the push button
  	TIMs_Disable(3);
		if((1<<6)==((GPIOC->IDR>>*(res+cont))&0b1111000000)){
			counter++;
		}
	}
	if(counter==4)
		GPIOC->ODR=0x0020;
	else
		GPIOC->ODR=0x0010;
	while((GPIOC->IDR&0x0400)==0);
	GPIOC->ODR=0;
	TIMs_Enable(3);
	while((TIM3->SR & 0x0001) == 0);
	TIMs_Disable(3);
	return counter;															//if the counter is equal to 4 the program will continue increasing the speen 
}