#include "device_driver.h"
#include <stdio.h>

#define BASE  (350)

void Buzzer_Speed_Beep(int first, int second)
{
    TIM3_Out_Freq_Generation(first);
	TIM3_Delay(100);
    TIM3_Out_Freq_Generation(second);
    TIM3_Delay(100);
	TIM3_Out_Stop();
}

void Buzzer_Beep(unsigned char tone, int duration)
{
	const unsigned short tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};

	TIM3_Out_Freq_Generation(tone_value[tone]);
	TIM3_Delay(duration);
	TIM3_Out_Stop();
}

void Play_Song()
{
    int i;
	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
	const int song1[][2] = {
    // 1마디: 미(4분) 미(4분) 도(2분)
    {E2, N4}, {E2, N4}, {C2, N2},
    // 2마디: 라(점4분) 시(8분) 도(4분) 라(4분)
    {A1, N4 + N8}, {B1, N8}, {C2, N4}, {A1, N4},
    // 3마디: 레(점4분) 미(8분) 레(4분) 도(4분)
    {D2, N4 + N8}, {E2, N8}, {D2, N4}, {C2, N4},
    // 4마디: 시(4분) 라(4분) 솔(2분)
    {B1, N4}, {A1, N4}, {G1, N2},
    // 5마디: 미(4분) 미(4분) 도(2분)
    {E2, N4}, {E2, N4}, {C2, N2},
    // 6마디: 라(점4분) 시(8분) 도(4분) 라(4분)
    {A1, N4 + N8}, {B1, N8}, {C2, N4}, {A1, N4},
    // 7마디: 레(점4분) 미(8분) 레(4분) 시(4분)
    {D2, N4 + N8}, {E2, N8}, {D2, N4}, {B1, N4},
    // 8마디: 도(2분)
    {C2, N2}
	};

    for(i=0; i<(sizeof(song1)/sizeof(song1[0])); i++)
	{
		Buzzer_Beep(song1[i][0], song1[i][1]);
	}

}