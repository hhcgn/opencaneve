/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"

#include<string.h>
#include<math.h>
#include "flash_read_write.h"
#include "VL53L0X.h"

#include "bmp280.h"

#include "MFRC522.h"

#include <string.h>
#include "st7735.h"
#include "fontslcd.h"
#include "testimg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rec_u1,rec_u2,rec_u3;
int sec=0;
char oled_data1[6],oled_data2[6];
int deg=0,last_deg;
uint32_t ADC_Value1,ADC_Value2;
uint8_t i,click_long=0,deg_sta=0,flash_i=0;//sta--0,1+,2-
uint32_t ad1,ad2;


//bmp data
BMP280_HandleTypedef bmp280;

float pressure, temperature, humidity;

uint16_t bmp_size;
uint8_t bmp_Data[256];



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int flashWriteData(int paramSizeKb, uint64_t data[], size_t len);
void flashReadData(int paramSizeKb, uint8_t *data, size_t len);
int comptodeg(int deg);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void init() {
    ST7735_Init();

    const char ready[] = "Ready!\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)ready, sizeof(ready)-1, HAL_MAX_DELAY);
}

void loop() {
    // Check border
    //ST7735_FillScreen(ST7735_BLACK);

    for(int x = 0; x < ST7735_WIDTH; x++) {
        ST7735_DrawPixel(x, 0, ST7735_RED);
        ST7735_DrawPixel(x, ST7735_HEIGHT-1, ST7735_RED);
    }

    for(int y = 0; y < ST7735_HEIGHT; y++) {
        ST7735_DrawPixel(0, y, ST7735_RED);
        ST7735_DrawPixel(ST7735_WIDTH-1, y, ST7735_RED);
    }

   // HAL_Delay(100);

    // Check fonts
//    ST7735_FillScreen(ST7735_BLACK);
//    ST7735_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ST7735_RED, ST7735_BLACK);
//    ST7735_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, ST7735_GREEN, ST7735_BLACK);
//    ST7735_WriteString(0, 3*10+3*18, "Font_16x26", Font_16x26, ST7735_BLUE, ST7735_BLACK);
//    HAL_Delay(2000);

    // Check colors
 /*   ST7735_FillScreen(ST7735_BLACK);
    ST7735_WriteString(0, 0, "BLACK", Font_11x18, ST7735_WHITE, ST7735_BLACK);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_BLUE);
    ST7735_WriteString(0, 0, "BLUE", Font_11x18, ST7735_BLACK, ST7735_BLUE);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_RED);
    ST7735_WriteString(0, 0, "RED", Font_11x18, ST7735_BLACK, ST7735_RED);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_GREEN);
    ST7735_WriteString(0, 0, "GREEN", Font_11x18, ST7735_BLACK, ST7735_GREEN);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_CYAN);
    ST7735_WriteString(0, 0, "CYAN", Font_11x18, ST7735_BLACK, ST7735_CYAN);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_MAGENTA);
    ST7735_WriteString(0, 0, "MAGENTA", Font_11x18, ST7735_BLACK, ST7735_MAGENTA);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_YELLOW);
    ST7735_WriteString(0, 0, "YELLOW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
    HAL_Delay(500);

    ST7735_FillScreen(ST7735_WHITE);
    ST7735_WriteString(0, 0, "WHITE", Font_11x18, ST7735_BLACK, ST7735_WHITE);
    HAL_Delay(500);
*/
#ifdef ST7735_IS_160X128
    // Display test image 128x128
    ST7735_DrawImage(0, 0, ST7735_WIDTH, ST7735_HEIGHT, (uint16_t*)test_img_128x160);

/*
    // Display test image 128x128 pixel by pixel
    for(int x = 0; x < ST7735_WIDTH; x++) {
        for(int y = 0; y < ST7735_HEIGHT; y++) {
            uint16_t color565 = test_img_128x128[y][x];
            // fix endiness
            color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8);
            ST7735_DrawPixel(x, y, color565);
        }
    }
*/
    
		HAL_Delay(50);
#endif // ST7735_IS_128X128

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,&rec_u1,1);
	HAL_UART_Receive_IT(&huart2,&rec_u2,1);
	HAL_UART_Receive_IT(&huart3,&rec_u3,1);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	if (ssd1306_Init(&hi2c1) != 0) {
    HAL_UART_Transmit(&huart1,(uint8_t *)"error",6,50);
  }
	HAL_Delay(10);

  ssd1306_Fill(Black);
  ssd1306_UpdateScreen(&hi2c1);
	
  HAL_Delay(10);
	//uint64_t writeData[6] = {'h','e','l','l','o'};
