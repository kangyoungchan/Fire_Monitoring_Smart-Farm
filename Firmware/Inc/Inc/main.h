/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define YL_69_PIN_Pin GPIO_PIN_0
#define YL_69_PIN_GPIO_Port GPIOA
#define MQ2_PIN_Pin GPIO_PIN_1
#define MQ2_PIN_GPIO_Port GPIOA
#define DISPLAY_CHANGE_PIN_Pin GPIO_PIN_4
#define DISPLAY_CHANGE_PIN_GPIO_Port GPIOA
#define DISPLAY_CHANGE_PIN_EXTI_IRQn EXTI4_IRQn
#define MOTOR_IN1_PIN_Pin GPIO_PIN_12
#define MOTOR_IN1_PIN_GPIO_Port GPIOB
#define MOTOR_IN3_PIN_Pin GPIO_PIN_13
#define MOTOR_IN3_PIN_GPIO_Port GPIOB
#define BUZZER_PIN_Pin GPIO_PIN_14
#define BUZZER_PIN_GPIO_Port GPIOB
#define FRAME_PIN_Pin GPIO_PIN_15
#define FRAME_PIN_GPIO_Port GPIOB
#define DHT11_PIN_Pin GPIO_PIN_8
#define DHT11_PIN_GPIO_Port GPIOA
#define RELAY_WARNING_LIGHT_PIN_Pin GPIO_PIN_3
#define RELAY_WARNING_LIGHT_PIN_GPIO_Port GPIOB
#define RELAY_LED_PIN_Pin GPIO_PIN_4
#define RELAY_LED_PIN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
