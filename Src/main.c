/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include "stdio.h"
	#include <string.h>

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
/* USER CODE BEGIN PFP */

void Indikator(uint32_t dozator);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

const unsigned long port_mask[] = {
	1UL<<0,			/*  0 LED PC  0 */
	1UL<<1,			/*  1 LED PC  1 */
	1UL<<2,			/*  2 LED PC  2 */
	1UL<<3,			/*  3 LED PC  3 */
	1UL<<4,			/*  4 LED PC  4 */
	1UL<<5,			/*  5 LED PC  5 */
	1UL<<6,			/*  6 LED PC  6 */
	1UL<<7,			/*  7 LED PC  7 */
	1UL<<8,			/*  8 LED PC  8 */
	1UL<<9, 		/*  9 LED PC  9 */
	1UL<<0x0A,		/* 10 LED PC 10 */
	1UL<<0x0B,		/* 11 LED PC 11 */
	1UL<<0x0C,		/* 12 LED PC 12 */
	1UL<<0x0D,		/* 13 LED PC 13 */
	1UL<<0x0E,		/* 14 LED PC 14 */
	1UL<<0x0F		/* 15 LED PC 15 */
 };

	#define		DOZ_ARRAY	30
	volatile 	uint8_t tim3_flag_u8 		= 0;
	volatile 	uint32_t time_between_electrons_u32 		= 0;
				uint8_t led_count_u8	= 0;
				uint32_t doz_u32_arr[DOZ_ARRAY];
				int		count_electrons_i = 0;

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
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

	  HAL_TIM_Base_Start(&htim3);
	  HAL_TIM_Base_Start_IT(&htim3);
	  HAL_TIM_Base_Start(&htim4);

  	  char DataChar[100];
  	  sprintf(DataChar,"\r\n Dosimeter SBM19 2020-march-17 \r\nUART3 for debug on speed 115200\r\n\r\n");
  	  HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (tim3_flag_u8 == 1) {
	  	  sprintf(DataChar,"\tTIM3 = 30Sec; %d\r\n", count_electrons_i);
	  	  HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);

		  Indikator(count_electrons_i);

	  	  count_electrons_i = 0;
		  tim3_flag_u8 = 0;
	  }

	  if (time_between_electrons_u32 > 0) {

		  count_electrons_i++;

		  for (int i=0; i < DOZ_ARRAY-1; i++) {
			  doz_u32_arr[i] = doz_u32_arr[i+1];
		  }

		  doz_u32_arr[DOZ_ARRAY-1] = time_between_electrons_u32;

	  	  sprintf(DataChar,"%d) {%d} ", count_electrons_i, (int)doz_u32_arr[DOZ_ARRAY-1]);
	  	  HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);

		  uint32_t res_doz_u32 = 0;
		  for (int i=0; i<DOZ_ARRAY; i++) {
			  res_doz_u32 = res_doz_u32 + doz_u32_arr[i];
		  }

		  sprintf(DataChar,"Total=%d \r\n", (int)res_doz_u32);
		  HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);

		  res_doz_u32 = ( 6000 * DOZ_ARRAY ) / res_doz_u32 ;

		  //Indikator(res_doz_u32);

		  HAL_GPIO_WritePin(LED__GREEN_GPIO_Port,	LED__GREEN_Pin,	SET);
		  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,	LED_YELLOW_Pin, SET);
		  HAL_GPIO_WritePin(LED____RED_GPIO_Port,	LED____RED_Pin, SET);

		  switch (led_count_u8) {
			  case 0: HAL_GPIO_WritePin(LED__GREEN_GPIO_Port,	LED__GREEN_Pin,	RESET); break;
			  case 1: HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,	LED_YELLOW_Pin, RESET); break;
			  case 2: HAL_GPIO_WritePin(LED____RED_GPIO_Port,	LED____RED_Pin,	RESET);	break;
			  case 3: HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,	LED_YELLOW_Pin, RESET); break;
			  default: break;
		  }

		  led_count_u8++;
		  if (led_count_u8 > 3) led_count_u8 = 0;

		  time_between_electrons_u32 = 0;
	  }

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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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

