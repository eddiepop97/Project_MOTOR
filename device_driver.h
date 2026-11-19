#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

extern void Uart2_Init(int baud);
extern char Uart2_Get_Pressed(void);
extern void Check_Uart(char input);
extern void Uart2_RX_Interrupt_Enable(int en);
// Led.c

extern void LED_Init(void);
extern void LED_On(void);
extern void LED_Off(void);
extern void LED_Display(int on);

// Clock.c

extern void Clock_Init(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern void Key_Wait_Key_Pressed(void);
extern void Key_ISR_Enable(int en);

// Motor.c

extern void Motor_Run(int speed, int direction);
extern void Motor_Stop(void);
extern void Motor_Init(void);


// Timer.c
extern void TIM3_Stopwatch_Start(void);
extern unsigned int TIM3_Stopwatch_Stop(void);
extern void TIM3_Out_Init(void);
extern void Change_Direction(short duty);