//
// Created by 86188 on 25-8-4.
//

#include "x_uart.h"
#include "cstring"
#include "malloc.h"

/** 注： 初始化列表的顺序一定要遵守变量声明顺序 ！！！ **/
template<typename rx_data_type>
x_uart<rx_data_type>::x_uart(UART_HandleTypeDef *uart_x,
							DataInitFuncPtr init_func, DataDeInitFuncPtr de_init_func, DataProcFuncPtr proc_func)
							:uart_stream_buf(uart_x), std::ostream(&uart_stream_buf)  {


	/** 注册UARTx实例；	在中断回调函数中使用；	用于根据不同uart实例调用不同数据处理协议 **/
	if (uart_x->Instance == USART1) {
		x_uart::UART1_register = this;
	}
	else if (uart_x->Instance == USART2) {
		x_uart::UART2_register = this;
	}
	else if (uart_x->Instance == USART3) {
		x_uart::UART3_register = this;
	}
	else if (uart_x->Instance == UART4) {
		x_uart::UART4_register = this;
	}
	else if (uart_x->Instance == UART5) {
		x_uart::UART5_register = this;
	}
	else if (uart_x->Instance == USART6) {
		x_uart::UART6_register = this;
	}

	/** 初始化类成员 **/
	this_uart = uart_x;

	rx_buffer = static_cast<uint8_t *>(malloc(UART_RX_BUFFER_SIZE));
	rx_buffer_length = 0;

	init_func_ = init_func;
	de_init_func_ = de_init_func;
	proc_func_ = proc_func;				// 数据结构体处理，用于通信协议

	init_func_(rx_data);				// 初始化接收数据结构体
}

template<typename rx_data_type>
x_uart<rx_data_type>::~x_uart() {
	free(rx_buffer);
	de_init_func_(rx_data);
}

template<typename rx_data_type>
uint8_t *x_uart<rx_data_type>::get_rx_buffer() const {
	return rx_buffer;
}

template<typename rx_data_type>
size_t x_uart<rx_data_type>::get_rx_buffer_length() const {
	return rx_buffer_length;
}

template<typename rx_data_type>
HAL_StatusTypeDef x_uart<rx_data_type>::transmit(uint8_t *buffer, size_t length, uint16_t time_out) const {
	return HAL_UART_Transmit(this_uart, buffer, length, time_out);
}

template<typename rx_data_type>
HAL_StatusTypeDef x_uart<rx_data_type>::transmit(std::string &str, uint16_t time_out) const {
	return HAL_UART_Transmit(this_uart, (uint8_t*)str.data(), str.length(), time_out);
}

template<typename rx_data_type>
void x_uart<rx_data_type>::begin_receive() const {
	HAL_UARTEx_ReceiveToIdle_DMA(this_uart, (rx_buffer), UART_RX_BUFFER_SIZE);
}


// size_t x_uart<uart_ctrl_type>::read_rx_data(uint8_t *target) {
// 	size_t length = uart_ctrl.rx_data_length;
// 	if (length > 0) {
// 		memcpy(target, uart_ctrl.rx_data, length);
// 		uart_ctrl.rx_data_length = 0;
// 	}
// 	return length;
// }

// void x_uart<uart_ctrl_type>::begin_receive() {
// 	HAL_UART_Receive_IT(this_uart, &uart_ctrl.rx_buf, 1);
// }

// template<typename ItemType>
// x_uart &x_uart::operator>>(ItemType &value) {
// 	while (uart_ctrl.rx_data_length == 0) {}	// 等待数据接收
//
// 	std::from_chars(reinterpret_cast<char *>(uart_ctrl.rx_data),
// 					reinterpret_cast<char *>(uart_ctrl.rx_data+uart_ctrl.rx_data_length),
// 					value);
//
// 	return *this;
// }

// x_uart &x_uart::operator>>(std::string &str) {
// 	while (uart_ctrl.rx_data_length == 0) {}	// 等待数据接收
//
// 	str.assign(reinterpret_cast<char *>(uart_ctrl.rx_data), uart_ctrl.rx_data_length);
//
// 	return *this;
// }
