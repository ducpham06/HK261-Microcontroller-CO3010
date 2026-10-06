/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Pin definitions for Way 1
#define W1_RED        GPIO_PIN_5
#define W1_YELLOW     GPIO_PIN_6
#define W1_GREEN      GPIO_PIN_7

// Pin definitions for Way 2
#define W2_RED        GPIO_PIN_8
#define W2_YELLOW     GPIO_PIN_9
#define W2_GREEN      GPIO_PIN_10

// Common Anode 7-Segment lookup table (0 - 9)
// Bit order: 0b0_gfe_dcba (0 = ON, 1 = OFF)
const uint8_t segment_map[10] = {
    0x40, // 0: a, b, c, d, e, f
    0x79, // 1: b, c
    0x24, // 2: a, b, d, e, g
    0x30, // 3: a, b, c, d, g
    0x19, // 4: b, c, f, g
    0x12, // 5: a, c, d, f, g
    0x02, // 6: a, c, d, e, f, g
    0x78, // 7: a, b, c
    0x00, // 8: a, b, c, d, e, f, g
    0x10  // 9: a, b, c, d, f, g
};

void display7SEG(int num) {
    if (num < 0 || num > 9) return;
    uint8_t segments = segment_map[num];
    for (int i = 0; i < 7; i++) {
        GPIO_PinState state = ((segments >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(GPIOB, (uint16_t)(1 << i), state);
    }
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
  /* USER CODE BEGIN 2 */
    int timer = 0;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */

      // Sub-cycle 1 (3 seconds): Way 1 GREEN (3s countdown), Way 2 RED (5s countdown)
      if (timer < 3) {
          // Way 1: Green ON, Red & Yellow OFF
          HAL_GPIO_WritePin(GPIOA, W1_RED | W1_YELLOW, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOA, W1_GREEN, GPIO_PIN_RESET);

          // Way 2: Red ON, Yellow & Green OFF
          HAL_GPIO_WritePin(GPIOA, W2_RED, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, W2_YELLOW | W2_GREEN, GPIO_PIN_SET);

          display7SEG(3 - timer); // Counts down: 3, 2, 1
      }
      // Sub-cycle 2 (2 seconds): Way 1 YELLOW (2s countdown), Way 2 RED
      else if (timer < 5) {
          // Way 1: Yellow ON, Red & Green OFF
          HAL_GPIO_WritePin(GPIOA, W1_RED | W1_GREEN, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOA, W1_YELLOW, GPIO_PIN_RESET);

          // Way 2: Red ON, Yellow & Green OFF
          HAL_GPIO_WritePin(GPIOA, W2_RED, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, W2_YELLOW | W2_GREEN, GPIO_PIN_SET);

          display7SEG(5 - timer); // Counts down: 2, 1
      }
      // Sub-cycle 3 (3 seconds): Way 1 RED (5s countdown), Way 2 GREEN (3s countdown)
      else if (timer < 8) {
          // Way 1: Red ON, Yellow & Green OFF
          HAL_GPIO_WritePin(GPIOA, W1_RED, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, W1_YELLOW | W1_GREEN, GPIO_PIN_SET);

          // Way 2: Green ON, Red & Yellow OFF
          HAL_GPIO_WritePin(GPIOA, W2_RED | W2_YELLOW, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOA, W2_GREEN, GPIO_PIN_RESET);

          display7SEG(8 - timer); // Counts down: 3, 2, 1
      }
      // Sub-cycle 4 (2 seconds): Way 1 RED, Way 2 YELLOW (2s countdown)
      else {
          // Way 1: Red ON, Yellow & Green OFF
          HAL_GPIO_WritePin(GPIOA, W1_RED, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, W1_YELLOW | W1_GREEN, GPIO_PIN_SET);

          // Way 2: Yellow ON, Red & Green OFF
          HAL_GPIO_WritePin(GPIOA, W2_RED | W2_GREEN, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOA, W2_YELLOW, GPIO_PIN_RESET);

          display7SEG(10 - timer); // Counts down: 2, 1
      }

      HAL_Delay(1000); // 1-second timebase
      timer = (timer + 1) % 10;
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|LED_RED_Pin|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 LED_RED_Pin PA6 PA7
                           PA8 PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|LED_RED_Pin|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
