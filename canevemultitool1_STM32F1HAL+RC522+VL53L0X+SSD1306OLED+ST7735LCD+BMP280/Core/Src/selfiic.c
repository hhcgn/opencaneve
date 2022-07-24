#include "selfiic.h"
void RCCdelay_us(uint32_t udelay)
{
   uint32_t Delay = udelay ;//* //8 / 8;       //72M主频
  do
  {
    __NOP();                                   //空语句
  }
  while (Delay --);
}
void I2C_SDA_Mode(uint8_t addr) //输入参数：1表示配置SDA为输出模式，0表示配置SDA为输入模式
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(addr)                                                   //1 表示 out
	{
		GPIO_InitStruct.Pin = selfsda_Pin;                     //IIC的SDA引脚
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;            //开漏输出模式
		GPIO_InitStruct.Pull = GPIO_PULLUP;                    //上拉
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;           //SDA口速度
		HAL_GPIO_Init(selfsda_GPIO_Port, &GPIO_InitStruct);  //设置SDA口
	}
	else                                                       //0 表示 input
	{
		GPIO_InitStruct.Pin = selfsda_Pin;                      //IIC的SDA引脚
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                 //输入模式
		GPIO_InitStruct.Pull = GPIO_PULLUP;                     //上拉
		HAL_GPIO_Init(selfsda_GPIO_Port, &GPIO_InitStruct);   //设置SDA口
	}
}

void I2C_Start(void)      //IIC起始信号
{
	I2C_SDA_Mode(OUT);    //先配置SDA为输出
	
	RESET_SCL;            //先拉低SCL为下面拉高SDA做准备（在SCL低时变化SDA信号可防止误操作）
	SET_SDA;              //拉高SDA
	SET_SCL;              //拉高SCL
	RCCdelay_us(5);       //延时5us(满足时序中在SCL高期间SDA高电平时间保持4.7us以上)
	RESET_SDA;            //拉低SDA，产生下降沿
	RCCdelay_us(5);       //延时5us(满足时序中在SCL高电平期间SDA低电平保持4us以上）
	RESET_SCL;            //拉低SCL
}
 
void I2C_Stop(void)       //IIC终止信号
{
	I2C_SDA_Mode(OUT);    //先配置SDA为输出
	
	RESET_SCL;            //先拉低SCL为下面拉低SDA做准备（在SCL低时变化SDA信号可防止误操作）
	RESET_SDA;            //拉低SDA
	SET_SCL;              //拉高SCL
	RCCdelay_us(5);       //延时5us(满足时序中在SCL高期间SDA低电平时间保持4us以上)
	SET_SDA;              //拉高SDA
	RCCdelay_us(5);       //延时5us(满足时序中在SCL高电平期间SDA高电平保持4.7us以上）
    RESET_SCL;            //拉低SCL
}



uint8_t I2C_Wait_Ack(void)         //主机等待应答
{
	uint8_t Time_Ack=0;            //定义变量用来标志应答超时
	
	I2C_SDA_Mode(IN);              //主机接收应答信号，此时应该设置SDA为输出模式
	
  RESET_SCL;                     //拉低SCL（因为从机发送应答信号是在SCL低电平期间允许改变）
	RCCdelay_us(4);                //延时4us确保SDA数据完全改变
	SET_SCL;                       //拉高SCL（因为从机发送的信号是在SCL高电平期间被采样接受）              
	RCCdelay_us(4);                //SCL为高期间SDA数据保持不变
	
	while(HAL_GPIO_ReadPin(selfsda_GPIO_Port, selfsda_Pin)) //当读取SDA为高电平时候进入
	{
		if(++Time_Ack > 250)//高电平也就是无应答，一直在这等待，直到状态变量大于250认为超时
		{
			I2C_Stop();return 1; //从机无应答，停止IIC，返回1
		}			
	}
	RESET_SCL;              
	RCCdelay_us(4);             //如果跳出了while循环就是有应答信号，拉低SCL
	
	return 0;                   //有应答返回0
	
}

void I2C_Send_Ack(uint8_t ack)//主机发送是否应答信号 1表示不应答 0表示应答
{
	if(ack)                                   // 1 表示不应答 NoAck
	{
		RESET_SCL;
		RCCdelay_us(5);
		SET_SDA;
		RCCdelay_us(5);
		SET_SCL;                           //SCL拉高，保持4us以上，此时SDA为高，表示不应答
		RCCdelay_us(5);
		RESET_SCL;                         //SCL拉低，表示不应答信号发送结束
		RCCdelay_us(5);
		RESET_SDA;
	}
	else                                      //0 表示应答 Ack
	{
		RESET_SCL;
		RCCdelay_us(5);
		RESET_SDA;
		RCCdelay_us(5);
		SET_SCL;                           //SCL拉高，保持4us以上，此时SDA为低，表示应答
		RCCdelay_us(5);
		RESET_SCL;                         //SCL拉低，表示应答信号发送结束
		RCCdelay_us(5);
		SET_SDA;
	}
}



void I2C_Write_Byte(uint8_t Data) //主机发送一个字节数据
{
	I2C_SDA_Mode(OUT);            //配置SDA为输出模式
	
	RESET_SCL;                    //先拉低SCL，为SDA第一个数据可变化做准备
	RCCdelay_us(1);               //延时1us，确保完全拉低SCL
	
	
	for(uint8_t i=0;i<8;i++)      //循环8此讲Data数据发送出去
	{
		if((Data<<i) & 0x80)     //Data循环次数左移，保证循环从最高位到最低位发送数据
			SET_SDA;
		else
			RESET_SDA;
		
        RCCdelay_us(4);          //延时4us，确保SDA的数据被稳定接收
		SET_SCL;                 //SCL拉高，准备发送数据
		RCCdelay_us(4);          //延时4us，确保SDA的数据被稳定接收
		RESET_SCL;               //SCL拉低，允许SDA数据变化
	}
}

uint8_t I2C_Read_Data(void)      //主机接收一个字节
{
	uint8_t Data;                //定义一个变量接收数据
	
	I2C_SDA_Mode(IN);            //设置SDA为输入模式
	
	RESET_SCL;                   //拉低SCL,允许SDA的数据变化
	RCCdelay_us(4);              //延时4us，确保SDA数据稳定
 
	
	for(uint8_t i=0;i<8;i++)     //循环8次接收一个字节数据
	{
		SET_SCL;                  //拉高SCL，读取SDA数据
		RCCdelay_us(1);           //延时1us确保SCL被完全拉高
		Data = Data<<1;           //将得到的数据每次左移一位，满足第一个数据最终在最高位
		if(HAL_GPIO_ReadPin(selfsda_GPIO_Port, selfsda_Pin))//读SDA的电平
			Data |= 0x01;         //高电平则把最低为置1，低电平则不用操作（因为左移自动补0）
        
		RESET_SCL;                //拉低SCL,允许下一个SDA的数据变化
		RCCdelay_us(4);           //延时4us，确保SDA数据稳定
	}
	return Data;                  //返回读到的一个字节数据
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
////清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
//void OLED_Clear(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
//		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //更新显示
//}
////坐标设置

//	void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WR_Byte(0xb0+y,OLED_CMD);
//	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f),OLED_CMD); 
//}   
////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////mode:0,反白显示;1,正常显示				 
////size:选择字体 16/12 
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//得到偏移后的值			
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
////初始化SSD1306					    
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

