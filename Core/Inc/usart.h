/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

#define UART_RX_BUFFER_SIZE 128

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

enum rx_sta_enum {Receiving, Waiting_n};

typedef struct {
  enum rx_sta_enum rx_state;		// uart接收数据状态变量
  uint8_t rx_buf;				// uart采用中断接收，一次只接收一个字节

  uint8_t* rx_buffer;			// 接收缓存
  size_t rx_buffer_length;

  uint8_t* rx_data;			// 数据缓存，接收完成了由 rx_buffer 中将数据赋值过来
  size_t rx_data_length;
}uart_ctrl_type;


void USER_UART_RxCallback(uart_ctrl_type *uart_ctrl);


extern uart_ctrl_type *uart1_ctrl_linker;    // 用于联系 x_uart 类和这里的中断函数
extern uart_ctrl_type *uart3_ctrl_linker;    // 用于联系 x_uart 类和这里的中断函数

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

