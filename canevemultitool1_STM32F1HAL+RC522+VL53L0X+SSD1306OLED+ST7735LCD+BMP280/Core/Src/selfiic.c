#include "selfiic.h"
void RCCdelay_us(uint32_t udelay)
{
   uint32_t Delay = udelay ;//* //8 / 8;       //72M��Ƶ
  do
  {
    __NOP();                                   //�����
  }
  while (Delay --);
}
void I2C_SDA_Mode(uint8_t addr) //���������1��ʾ����SDAΪ���ģʽ��0��ʾ����SDAΪ����ģʽ
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(addr)                                                   //1 ��ʾ out
	{
		GPIO_InitStruct.Pin = selfsda_Pin;                     //IIC��SDA����
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;            //��©���ģʽ
		GPIO_InitStruct.Pull = GPIO_PULLUP;                    //����
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;           //SDA���ٶ�
		HAL_GPIO_Init(selfsda_GPIO_Port, &GPIO_InitStruct);  //����SDA��
	}
	else                                                       //0 ��ʾ input
	{
		GPIO_InitStruct.Pin = selfsda_Pin;                      //IIC��SDA����
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                 //����ģʽ
		GPIO_InitStruct.Pull = GPIO_PULLUP;                     //����
		HAL_GPIO_Init(selfsda_GPIO_Port, &GPIO_InitStruct);   //����SDA��
	}
}

void I2C_Start(void)      //IIC��ʼ�ź�
{
	I2C_SDA_Mode(OUT);    //������SDAΪ���
	
	RESET_SCL;            //������SCLΪ��������SDA��׼������SCL��ʱ�仯SDA�źſɷ�ֹ�������
	SET_SDA;              //����SDA
	SET_SCL;              //����SCL
	RCCdelay_us(5);       //��ʱ5us(����ʱ������SCL���ڼ�SDA�ߵ�ƽʱ�䱣��4.7us����)
	RESET_SDA;            //����SDA�������½���
	RCCdelay_us(5);       //��ʱ5us(����ʱ������SCL�ߵ�ƽ�ڼ�SDA�͵�ƽ����4us���ϣ�
	RESET_SCL;            //����SCL
}
 
void I2C_Stop(void)       //IIC��ֹ�ź�
{
	I2C_SDA_Mode(OUT);    //������SDAΪ���
	
	RESET_SCL;            //������SCLΪ��������SDA��׼������SCL��ʱ�仯SDA�źſɷ�ֹ�������
	RESET_SDA;            //����SDA
	SET_SCL;              //����SCL
	RCCdelay_us(5);       //��ʱ5us(����ʱ������SCL���ڼ�SDA�͵�ƽʱ�䱣��4us����)
	SET_SDA;              //����SDA
	RCCdelay_us(5);       //��ʱ5us(����ʱ������SCL�ߵ�ƽ�ڼ�SDA�ߵ�ƽ����4.7us���ϣ�
    RESET_SCL;            //����SCL
}



uint8_t I2C_Wait_Ack(void)         //�����ȴ�Ӧ��
{
	uint8_t Time_Ack=0;            //�������������־Ӧ��ʱ
	
	I2C_SDA_Mode(IN);              //��������Ӧ���źţ���ʱӦ������SDAΪ���ģʽ
	
  RESET_SCL;                     //����SCL����Ϊ�ӻ�����Ӧ���ź�����SCL�͵�ƽ�ڼ�����ı䣩
	RCCdelay_us(4);                //��ʱ4usȷ��SDA������ȫ�ı�
	SET_SCL;                       //����SCL����Ϊ�ӻ����͵��ź�����SCL�ߵ�ƽ�ڼ䱻�������ܣ�              
	RCCdelay_us(4);                //SCLΪ���ڼ�SDA���ݱ��ֲ���
	
	while(HAL_GPIO_ReadPin(selfsda_GPIO_Port, selfsda_Pin)) //����ȡSDAΪ�ߵ�ƽʱ�����
	{
		if(++Time_Ack > 250)//�ߵ�ƽҲ������Ӧ��һֱ����ȴ���ֱ��״̬��������250��Ϊ��ʱ
		{
			I2C_Stop();return 1; //�ӻ���Ӧ��ֹͣIIC������1
		}			
	}
	RESET_SCL;              
	RCCdelay_us(4);             //���������whileѭ��������Ӧ���źţ�����SCL
	
	return 0;                   //��Ӧ�𷵻�0
	
}

void I2C_Send_Ack(uint8_t ack)//���������Ƿ�Ӧ���ź� 1��ʾ��Ӧ�� 0��ʾӦ��
{
	if(ack)                                   // 1 ��ʾ��Ӧ�� NoAck
	{
		RESET_SCL;
		RCCdelay_us(5);
		SET_SDA;
		RCCdelay_us(5);
		SET_SCL;                           //SCL���ߣ�����4us���ϣ���ʱSDAΪ�ߣ���ʾ��Ӧ��
		RCCdelay_us(5);
		RESET_SCL;                         //SCL���ͣ���ʾ��Ӧ���źŷ��ͽ���
		RCCdelay_us(5);
		RESET_SDA;
	}
	else                                      //0 ��ʾӦ�� Ack
	{
		RESET_SCL;
		RCCdelay_us(5);
		RESET_SDA;
		RCCdelay_us(5);
		SET_SCL;                           //SCL���ߣ�����4us���ϣ���ʱSDAΪ�ͣ���ʾӦ��
		RCCdelay_us(5);
		RESET_SCL;                         //SCL���ͣ���ʾӦ���źŷ��ͽ���
		RCCdelay_us(5);
		SET_SDA;
	}
}



