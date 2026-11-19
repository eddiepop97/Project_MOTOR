#include "device_driver.h"
#define TIM3_TICK             (20)                     // usec
#define TIM3_FREQ               (1000000./TIM3_TICK)    // Hz
#define TIM3_PLS_OF_1ms      (1000./TIM3_TICK)
#define TIM3_MAX              (0xffffffffu)

void TIM3_Stopwatch_Start(void)
{
    Macro_Set_Bit(RCC->APB1ENR, 0);

    TIM3->CR1 = (1<<4)|(1<<3);
    TIM3->PSC = (unsigned int)(TIMXCLK/TIM3_FREQ + 0.5)-1;
    TIM3->ARR = TIM3_MAX;

    Macro_Set_Bit(TIM3->EGR,0);
    Macro_Set_Bit(TIM3->CR1, 0);
}

unsigned int TIM3_Stopwatch_Stop(void)
{
    unsigned int time;

    Macro_Clear_Bit(TIM3->CR1, 0);
    time = (TIM3_MAX - TIM3->CNT) * TIM3_TICK;
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

void TIM3_Out_Freq_Generation_2(unsigned short freq)
{
	TIM3->PSC = (unsigned int)(TIMXCLK/TIM3_FREQ + 0.5)-1;
	TIM3->ARR = TIM3_FREQ/freq-1;
	TIM3->CCR3 = TIM3->ARR/2;

	Macro_Set_Bit(TIM3->EGR,0);
	TIM3->CR1 = (1<<4)|(0<<3)|(0<<1)|(1<<0);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	TIM3->PSC = (unsigned int)(TIMXCLK/TIM3_FREQ + 0.5)-1;
	TIM3->ARR = TIM3_FREQ/freq-1;
	TIM3->CCR3 = TIM3->ARR/4;

	Macro_Set_Bit(TIM3->EGR,0);
	TIM3->CR1 = (1<<4)|(0<<3)|(0<<1)|(1<<0);
}


void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

void Change_Speed(short duty)
{
	TIM2->CCR1=(TIM2->ARR+1)*duty/100;
	TIM2->CCR2=0;
	Macro_Set_Bit(TIM2->EGR,0);
}

void Change_Direction(short duty)
{
	TIM2->CCR1=0;
	TIM2->CCR2=(TIM2->ARR+1)*duty/100;
	Macro_Set_Bit(TIM2->EGR,0);
}
