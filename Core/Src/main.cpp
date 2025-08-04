//
// Created by 86188 on 25-8-4.
//

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "ESP8266/bsp_ESP8266.h"         // ESP8266通信的自定义文件，包含：发送函数、接收函数等
#include "UART/x_uart.h"


extern "C" int main(void)
{
HAL_Delay(1000);
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  MX_USART1_UART_Init();
  MX_USART3_UART_Init();

  x_uart uart1_handler(&huart1, &uart1_ctrl_linker);
  x_uart uart3_handler(&huart3, &uart3_ctrl_linker);

  uart1_handler.transmit((uint8_t*)"HelloWorld!", 11);

  while (1)
  {
    HAL_Delay(1);
  }

}

