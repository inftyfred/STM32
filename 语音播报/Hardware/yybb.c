#include "stm32f10x.h"                  // Device header
#include "string.h"

/**************оƬ��������*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����

//USART��tx��rx���� ����ѡ����USART1��
#define USART_TX GPIO_Pin_9
#define USART_RX GPIO_Pin_10

void MyUsart_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = USART_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = USART_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//���ò�����	��ʼ���������Զ������Ƶֵ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������Ƶ�ѡ�� ���ﲻʹ������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;//ѡ��ģʽ ����ֻʹ�÷���ģʽ  ��Ҫ��ʹ�� ��ʹ�� | ����
	USART_InitStructure.USART_Parity = USART_Parity_No;//ѡ����У���żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ���� ��ѡ 0.5 1 1.5 2
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ���ȣ�����У��λ�� ��ѡ8 ���� 9
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
}


//���ڷ���һ���ֽ�
void Serial_SendByte(uint8_t Byte){
	while((USART1->SR & 0X40) == 0);
	USART1->DR = Byte;
}

//����һ���ַ���
void Serial_SendString(u8 *str, u8 len){
	u8 i;
	for(i = 0; i < len; i++)
	{
		Serial_SendByte(*str++);
	}
}


//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************��Ҫ���͵��ı�**********************************/
  unsigned char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//����У���ֽ�
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

  /*****************֡�̶�������Ϣ**************************************/
  Frame_Info[0] = 0xFD ; 			//����֡ͷFD
  Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
  Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
  Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
  Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨

  /*******************У�������***************************************/
  for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
  {
    ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
  }

  for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
  {
    ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
  }
  /*******************����֡��Ϣ***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  Serial_SendString(Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* ��    �ƣ� YS_SYN_Set(u8 *Info_data)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ����
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  Serial_SendString(Info_data, Com_Len);
}


/* �ı����Ʊ���б� 
����ǰ���ı���������������ʾ��Ч�� ȫ�� [v?] ?Ϊ����ֵ ȡֵ0-16 0 Ϊ����  Ĭ��[v10]
���ñ����������� ȫ�� [m?] ? Ϊ����ֵ��ȡֵ0-16  0Ϊ����  Ĭ��[m4]
���ô�������  ȫ�� [t?]  ? Ϊ��������ֵ��ȡֵ0-5  Ĭ��[t4]
�������ִ������  ȫ�� [n?] ? Ϊ0���Զ��ж� Ϊ1 ���������봦�� Ϊ2 ��������ֵ����  Ĭ��[n0]
*/