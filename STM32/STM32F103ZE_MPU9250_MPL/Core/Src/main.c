/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu9250.h"
#include "inv_mpu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t init_flag=0,rec_count=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void mpu9250_Start(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
		mpu9250_Start();
		HAL_TIM_Base_Start_IT(&htim8);
//	  printf("-- Mpu9250 Project Start -- \r\n");
//      uint8_t recv = 0x00;
//      uint8_t i2c_err;
//      i2c_err = HAL_I2C_Mem_Read(&hi2c1, (0x68<<1), 0x75, I2C_MEMADD_SIZE_8BIT, &recv, 1, 0xfff);
//      if(recv == 0x71)
//      {
//          printf("mpu9250 ID Read: OK（0x71 at 0x75)\r\n");
//      }
//      else
//      {
//          printf("Err:%d\r\n",i2c_err);
//      }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		printf("-- Mpu9250 Project Start -- \r\n");
//		HAL_UART_Transmit(&huart1,(uint8_t *)"hello",5,100);
//		HAL_Delay(500);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{			

	float pitch,roll,yaw; 	        //欧拉角
	short aacx,aacy,aacz;	        //加速度传感器原始数据
	short gyrox,gyroy,gyroz;        //陀螺仪原始数据 
    short temp;                     //温度
				 uint8_t err=0;
	if(init_flag){				
		err = mpu_mpl_get_data(&pitch,&roll,&yaw);
        if(err == 0)
        {
                temp = MPU_Get_Temperature();	                                     //得到温度值（扩大了100倍）
                MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	                         //得到加速度传感器数据
                MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	                         //得到陀螺仪数据
                //mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);               //发送加速度+陀螺仪原始数据
                printf(/*"roll = %f   pitch = %f */  "yaw = %f (T = %d)\r\n",/*roll,pitch,*/yaw,temp);
					
                
        }
			}
}


void mpu9250_Start(void)
{
	
	float pitch,roll,yaw; 	        //欧拉角
	short aacx,aacy,aacz;	        //加速度传感器原始数据
	short gyrox,gyroy,gyroz;        //陀螺仪原始数据 
    short temp;                     //温度
    
    uint8_t err = mpu_dmp_init();
    while(err)         
	{   
        printf("mpu_init_err:%d\r\n",err);
		HAL_Delay(200);
        err = mpu_dmp_init();
	}
	init_flag=1;
    printf("mpu9250 Ok\r\n");
//    while(1)
//    {
//        err = mpu_mpl_get_data(&pitch,&roll,&yaw);
//        if(err == 0)
//        {
//                temp = MPU_Get_Temperature();	                                     //得到温度值（扩大了100倍）
//                MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	                         //得到加速度传感器数据
//                MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	                         //得到陀螺仪数据
//                //mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);               //发送加速度+陀螺仪原始数据
//                printf(/*"roll = %f   pitch = %f */  "yaw = %f (T = %d)\r\n",/*roll,pitch,*/yaw,temp);
//					
//                
//        }
//		HAL_Delay(50);
//    }
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
