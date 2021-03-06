uint8_t Hzk[][16]={

/*--  文字:  你  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=16x16   --*/
{0x00,0x80,0x60,0xF8,0x07,0x40,0x20,0x18,0x0F,0x08,0xC8,0x08,0x08,0x28,0x18,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x10,0x0C,0x03,0x40,0x80,0x7F,0x00,0x01,0x06,0x18,0x00},//1

/*--  文字:  好  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=16x16   --*/
{0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x80,0x82,0x82,0xE2,0x92,0x8A,0x86,0x80,0x00},
{0x40,0x22,0x15,0x08,0x16,0x61,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00},//2
/*--  文字:  测  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
{0x10,0x60,0x02,0x8C,0x00,0xFE,0x02,0xF2,0x02,0xFE,0x00,0xF8,0x00,0xFF,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x80,0x47,0x30,0x0F,0x10,0x27,0x00,0x47,0x80,0x7F,0x00,0x00},//3

/*--  文字:  试  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
{0x40,0x40,0x42,0xCC,0x00,0x90,0x90,0x90,0x90,0x90,0xFF,0x10,0x11,0x16,0x10,0x00},
{0x00,0x00,0x00,0x3F,0x10,0x28,0x60,0x3F,0x10,0x10,0x01,0x0E,0x30,0x40,0xF0,0x00},//4

/*--  文字:  中  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
{0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},//5

};


static uint8_t ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t command)
{
    return HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);
}
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{ //xy为坐标，no为汉字在数组中的位置   (使用的16x16的字体大小)  			    
	u8 t,adder=0;
	uint8_t i;

  ssd1306_WriteCommand(&hi2c1, 0xB0+y );//设置y
  ssd1306_WriteCommand(&hi2c1, 0x00);//设置x偏移
  ssd1306_WriteCommand(&hi2c1, 0x10+x);//设置x

  if(HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, Hzk[2*no], 16, 100)!=HAL_OK)  {
		return;
		}
  ssd1306_WriteCommand(&hi2c1, 0xB0 +y+ 1);
  ssd1306_WriteCommand(&hi2c1, 0x00);
  ssd1306_WriteCommand(&hi2c1, 0x10+x);
					
if(HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, Hzk[2*no+1], 16, 100)!=HAL_OK)  {
		return;
				}
			
}
