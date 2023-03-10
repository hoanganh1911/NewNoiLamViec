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
#include "stdbool.h"
#include "lcd_screw.h"
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Khai báo các biến toàn cục */
/* Quay thuận */
volatile bool turnedCW = false;
volatile bool checkturnedCW = false;

/* Quay nghịch */
volatile bool turnedCCW = false;
volatile bool checkturnedCCW = false;

/* Bấm giữ nút nhất */
volatile bool pressed_BTN_EN = false;
/* Biến đếm */
static int16_t screw_set_count = 0;

/* Sử dụng delay trong ngắt timer 2*/
void Delay_us(uint16_t t)
{
	__HAL_TIM_SET_COUNTER(&htim2,0);
	while(__HAL_TIM_GET_COUNTER(&htim2)<t);
}
void Delay_ms(uint32_t t)
{
	for(uint32_t i = 0; i<1000;i++)
		Delay_us(t);
}
/* Thực hiện ngắt */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	__disable_irq();
	Delay_ms(1);

	// Xác định trạng thái đếm lên
	if(GPIO_Pin == CLK_EN_Pin && checkturnedCW == false)
	{
		if(HAL_GPIO_ReadPin(DT_EN_GPIO_Port, DT_EN_Pin	) == 1 && HAL_GPIO_ReadPin(CLK_EN_GPIO_Port, CLK_EN_Pin) == 0)
			checkturnedCW = true;
	}
	if(GPIO_Pin == DT_EN_Pin && checkturnedCW == true)
	{
		if(HAL_GPIO_ReadPin(DT_EN_GPIO_Port, DT_EN_Pin) == 0 && HAL_GPIO_ReadPin(CLK_EN_GPIO_Port, CLK_EN_Pin) == 0)
		{
			turnedCW = true;
			checkturnedCW = false;
		}
	}

	// Xác định trạng thái đễm xuống
	if(GPIO_Pin == DT_EN_Pin && checkturnedCCW == false)
	{
		if(HAL_GPIO_ReadPin(DT_EN_GPIO_Port, DT_EN_Pin) == 0 && HAL_GPIO_ReadPin(CLK_EN_GPIO_Port, CLK_EN_Pin) == 1)
			checkturnedCCW = true;
	}
	if(GPIO_Pin == CLK_EN_Pin && checkturnedCCW == true)
	{
		if(HAL_GPIO_ReadPin(DT_EN_GPIO_Port, DT_EN_Pin) == 0 && HAL_GPIO_ReadPin(CLK_EN_GPIO_Port, CLK_EN_Pin) == 0)
		{
			turnedCCW = true;
			checkturnedCCW = false;
		}
	}

	if(GPIO_Pin == BTN_EN_Pin && pressed_BTN_EN == false)
	{
		if(HAL_GPIO_ReadPin(BTN_EN_GPIO_Port, BTN_EN_Pin) == 0)
		{
			int i = 0;
			for(i = 0;i<200;i++)
			{
				if(HAL_GPIO_ReadPin(BTN_EN_GPIO_Port, BTN_EN_Pin) == 1)
					break;
				Delay_ms(1);
			}
			if(i==200)
				pressed_BTN_EN = true;
		}
	}
	__enable_irq();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	hc595_t conf={
			.CLK_Port = Clock_GPIO_Port,
			.CLK_Pin = Clock_Pin,
			.LATCH_Port = Latch_GPIO_Port,
			.LATCH_Pin = Latch_Pin,
			.DATA_Port = Data_GPIO_Port,
			.DATA_Pin = Data_Pin
	};
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  /* Khởi động timer 2*/
  HAL_TIM_Base_Start(&htim2);
  hc595_int(&conf);
  lcd_Init();
  screw_Set_Show(screw_set_count);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(turnedCW == true)
	  {
		screw_set_count++;
		lcd_Clear();
		screw_Set_Show(screw_set_count);
		turnedCW = false;
	  }
	  if(turnedCCW == true)
	  {
		screw_set_count--;
		if(screw_set_count > -1)
		{
			lcd_Clear();
			screw_Set_Show(screw_set_count);
		}
		else
			screw_set_count = 0;
		turnedCCW = false;
	  }
	  if(pressed_BTN_EN == true)
	  {
		  screw_set_count = 0;
		  lcd_Clear();
		  screw_Set_Show(screw_set_count);
		  pressed_BTN_EN = false;
	  }

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Clock_Pin|Latch_Pin|Data_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CLK_EN_Pin BTN_EN_Pin */
  GPIO_InitStruct.Pin = CLK_EN_Pin|BTN_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DT_EN_Pin */
  GPIO_InitStruct.Pin = DT_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DT_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Clock_Pin Latch_Pin Data_Pin */
  GPIO_InitStruct.Pin = Clock_Pin|Latch_Pin|Data_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
