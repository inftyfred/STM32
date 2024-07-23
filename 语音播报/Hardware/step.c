#include "stm32f10x.h"                  // Device header
#include "Delay.h"

u8 STEP;//步数

//电机的速度控制是通过延时来完成的 speed实际为延时的时间 单位为us  -> 每 speed us 转一拍 一圈为4096拍
//理论上 speed 越小转的速度越快 但是要注意不可以太小 否则电机旋转角度会有很大误差甚至不转
//经过测试 speed >= 943 时 可以正常旋转 但一般建议超过1000
//-> 若想要修改速度的逻辑 可在STEP_MOTOR_8A函数中修改
//步进角度 5.625/64 即一拍转的角度 
//满足关系4096 * 步进角度 = 360度


//接线
//除此之外 步进电机还应该接5v和地
#define STEP_MOTOR_PORT	GPIOB	//定义IO接口所在组
#define STEP_MOTOR_A	GPIO_Pin_11	//定义IO接口
#define STEP_MOTOR_B	GPIO_Pin_12	//定义IO接口
#define STEP_MOTOR_C	GPIO_Pin_13	//定义IO接口
#define STEP_MOTOR_D	GPIO_Pin_14	//定义IO接口


void STEP_MOTOR_OFF (void){//电机断电
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//各接口置0
}

void step_Init(void){
	
	//GPIO Init
	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //APB2外设GPIO时钟使能      
    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D; //选择端口                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式   推挽输出    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);
	
	
	STEP_MOTOR_OFF();//关闭电机
	
}

//speed建议大于1000
void STEP_MOTOR_8A (u8 a,u16 speed){//电机单步8拍
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
	Delay_us(speed); //延时 控制速度
	STEP_MOTOR_OFF(); //进入断电状态，防电机过热
}

//RL控制方向 当RL=1右转，RL=0左转  总之两者旋转方向相反 实际情况具体分析
//num控制步数  1圈为4096步
//speed控制速度
void STEP_MOTOR_NUM (u8 RL,u16 num,u16 speed){//电机按步数运行
	u16 i;
	for(i=0;i<num;i++){	
		if(RL==1){ //当RL=1右转，RL=0左转
			STEP++;
			if(STEP>7)STEP=0;
		}else{
			if(STEP==0)STEP=8;
			STEP--;
		}
		STEP_MOTOR_8A(STEP,speed);
	}
}

//步进电机按角度旋转
//angle 0-360度
void STEP_MOTOR_Angle(u8 RL, u32 angle, u16 speed){
	STEP_MOTOR_NUM(RL,angle*512/45,speed);
}

//电机按圈数运行
//RL控制方向 当RL=1右转，RL=0左转 总之两者旋转方向相反 实际情况具体分析
//LOOP 控制圈数
//speed控制速度
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u16 speed){
	STEP_MOTOR_NUM(RL,LOOP*4096,speed); 
}



