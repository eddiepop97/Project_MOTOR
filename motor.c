#include "device_driver.h"
#include <stdio.h>
#define TIM2_FREQ 	  			(8000000.) 	      		// Hz
#define TIM2_TICK	  			(1000000./TIM2_FREQ)	// usec
#define TIME2_PLS_OF_1ms  		(1000./TIM2_TICK)

extern volatile int speed;
extern volatile int direction;

void Motor_Run(int speed, int direction)
{
    printf("방향: %d\n",direction);

    if(!direction) //CW
    {
        TIM2->CCR2 = (TIM2 -> ARR +1)*speed/100;
        TIM2->CCR1 = 0;
    }
    else //CCW
    {
        TIM2->CCR2 = 0;
        TIM2->CCR1 = (TIM2 -> ARR +1)*speed/100; 
    }
    
    return;
}
void Motor_Stop(void)
{
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
    speed = 0;
}

void Motor_Init(void)
{
    Macro_Set_Bit(RCC->APB1ENR,0); //TIM2 활성화

    Macro_Write_Block(GPIOA->MODER,0x3,0x2,0); //ALTERNATE 
    Macro_Write_Block(GPIOA->MODER,0x3,0x2,2); //ALTERNATE 

    Macro_Write_Block(GPIOA->AFR[0],0xf,0x1,0); //PA0 TIM2로 만들기 AF01
    Macro_Write_Block(GPIOA->AFR[0],0xf,0x1,4); //PA1 TIM2로 만들기 AF01

    Macro_Write_Block(TIM2->CCMR1,0xff,0x68,0); //PWM1 설정 PA0
    Macro_Write_Block(TIM2->CCMR1,0xff,0x68,8); //PWM1 설정 PA1

    TIM2->CR1=(0x0<<7)|(0x1<<4)|(0x0<<3);
    TIM2->PSC=(unsigned int)(TIMXCLK/TIM2_FREQ + 0.5)-1;
    TIM2->ARR = 8000000/1000-1;
    TIM2->CCER=(1<<0)|(0<<1)|(1<<4)|(0<<5); //각각 출력 활성화 및 active high(이래야 duty구하기 편함)

    TIM2->CCR1=0;    //초기값 0 
    TIM2->CCR2=0;    //초기값 0

    Macro_Set_Bit(TIM2->EGR,0);
    Macro_Set_Bit(TIM2->CR1,0); 
}