/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D2_A_GPIO_Port, D2_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D2_E_Pin|D2_F_Pin|D2_G_Pin|D1_C_Pin
                          |D1_B_Pin|D1_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_1_Pin|LED_2_Pin|LED_3_Pin|LED_4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BUZZER_Pin|D1_G_Pin|D1_F_Pin|D1_E_Pin
                          |D1_D_Pin|D2_D_Pin|TM1637_CLK_Pin|TM1637_DIO_Pin
                          |D2_C_Pin|D2_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : D2_A_Pin */
  GPIO_InitStruct.Pin = D2_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(D2_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D2_E_Pin D2_F_Pin D2_G_Pin LED_1_Pin
                           LED_2_Pin LED_3_Pin LED_4_Pin D1_C_Pin
                           D1_B_Pin D1_A_Pin */
  GPIO_InitStruct.Pin = D2_E_Pin|D2_F_Pin|D2_G_Pin|LED_1_Pin
                          |LED_2_Pin|LED_3_Pin|LED_4_Pin|D1_C_Pin
                          |D1_B_Pin|D1_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BUZZER_Pin D1_G_Pin D1_F_Pin D1_E_Pin
                           D1_D_Pin D2_D_Pin TM1637_CLK_Pin TM1637_DIO_Pin
                           D2_C_Pin D2_B_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin|D1_G_Pin|D1_F_Pin|D1_E_Pin
                          |D1_D_Pin|D2_D_Pin|TM1637_CLK_Pin|TM1637_DIO_Pin
                          |D2_C_Pin|D2_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SBM19_Pin */
  GPIO_InitStruct.Pin = SBM19_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SBM19_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
