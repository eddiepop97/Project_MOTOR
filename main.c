#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

// void Check_Uart(char input);

volatile int Uart_Data_In = 0;
volatile unsigned char Uart_Data = 0;
volatile int speed=0;
volatile int direction=0;
volatile int run=0;
volatile int start_flag = 0;
volatile int long_pulse_flag = 0;

void Main(void)
{
    Sys_Init(115200);
    Key_Poll_Init();
    Motor_Init();    
    
    Uart2_RX_Interrupt_Enable(1);
    Key_ISR_Enable(1);

    for (;;) 
    {
        if (long_pulse_flag)
        {
            if (!run) 
            {
                speed = 55;
                Motor_Run(speed, direction);
                run = 1;
            }
            else 
            {
                Motor_Stop();
                run = 0;
            }
            long_pulse_flag = 0;
        }

        if(run && start_flag)
        {
            Motor_Run(speed, direction);
            start_flag = 0;
        }
    }
}

/*
void Check_Uart(char input)
{
    printf("input: %c\n",input);

    if((input>='1')&&(input<='9'))
    {   
        speed=(int)(input)-48;
        speed=50+speed*5;
        printf("%d\n",speed);
        Motor_Run(speed, direction);
    }
    else if(input=='F')
    {
        direction=0;
        Motor_Run(speed, direction);
    }
    else if(input=='R')
    {
        direction=1;
        Motor_Run(speed, direction);
    }
}
*/

