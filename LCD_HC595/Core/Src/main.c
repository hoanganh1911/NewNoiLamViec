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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void hc595_trans(uint8_t c)
{
	for(int i = 0;i<8;i++)
	{
		uint8_t x = c & (0x01<<(7-i));

		if(x)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
		else
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
}
void lcd_Send_Cmd(char cmd) //Mỗi lần gửi sẽ gửi 4 bit cao xong rồi đến 4 bit thấp
{
	char data_u, data_l; // vi du 0x30
	uint8_t data_t[4];
	data_u = (cmd & 0xf0); // data_u = 	0x30
	data_l = ((cmd << 4) & 0xf0); // data_l = 0x00
	data_t[0] = data_u | 0x0C; // 0000 1100 | 0011 0000 = 0011 1100 = 0x3C // C và 8 thay đổi bit thứ 6 0 1 nhằm EN
	data_t[1] = data_u | 0x08; // 0000 1000 | 0011 0000 = 0011 1000 = 0x38
	data_t[2] = data_l | 0x0C; // 0000 1100 | 0000 0000 = 0000 1100 = 0x0C
	data_t[3] = data_l | 0x08; // 0000 1000 | 0000 0000 = 0000 1000 = 0x08

	for(int i = 0;i<4;i++)
	{
		hc595_trans(data_t[i]);
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
	}
	//HAL_I2C_Master_Transmit(&hi2c1, 0x27 << 1,(uint8_t *)data_t, 4, 100);
}
void lcd_Send_Data(char data)
{
	char data_u,data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D; // 1101
	data_t[1] = data_u | 0x09; // 1001
	data_t[2] = data_l | 0x0D;
	data_t[3] = data_l | 0x09;


	for(int i = 0;i<4;i++)
	{
		hc595_trans(data_t[i]);

	}

	//HAL_I2C_Master_Transmit(&hi2c1, 0x27 << 1,(uint8_t *)data_t, 4, 100);
}
void lcd_Clear(void)
{
	lcd_Send_Cmd(0x80);
	for(int i = 0; i < 100; i++)
	{
		lcd_Send_Data(' ');
	}
}
void lcd_Put_Cur(int row,int col)
{
	switch(row)
	{
		case 0:
			col |= 0x80; //0x80 1 000 000x
			break;
		case 1:
			col |= 0xC0; // 0xC1 1 100 000x
			break;
	}
	//Gửi lệnh ứng với vị trí cần xuất hiện con trỏ Set DDRAM address tr24
	lcd_Send_Cmd(col);
}
void lcd_Init(void)
{
	// Khởi tạo 4 bit
	HAL_Delay(50);
	lcd_Send_Cmd(0x30);
	HAL_Delay(5);
	lcd_Send_Cmd(0x30);
	HAL_Delay(1);
	lcd_Send_Cmd(0x30);
	HAL_Delay(10);
	lcd_Send_Cmd(0x20);
	HAL_Delay(10);
	// Khởi tạo hiển thị
	//00 00 101 00
	lcd_Send_Cmd(0x28); //function set bảng 6/tr23: set data interface 4bits 2lines 5x8 bit
	HAL_Delay(1);
	lcd_Send_Cmd(0x08); //display on/off control
	HAL_Delay(1);
	lcd_Send_Cmd(0x01); //clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_Send_Cmd(0x06); //entry mode set: i/d=1 increment s=0
	HAL_Delay(1);
	lcd_Send_Cmd(0x0C); // cho phep hien thi man hinh
}
void lcd_Send_String(char *str)
{
	while(*str) lcd_Send_Data(*str++);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
  lcd_Init();
  lcd_Put_Cur(0, 0);
  lcd_Send_String("DCMHC595");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CLK_Pin|LATCH_Pin|DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(M_RESET_GPIO_Port, M_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : CLK_Pin DATA_Pin */
  GPIO_InitStruct.Pin = CLK_Pin|DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LATCH_Pin */
  GPIO_InitStruct.Pin = LATCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LATCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : M_RESET_Pin */
  GPIO_InitStruct.Pin = M_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(M_RESET_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
