
#ifndef _SELFIIC_H_
#define _SELFIIC_H_
 
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "fonts.h"
/*����SDA�����뻹�����*/
#define OUT 1
#define IN 0                                      
 
/*����Ӧ���ǲ�Ӧ��*/
#define Ack 0
#define NoAck 1           

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
 
/*��IIC��SDA��SCL�����Լ��趨��IO�ڶ�Ӧ*/
#define PORT_SCL IIC_SCL_GPIO_Port
#define PIN_SCL  IIC_SCL_Pin
 
#define PORT_SDA IIC_SDA_GPIO_Port
#define PIN_SDA  IIC_SDA_Pin
 
 
#define SET_SCL     HAL_GPIO_WritePin(selfscl_GPIO_Port, selfscl_Pin, GPIO_PIN_SET)
#define RESET_SCL   HAL_GPIO_WritePin(selfscl_GPIO_Port, selfscl_Pin, GPIO_PIN_RESET)
 
#define SET_SDA     HAL_GPIO_WritePin(selfsda_GPIO_Port, selfsda_Pin, GPIO_PIN_SET)
#define RESET_SDA   HAL_GPIO_WritePin(selfsda_GPIO_Port, selfsda_Pin, GPIO_PIN_RESET)
 
#define Max_Column	128
#define Max_Row		64
 
/*��������*/
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Send_Ack(uint8_t ack);
void I2C_Write_Byte(uint8_t Data);
uint8_t I2C_Read_Data(void);
void I2C_SDA_Mode(uint8_t addr);       
//void OLED_Init(void);
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
//void OLED_Set_Pos(unsigned char x, unsigned char y) ; 
//void OLED_Clear(void);
#endif
