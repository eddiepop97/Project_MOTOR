#include "device_driver.h"

#define TIM5_TICK         	(20) 					// usec
#define TIM5_FREQ 	  		(1000000./TIM5_TICK)	// Hz
#define TIM5_PLS_OF_1ms  	(1000./TIM5_TICK)
#define TIM5_MAX              (0xffffffffu)

void TIM5_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 3);

	TIM5->CR1 = (1<<4)|(1<<3);
	TIM5->PSC = (unsigned int)(TIMXCLK/TIM5_FREQ + 0.5) - 1;
	TIM5->ARR = TIM5_MAX;

	Macro_Set_Bit(TIM5->EGR, 0);
	Macro_Set_Bit(TIM5->CR1, 0);
}

unsigned int TIM5_Stopwatch_Stop(void)
{
	unsigned int time;

	Macro_Clear_Bit(TIM5->CR1, 0);
	time = (TIM5->ARR - TIM5->CNT) * TIM5_TICK;
	return time;
}


void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1);
	Macro_Set_Bit(RCC->APB1ENR, 1);

	Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);
	Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);

	Macro_Write_Block(TIM3->CCMR2,0xff, 0x68, 0);
	TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}



