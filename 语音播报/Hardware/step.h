#ifndef __STEP_H__
#define __STEP_H__
void step_Init(void);//初始化函数
void STEP_MOTOR_8A (u8 a,u16 speed);//电机单步8拍
void STEP_MOTOR_NUM (u8 RL,u16 num,u16 speed);//电机按步数运行
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u16 speed);//电机按圈数运行
void STEP_MOTOR_Angle(u8 RL, u32 angle, u16 speed);//步进电机按角度旋转
#endif