void I2C_Write_Byte(uint8_t Data) //��������һ���ֽ�����
{
	I2C_SDA_Mode(OUT);            //����SDAΪ���ģʽ
	
	RESET_SCL;                    //������SCL��ΪSDA��һ�����ݿɱ仯��׼��
	RCCdelay_us(1);               //��ʱ1us��ȷ����ȫ����SCL
	
	
	for(uint8_t i=0;i<8;i++)      //ѭ��8�˽�Data���ݷ��ͳ�ȥ
	{
		if((Data<<i) & 0x80)     //Dataѭ���������ƣ���֤ѭ�������λ�����λ��������
			SET_SDA;
		else
			RESET_SDA;
		
        RCCdelay_us(4);          //��ʱ4us��ȷ��SDA�����ݱ��ȶ�����
		SET_SCL;                 //SCL���ߣ�׼����������
		RCCdelay_us(4);          //��ʱ4us��ȷ��SDA�����ݱ��ȶ�����
		RESET_SCL;               //SCL���ͣ�����SDA���ݱ仯
	}
}

uint8_t I2C_Read_Data(void)      //��������һ���ֽ�
{
	uint8_t Data;                //����һ��������������
	
	I2C_SDA_Mode(IN);            //����SDAΪ����ģʽ
	
	RESET_SCL;                   //����SCL,����SDA�����ݱ仯
	RCCdelay_us(4);              //��ʱ4us��ȷ��SDA�����ȶ�
 
	
	for(uint8_t i=0;i<8;i++)     //ѭ��8�ν���һ���ֽ�����
	{
		SET_SCL;                  //����SCL����ȡSDA����
		RCCdelay_us(1);           //��ʱ1usȷ��SCL����ȫ����
		Data = Data<<1;           //���õ�������ÿ������һλ�������һ���������������λ
		if(HAL_GPIO_ReadPin(selfsda_GPIO_Port, selfsda_Pin))//��SDA�ĵ�ƽ
			Data |= 0x01;         //�ߵ�ƽ������Ϊ��1���͵�ƽ���ò�������Ϊ�����Զ���0��
        
		RESET_SCL;                //����SCL,������һ��SDA�����ݱ仯
		RCCdelay_us(4);           //��ʱ4us��ȷ��SDA�����ȶ�
	}
	return Data;                  //���ض�����һ���ֽ�����
}
/**********************************************
// IIC Write Command
**********************************************/
void SELF_Write_IIC_Command(unsigned char IIC_Command)
{
		I2C_Start();
		I2C_Write_Byte(0x78);            //Slave address,SA0=0
		I2C_Wait_Ack();	
		I2C_Write_Byte(0x00);			//write command
		I2C_Wait_Ack();	
		I2C_Write_Byte(IIC_Command); 
		I2C_Wait_Ack();	
		I2C_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void SELF_Write_IIC_Data(unsigned char IIC_Data)
{
		I2C_Start();
		I2C_Write_Byte(0x78);			//D/C#=0; R/W#=0
		I2C_Wait_Ack();	
		I2C_Write_Byte(0x40);			//write data
		I2C_Wait_Ack();	
		I2C_Write_Byte(IIC_Data);
		I2C_Wait_Ack();	
		I2C_Stop();
}
//void OLED_WR_Byte(unsigned dat,unsigned cmd)
//{
//	if(cmd)
//	{
//   SELF_Write_IIC_Data(dat);
//	}
//	else
//	{
//   SELF_Write_IIC_Command(dat);
//	}
//}
////��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
//void OLED_Clear(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //������ʾ
//}
////��������

//	void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WR_Byte(0xb0+y,OLED_CMD);
//	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f),OLED_CMD); 
//}   
////��ָ��λ����ʾһ���ַ�,���������ַ�
////x:0~127
////y:0~63
////mode:0,������ʾ;1,������ʾ				 
////size:ѡ������ 16/12 
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
//		if(x>Max_Column-1){x=0;y=y+2;}
//		if(Char_Size ==16)
//			{
//			OLED_Set_Pos(x,y);	
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16s[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16s[c*16+i+8],OLED_DATA);
//			}
//			else {	
//				OLED_Set_Pos(x,y);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8s[c][i],OLED_DATA);
//				
//			}
//}
////��ʼ��SSD1306					    
//void OLED_Init(void)
//{
// 	
//	
//	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
//	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
//	OLED_WR_Byte(0x81,OLED_CMD); // contract control
//	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
//	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
//	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
//	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
//	OLED_WR_Byte(0x00,OLED_CMD);//
//	
//	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
//	OLED_WR_Byte(0x80,OLED_CMD);//
//	
//	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
//	OLED_WR_Byte(0x05,OLED_CMD);//
//	
//	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
//	OLED_WR_Byte(0xF1,OLED_CMD);//
//	
//	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
//	OLED_WR_Byte(0x12,OLED_CMD);//
//	
//	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
//	OLED_WR_Byte(0x30,OLED_CMD);//
//	
//	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
//	OLED_WR_Byte(0x14,OLED_CMD);//
//	
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//}  

