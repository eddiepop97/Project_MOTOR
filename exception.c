#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

extern volatile int speed;
extern volatile int run;
extern volatile int direction;
extern volatile int start_flag;
extern volatile int long_pulse_flag;

/*
extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;
*/
void USART2_IRQHandler(void)
{
    unsigned char input = (unsigned char)USART2->DR;

    printf("input: %c\n",input);

    if((input >= '1')&&(input <= '9'))
    {   
        speed = 50 + ((int)(input) - 48) * 5;
    }
    else if(input=='F')
    {
        direction = 0;
    }
    else if(input=='R')
    {
        direction = 1;
    }
    start_flag = 1;

	NVIC_ClearPendingIRQ(38);
}

void EXTI15_10_IRQHandler(void)
{
    TIM5_Stopwatch_Start();
	Key_Wait_Key_Released();
	unsigned int t = TIM5_Stopwatch_Stop();

    printf("t: %d\n", t);
    if (t >= 2000000) 
    {
        long_pulse_flag = 1;
    }
    else if (t < 2000000) 
    {
        start_flag = 1;
        direction = !direction;
    }

    EXTI->PR = (0x1 << 13);
    NVIC_ClearPendingIRQ((IRQn_Type)40);
}