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

#define TIM3_FREQ 	  			(8000000.) 	      		// Hz
#define TIM3_TICK	  			(1000000./TIM3_FREQ)	// usec
#define TIM3_PLS_OF_1ms  		(1000./TIM3_TICK)

void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1);
	Macro_Set_Bit(RCC->APB1ENR, 1);

	Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);
	Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);

	Macro_Write_Block(TIM3->CCMR2,0xff, 0x68, 0);
	TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Delay(int time)
{
    unsigned int freq;
    unsigned int count;
    unsigned int i;

    freq = TIMXCLK / ((TIM3->PSC + 1) * (TIM3->ARR + 1));

    count = (unsigned int)
            ((unsigned long long)freq * time / 1000);

    Macro_Clear_Bit(TIM3->SR, 0);

    for(i = 0; i < count; i++)
    {
        while(Macro_Check_Bit_Clear(TIM3->SR, 0));

        Macro_Clear_Bit(TIM3->SR, 0);
    }
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM3->PSC=(int)(TIMXCLK/TIM5_FREQ+0.5)-1;
	// 요청한 주파수가 되도록 ARR 설정
	TIM3->ARR=(int)(TIM5_FREQ/freq+0.5);
	// Duty Rate 50%가 되도록 CCR3 설정
	TIM3->CCR3=TIM3->ARR/2;
	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM3->EGR,0);
	// Down Counter, Repeat Mode, Timer Start 
	TIM3->CR1=(0x1<<4)|(0x1<<0);

}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}