#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "SysTick.h"
#include "step.h"
#include "key.h"


int main(void)
{

	SysTick_Init(72);
	step_Init();
	KEY_Init();
	u8 key;
	
	//STEP_MOTOR_LOOP(1,1,1500);
	

	
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == KEY0_PRESS)//��
		{
			STEP_MOTOR_Angle(1,200,1500);//����
		}
		else if(key == KEY1_PRESS)//�ر�
		{
			STEP_MOTOR_Angle(0,200,1500);//�ش�
		}
		
	}
}