//	flashWriteData(31,writeData,5);
//	flashWriteData(32,writeData,5);
	//flashWriteData(31,writeData,6);
	flashReadData(124,flash_data_read,6);
	if(flash_data_read[0]!='\0')
		deg=ctoi(flash_data_read);
  // Write data to local screenbuffer
  ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("ssd1306", Font_7x10, White);
	
	  ssd1306_UpdateScreen(&hi2c1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Value1, 1);
	HAL_ADC_Start(&hadc2);
	
	//vl53l0x
	char msgBuffer[52];
	for (uint8_t i = 0; i < 52; i++) {
		msgBuffer[i] = ' ';
	}

	// Initialise the VL53L0X
	statInfo_t_VL53L0X distanceStr;
	initVL53L0X(1, &hi2c1);

	// Configure the sensor for high accuracy and speed in 20 cm.
	setSignalRateLimit(200);
	setVcselPulsePeriod(VcselPeriodPreRange, 10);
	setVcselPulsePeriod(VcselPeriodFinalRange, 14);
	setMeasurementTimingBudget(300 * 1000UL);

	uint16_t distance;
	
	
	
	//LCD
	
		init();
	
	//bmp
//	bmp280_init_default_params(&bmp280.params);
//	bmp280.addr = BMP280_I2C_ADDRESS_0;
//	bmp280.i2c = &hi2c1;

