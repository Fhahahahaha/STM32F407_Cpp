//
// Created by 86188 on 25-8-4.
//

#include "main.h"
#include <cstring>
#include <iostream>
#include <ostream>

#include "gpio.h"
#include "usart.h"
#include "ESP8266/esp8266.h"
#include "UART/x_uart.h"
#include "LED/led.h"


extern "C" int main(void)
{
  /** HAL_SYS_Init **/
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  MX_USART1_UART_Init();
  MX_USART3_UART_Init();

  /** My Init **/
  led blue_led(LED_BLUE_GPIO_Port, LED_BLUE_Pin );
  led red_led(LED_RED_GPIO_Port, LED_RED_Pin );

  blue_led.on();
  x_uart uart1_handler(&huart1, &uart1_ctrl_linker);
  // uart1_handler.begin_receive();

  // esp8266 esp8266_handler(&huart3, &uart3_ctrl_linker);   // esp8266 已经继承了 x_uart 类
  // esp8266_handler.begin_receive();

  uart1_handler.transmit((uint8_t*)"HelloWorld!\r\n", strlen("HelloWorld!\r\n"), 1000);

  uart1_handler.stream() << "HELLO WORLD! \r\n" << 213123 << "sfs";

  // esp8266_handler.AT();
  // esp8266_handler.AT_CWMODE(esp8266::STA);
  // esp8266_handler.AT_CWJAP("LAPTOP-D8AC4NAL 5640", "81uL521<");
  // esp8266_handler.AT_CIPSTART("TCP", "192.168.137.1", "6466");
  // esp8266_handler.AT_CIPMUX(0);
  // esp8266_handler.AT_SetPassThrough(1);
  // size_t temp_len;
  while (1)
  {
    red_led.toggle();
    // temp_len = uart1_handler.get_rx_data_length();
    // if (temp_len > 0) {
    //   auto *temp = (uint8_t*)malloc(temp_len);
    //   uart1_handler.read_rx_data(temp);
    //
    //   esp8266_handler.transmit(temp, temp_len, 1000);
    // }
    HAL_Delay(1);
  }

}

