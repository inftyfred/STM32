#include "stm32f10x.h"                  // Device header
#include "Delay.h"

u8 STEP;//����

//������ٶȿ�����ͨ����ʱ����ɵ� speedʵ��Ϊ��ʱ��ʱ�� ��λΪus  -> ÿ speed us תһ�� һȦΪ4096��
//������ speed ԽСת���ٶ�Խ�� ����Ҫע�ⲻ����̫С ��������ת�ǶȻ��кܴ����������ת
//�������� speed >= 943 ʱ ����������ת ��һ�㽨�鳬��1000
//-> ����Ҫ�޸��ٶȵ��߼� ����STEP_MOTOR_8A�������޸�
//�����Ƕ� 5.625/64 ��һ��ת�ĽǶ� 
//�����ϵ4096 * �����Ƕ� = 360��


//����
//����֮�� ���������Ӧ�ý�5v�͵�
#define STEP_MOTOR_PORT	GPIOB	//����IO�ӿ�������
#define STEP_MOTOR_A	GPIO_Pin_11	//����IO�ӿ�
#define STEP_MOTOR_B	GPIO_Pin_12	//����IO�ӿ�
#define STEP_MOTOR_C	GPIO_Pin_13	//����IO�ӿ�
#define STEP_MOTOR_D	GPIO_Pin_14	//����IO�ӿ�


void STEP_MOTOR_OFF (void){//����ϵ�
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//���ӿ���0
}

void step_Init(void){
	
	//GPIO Init
	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //APB2����GPIOʱ��ʹ��      
    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D; //ѡ��˿�                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ   �������    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);
	
	
	STEP_MOTOR_OFF();//�رյ��
	
}

//speed�������1000
void STEP_MOTOR_8A (u8 a,u16 speed){//�������8��
	switch (a){
		case 0:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
			break;
		case 1:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
			break;
		case 2:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
			break;
		case 3:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
			break;
		case 4:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
			break;
		case 5:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
			break;
		case 6:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
			break;
		case 7:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
			break;
		default:
			break;
	}
	Delay_us(speed); //��ʱ �����ٶ�
	STEP_MOTOR_OFF(); //����ϵ�״̬�����������
}

//RL���Ʒ��� ��RL=1��ת��RL=0��ת  ��֮������ת�����෴ ʵ������������
//num���Ʋ���  1ȦΪ4096��
//speed�����ٶ�
void STEP_MOTOR_NUM (u8 RL,u16 num,u16 speed){//�������������
	u16 i;
	for(i=0;i<num;i++){	
		if(RL==1){ //��RL=1��ת��RL=0��ת
			STEP++;
			if(STEP>7)STEP=0;
		}else{
			if(STEP==0)STEP=8;
			STEP--;
		}
		STEP_MOTOR_8A(STEP,speed);
	}
}

//����������Ƕ���ת
//angle 0-360��
void STEP_MOTOR_Angle(u8 RL, u32 angle, u16 speed){
	STEP_MOTOR_NUM(RL,angle*512/45,speed);
}

//�����Ȧ������
//RL���Ʒ��� ��RL=1��ת��RL=0��ת ��֮������ת�����෴ ʵ������������
//LOOP ����Ȧ��
//speed�����ٶ�
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u16 speed){
	STEP_MOTOR_NUM(RL,LOOP*4096,speed); 
}



