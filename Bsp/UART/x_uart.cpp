//
// Created by 86188 on 25-8-4.
//

#include "x_uart.h"
#include "cstring"
#include "malloc.h"

x_uart::x_uart(UART_HandleTypeDef* uart_x, uart_ctrl_type ** uart_ctrl_linker) {
	this_uart = uart_x;

	uart_ctrl = {
		.rx_state = Receiving,
		.rx_buf = 0,

		.rx_buffer = (uint8_t *)malloc(UART_RX_BUFFER_SIZE),
		.rx_buffer_length = 0,

		.rx_data = (uint8_t *)malloc(UART_RX_BUFFER_SIZE),
		.rx_data_length = 0,
	};

	*uart_ctrl_linker = &uart_ctrl;		// 连接中断回调函数的 uart_ctrl_type 指针
}

x_uart::~x_uart() {
	free(uart_ctrl.rx_buffer);
	free(uart_ctrl.rx_data);
}


HAL_StatusTypeDef x_uart::transmit(uint8_t *buffer, size_t length) const {
	return HAL_UART_Transmit(this_uart, buffer, length, 1000);
}

uint8_t *x_uart::get_rx_buffer() const {
	return uart_ctrl.rx_buffer;
}

size_t x_uart::read_rx_data(uint8_t *target) {
	size_t length = uart_ctrl.rx_data_length;
	if (length > 0) {
		memcpy(target, uart_ctrl.rx_data, uart_ctrl.rx_data_length);
		uart_ctrl.rx_data_length = 0;
	}
	return length;
}


void x_uart::begin_receive() {
	HAL_UART_Receive_IT(this_uart, &uart_ctrl.rx_buf, 1);
}


