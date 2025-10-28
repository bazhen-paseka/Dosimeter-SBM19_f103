/*
 * local_config.h
 *
 *  Created on: Oct 16, 2025
 *      Author: paseka
 */

#ifndef DOSIMETER_SBM19_F103_LOCAL_CONFIG_INCLUDED_H_
#define DOSIMETER_SBM19_F103_LOCAL_CONFIG_INCLUDED_H_

	#define 	SOFT_VERSION 			2020
	#define 	DEBUG_LVL 				3
	#define 	UART_DEBUG				huart1
	#define 	TIM_60_SEC				htim3
	#define 	TIM_BETWEEN				htim4
	#define		START_RADIATION_VALUE	95
	#define		VALUE_ARRAY_CNT			100
	#define		HELP_DIVISION			5
	#define		DISPLAY_TM1637
	#define		LED_4_PCS

#endif /* DOSIMETER_SBM19_F103_LOCAL_CONFIG_INCLUDED_H_ */