//	while (!bmp280_init(&bmp280, &bmp280.params)) {
//		bmp_size = sprintf((char *)bmp_Data, "BMP280 initialization failed\n");
//		HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
//		HAL_Delay(2000);
//	}
//	bool bme280p = bmp280.id == BME280_CHIP_ID;
//	bmp_size = sprintf((char *)bmp_Data, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
//	HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
	
	  /*ssd1306_SetCursor(0, 36);
  ssd1306_WriteString("4ilo", Font_11x18, White);

  // Draw rectangle on screen
  for (uint8_t i=0; i<28; i++) {
      for (uint8_t j=0; j<64; j++) {
          ssd1306_DrawPixel(100+i, 0+j, White);
      }
  }*/

  // Copy all data from local screenbuffer to the screen

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_Delay(10);
		
		distance = readRangeSingleMillimeters(&distanceStr);
		
		itoa(deg,oled_data1,10);
		for(int i3=strlen(oled_data1);i3<5;i3++)
		oled_data1[i3]=' ';
		ssd1306_SetCursor(0, 30);
		ssd1306_WriteString(oled_data1, Font_7x10, White);
		itoa(comptodeg(deg),oled_data2,10);
		for(int i3=strlen(oled_data2);i3<5;i3++)
		oled_data2[i3]=' ';
		ssd1306_SetCursor(50, 30);
		ssd1306_WriteString(oled_data2, Font_7x10, White);
		ssd1306_SetCursor(80, 25);
		ssd1306_WriteString("o", Font_7x10, White);
		ssd1306_SetCursor(0, 50);
		ssd1306_WriteString(flash_data_read, Font_7x10, White);

		ssd1306_SetCursor(50, 50);
		
		loop();
		/*while (!bmp280_read_float(&bmp280, &temperature, &pressure, &humidity)) {
			bmp_size = sprintf((char *)bmp_Data,
					"Temperature/pressure reading failed\n");
			HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
			HAL_Delay(2000);
		}

		bmp_size = sprintf((char *)bmp_Data,"Pressure: %.2f Pa, Temperature: %.2f C",
				pressure, temperature);
		HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
		if (bme280p) {
			bmp_size = sprintf((char *)bmp_Data,", Humidity: %.2f\n", humidity);
			HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
		}

		else {
			bmp_size = sprintf((char *)bmp_Data, "\n");
			HAL_UART_Transmit(&huart1, bmp_Data, bmp_size, 1000);
		}*/
		
		itoa(distance,msgBuffer,10);
		for(int i3=strlen(msgBuffer);i3<6;i3++)
			msgBuffer[i3]=' ';
		//sprintf(msgBuffer,"mm", distance);
		ssd1306_WriteString(msgBuffer, Font_7x10, White);
		ssd1306_SetCursor(90, 50);
		ssd1306_WriteString("mm", Font_7x10, White);
		ssd1306_UpdateScreen(&hi2c1);
		
		
		/*itoa(ad1,oled_data1,10);
		itoa(ad2,oled_data2,10);
		for(int i3=strlen(oled_data1);i3<5;i3++)
		oled_data1[i3]=' ';
		for(int i3=strlen(oled_data2);i3<5;i3++)
		oled_data2[i3]=' ';
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		HAL_Delay(200);
//		HAL_UART_Transmit(&huart1,(uint8_t *)"a",1,50);
		ssd1306_SetCursor(0, 30);
		ssd1306_WriteString(oled_data1, Font_7x10, Black);
		ssd1306_WriteString("mV", Font_7x10, Black);
		ssd1306_UpdateScreen(&hi2c1);
		ssd1306_SetCursor(0, 42);
		ssd1306_WriteString(oled_data2, Font_7x10, Black);
		ssd1306_WriteString("mV", Font_7x10, Black);
		ssd1306_UpdateScreen(&hi2c1);
		sec++;
		
		ad1 = hadc1.Instance->DR;
		ad2 = hadc2.Instance->DR;
					
				//ad2 /= 50;
				//printf("\r\n********ADC-DMA-Example********\r\n");
				//printf("%1.3fV\r\n", ad1*3.3f/4096);
		ad1=(ad1*3.3f/4096)*1000;
		ad2=(ad2*3.3f/4096)*1000;*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int comptodeg(int deg){
	//0.5ms-2.5ms 2000: 50->250
	int ass=50;
	if(deg>250) deg=180;
	else if(deg<50) deg=0;
	else {
		deg=((deg-50)/200.0)*180;
	}
	return deg;
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)) {
			click_long=0;
			deg_sta=0;
		}
		else click_long+=1;
		if(click_long>15) deg+=(deg_sta==1? 1:(deg_sta==2 ? (-1):0));
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,deg);
  HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}

int htoi(char a[])
{
    int n = strlen(a);//判断字符串的长度函数
    int i, j = 0, num = 0;
    for (i = 0; i < n; i++) 
    {
        if (a[i] == 'A')
            num += 10 * pow(16, n - i - 1); //pow() 函数用来求 x 的 y 次方的值。
        else if (a[i] == 'B')
            num += 11 * pow(16, n - i - 1);
        else if (a[i] == 'C')
            num += 12 * pow(16, n - i - 1);
        else if (a[i]=='D')
            num += 13 * pow(16, n - i - 1);
        else if (a[i] == 'E')
            num += 14 * pow(16, n - i - 1);
        else
            num += 15 * pow(16, n - i - 1);
    }
    return num;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t key=' ';
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(80);
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==GPIO_PIN_RESET)
		{
			key='u';
			deg++;
			deg_sta=1;
		}
		else if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==GPIO_PIN_RESET))
		{
			key='r';
			deg_sta=0;
			deg=0;
			
		}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==GPIO_PIN_RESET)
		{
			key='d';
			deg--;
			deg_sta=2;
		}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==GPIO_PIN_RESET)
		{
			key='l';
			deg_sta=0;
		}else deg_sta=0;
		if(key!=' ')
		HAL_UART_Transmit(&huart1,&key,1,50);
		click_long+=1;
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,deg);
		last_deg=deg;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart==&huart1){
		HAL_UART_Transmit(&huart1,(uint8_t *)"uart1",6,50);
		HAL_UART_Transmit(&huart1,&rec_u1,1,50);
		HAL_UART_Receive_IT(&huart1,&rec_u1,1);
	}
	else if(huart==&huart2){
		HAL_UART_Transmit(&huart2,(uint8_t *)"uart2",6,50);
		HAL_UART_Transmit(&huart2,&rec_u2,1,50);
		HAL_UART_Receive_IT(&huart2,&rec_u2,1);
	}
	else if(huart==&huart3){
		HAL_UART_Transmit(&huart3,(uint8_t *)"uart3",6,50);
		HAL_UART_Transmit(&huart3,&rec_u3,1,50);
		HAL_UART_Receive_IT(&huart3,&rec_u3,1);
	}
	
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