/*----------------------------------------------------------------------------
  Switch on
 *----------------------------------------------------------------------------*/
__INLINE static void Port_A_Off (uint32_t num)
	{
	GPIOA->BRR = port_mask[num];                 /* Turn On  LED          */
	}

/*----------------------------------------------------------------------------
  Switch off
 *----------------------------------------------------------------------------*/
__INLINE static void Port_A_On (uint32_t num)
	{
	GPIOA->BSRR  = port_mask[num];                 /* Turn Off LED      */
	}


/*----------------------------------------------------------------------------
  Switch on B
 *----------------------------------------------------------------------------*/
__INLINE static void Port_B_Off (uint32_t num)
	{
	GPIOB->BRR = port_mask[num];                 /* Turn On  LED          */
	}

/*----------------------------------------------------------------------------
  Switch off B
 *----------------------------------------------------------------------------*/
__INLINE static void Port_B_On (uint32_t num)
	{
	GPIOB->BSRR  = port_mask[num];                 /* Turn Off LED      */
	}


/*----------------------------------------------------------------------------
  Switch on C
 *----------------------------------------------------------------------------*/
__INLINE static void Port_C_Off (uint32_t num)
	{
	GPIOC->BRR = port_mask[num];                 /* Turn On  LED          */
	}

/*----------------------------------------------------------------------------
  Switch off B
 *----------------------------------------------------------------------------*/
__INLINE static void Port_C_On (uint32_t num)
	{
	GPIOC->BSRR  = port_mask[num];                 /* Turn Off LED      */
	}


/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_0 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_1 (void)
	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_Off (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_2 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_3 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_4 (void)
	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_5 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_Off  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_6 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_Off  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_7 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off  (15);	//	d
	Port_B_Off  (14);	//	e
	Port_B_Off  (13);	//	f
	Port_B_Off  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_8 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_9 (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_A (void)
	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_B (void)
	{
	Port_A_Off (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_C (void)
	{
	Port_A_On (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_D (void)
	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_E (void)
	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_1_F (void)
	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
	}


/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_0 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_Off ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_1 (void)
	{
	Port_C_Off (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_2 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_3 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_4 (void)
	{
	Port_C_Off  (13);	// a
	Port_B_On   ( 9);	// b
	Port_B_On   ( 8);	// c
	Port_B_Off  ( 5);	// d
	Port_A_Off  ( 1);	// e
	Port_A_On   ( 2);	// f
	Port_A_On   ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_5 (void)
	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_6 (void)
	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_7 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_8 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_9 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
	}
/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
__INLINE static void Letter_2_F (void)
	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
	}


void Indikator(uint32_t dozator)
	{
	uint8_t doza_10 = dozator/10;
	uint8_t doza_01 = dozator-((doza_10)*10);

	if (doza_10==0) Letter_1_0();
	if (doza_10==1) Letter_1_1();
	if (doza_10==2) Letter_1_2();
	if (doza_10==3) Letter_1_3();
	if (doza_10==4) Letter_1_4();
	if (doza_10==5) Letter_1_5();
	if (doza_10==6) Letter_1_6();
	if (doza_10==7) Letter_1_7();
	if (doza_10==8) Letter_1_8();
	if (doza_10==9) Letter_1_9();
	if (doza_10==10) Letter_1_A();
	if (doza_10==11) Letter_1_B();
	if (doza_10==12) Letter_1_C();
	if (doza_10==13) Letter_1_D();
	if (doza_10==14) Letter_1_E();
	if (doza_10==15) Letter_1_F();

	if (doza_01==0) Letter_2_0();
	if (doza_01==1) Letter_2_1();
	if (doza_01==2) Letter_2_2();
	if (doza_01==3) Letter_2_3();
	if (doza_01==4) Letter_2_4();
	if (doza_01==5) Letter_2_5();
	if (doza_01==6) Letter_2_6();
	if (doza_01==7) Letter_2_7();
	if (doza_01==8) Letter_2_8();
	if (doza_01==9) Letter_2_9();
	if (doza_01 >9) Letter_2_F();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
