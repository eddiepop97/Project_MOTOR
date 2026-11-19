#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;

void USART2_IRQHandler(void)
{
	Uart_Data = (unsigned char)USART2->DR;
	Uart_Data_In = 1;
	NVIC_ClearPendingIRQ(38);
}

extern volatile int run;
extern volatile int direction;
extern volatile int start_flag;

void EXTI15_10_IRQHandler(void)
{
    TIM3_Stopwatch_Start();
    Key_Wait_Key_Released();
    unsigned int t = TIM3_Stopwatch_Stop();

    printf("t: %d\n", t);
    if (t >= 2000000) { run = !run; }
    else if (t < 2000000) 
    {

        start_flag = 1;
        direction = !direction;
    }

    EXTI->PR = (0x1 << 13);
    NVIC_ClearPendingIRQ((IRQn_Type)40);
}