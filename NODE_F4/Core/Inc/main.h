/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOC
#define Relay_1_Pin GPIO_PIN_0
#define Relay_1_GPIO_Port GPIOA
#define Relay_2_Pin GPIO_PIN_1
#define Relay_2_GPIO_Port GPIOA
#define RS485_DERE_Pin GPIO_PIN_4
#define RS485_DERE_GPIO_Port GPIOA
#define LORA_NSS_Pin GPIO_PIN_0
#define LORA_NSS_GPIO_Port GPIOB
#define W25Q_CS_Pin GPIO_PIN_1
#define W25Q_CS_GPIO_Port GPIOB
#define LORA_RST_Pin GPIO_PIN_2
#define LORA_RST_GPIO_Port GPIOB
#define LORA_INT_Pin GPIO_PIN_10
#define LORA_INT_GPIO_Port GPIOB
#define LORA_INT_EXTI_IRQn EXTI15_10_IRQn
#define TFT_DB7_Pin GPIO_PIN_12
#define TFT_DB7_GPIO_Port GPIOB
#define TFT_DB6_Pin GPIO_PIN_13
#define TFT_DB6_GPIO_Port GPIOB
#define TFT_DB5_Pin GPIO_PIN_14
#define TFT_DB5_GPIO_Port GPIOB
#define TFT_DB4_Pin GPIO_PIN_15
#define TFT_DB4_GPIO_Port GPIOB
#define TFT_DB3_Pin GPIO_PIN_8
#define TFT_DB3_GPIO_Port GPIOA
#define TFT_DB2_Pin GPIO_PIN_9
#define TFT_DB2_GPIO_Port GPIOA
#define TFT_DB1_Pin GPIO_PIN_10
#define TFT_DB1_GPIO_Port GPIOA
#define TFT_DB0_Pin GPIO_PIN_11
#define TFT_DB0_GPIO_Port GPIOA
#define TFT_LED_Pin GPIO_PIN_12
#define TFT_LED_GPIO_Port GPIOA
#define TFT_RST_Pin GPIO_PIN_15
#define TFT_RST_GPIO_Port GPIOA
#define TFT_RD_Pin GPIO_PIN_3
#define TFT_RD_GPIO_Port GPIOB
#define TFT_WR_Pin GPIO_PIN_4
#define TFT_WR_GPIO_Port GPIOB
#define TFT_RS_Pin GPIO_PIN_5
#define TFT_RS_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_6
#define TFT_CS_GPIO_Port GPIOB
#define PCF_INT_Pin GPIO_PIN_7
#define PCF_INT_GPIO_Port GPIOB
#define PCF_INT_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
