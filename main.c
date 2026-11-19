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

void Check_Uart(char input);

volatile int Uart_Data_In = 0;
volatile unsigned char Uart_Data = 0;
extern int speed=0;
extern int direction=0;
extern int run=0;
extern int start_flag = 0;

void Main(void)
{
    Sys_Init(115200);
    Key_Poll_Init();
    Motor_Init();    
    
    Uart2_RX_Interrupt_Enable(1);
    Key_ISR_Enable(1);

    for (;;) 
    {
        if(Uart_Data_In)
		{
			Check_Uart(Uart_Data);
			Uart_Data_In = 0;
		}

        if(start_flag)
        {
            start_flag = 0;
        }

    }
}

void Check_Uart(char input)
{
    printf("input: %c\n",input);
    printf("방향: %d\n",direction);
    if((input>='1')&&(input<='9'))
    {   
        speed=(int)(input)-48;
        speed=50+speed*5;
        printf("%d\n",speed);
        Change_Speed(speed);
    }
    else if((input=='F')||(input=='R'))  
    {
        {
            Motor_Run(speed, direction);
        }
}
