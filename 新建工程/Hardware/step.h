#ifndef __STEP_H__
#define __STEP_H__
void step_Init(void);//��ʼ������
void STEP_MOTOR_8A (u8 a,u16 speed);//�������8��
void STEP_MOTOR_NUM (u8 RL,u16 num,u16 speed);//�������������
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u16 speed);//�����Ȧ������
void STEP_MOTOR_Angle(u8 RL, u32 angle, u16 speed);//����������Ƕ���ת
#endif
