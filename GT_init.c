
#include "STM32f10x.h"
#include "GT_init.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t PrescalerT2 = 5500, counter=0;


void GPIOx_Init(){
	//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			//ENABLE GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;			//ENABLE GPIOC
	/*  Bit 0 GPIOB */											//Input
	//GPIOA->CRL |= 8;				// Input with pull-up / pull-down
	GPIOC->CRL = 0x88111111;								//Enable bits 0 to 5 as output
	GPIOC->CRH = 0x00000888;								//Enable bits 6 to 9 as input
	GPIOC->IDR = 0;													//initialize IDR and ODR
	GPIOC->ODR = 0;
}

void TIMs_Init(){
	//Enable clock in the timers
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM2->PSC = PrescalerT2;								//Timer for the response
	TIM2->ARR = 0xFFFF;
	TIM3->PSC = 2400;												//Timer to avoid rebound of the pulse
	TIM3->ARR = 60000;
	TIM2->DIER |= TIM_DIER_TIE;							//ENABLE the interrupt
	TIM3->DIER |= TIM_DIER_TIE;
	TIM2->CR1 |= TIM_CR1_ARPE;						  //Enable Auto-Reload preload enable				
	TIM2->CR1 |= TIM_CR1_URS;								//Update the interrupt
	TIM3->CR1 |= TIM_CR1_ARPE;										
	TIM3->CR1 |= TIM_CR1_URS;
}

void TIMs_Enable(int indicator){
	if(indicator==2){
		TIM2->CR1 |= TIM_CR1_CEN;							//ENABLE timer
		TIM2->CNT=0;													//Reset counter
		TIM2->SR &= ~(1<<0);                  // clear UIF flag
	}
	else if(indicator==3){
		TIM3->CR1 |= TIM_CR1_CEN;							//ENABLE timer
		TIM3->CNT=0;													//Reset counter
		TIM3->SR &= ~(1<<0);                  // clear UIF flag
	}
}

void TIMs_Disable(int indicator){
	if(indicator==2)
		TIM2->CR1 &= ~(TIM_CR1_CEN);					//Dsable timer
	else if(indicator==3)
		TIM3->CR1 &= ~(TIM_CR1_CEN);
}

/*This function substract the prescaler 
in order to reduce the time and to increment 
the difficulty*/

void Substract_Prescaler(){								
	if (counter<5)
		PrescalerT2-=1000;
	else
		PrescalerT2-=100;
	TIM2->PSC = PrescalerT2;
	counter++;
}