//
// Created by 86188 on 25-8-4.
//

#ifndef XUART_H
#define XUART_H

#include "usart.h"
#include <string>

class x_uart {
public:
	explicit x_uart(UART_HandleTypeDef* uart_x, uart_ctrl_type ** uart_ctrl_linker);
	~x_uart();

	/** API **/
	HAL_StatusTypeDef transmit(uint8_t* buffer, size_t length, uint16_t time_out) const;		// 发送数据
	HAL_StatusTypeDef transmit(std::string &str, uint16_t time_out) const;		// 发送数据
	// [[nodiscard]] 的作用是告诉编译器：调用这个函数时，它的返回值不应被忽略
	[[nodiscard]] uint8_t* get_rx_buffer() const;					// 直接获取buffer地址
	size_t read_rx_data(uint8_t* target);
	[[nodiscard]] size_t get_rx_data_length() const;
	void begin_receive();

	/** 用于与C文件中的uart回调函数联系 **/
	uart_ctrl_type uart_ctrl{};

private:
	UART_HandleTypeDef* this_uart;

	// enum rx_sta_enum {Receiving, Waiting_n};
	// rx_sta_enum rx_state;		// uart接收数据状态变量
	//
	// uint8_t rx_buf;				// uart采用中断接收，一次只接收一个字节
	//
	// uint8_t* rx_buffer;			// 接收缓存
	// size_t rx_buffer_length;
	//
	// uint8_t* rx_data;			// 数据缓存，接收完成了由 rx_buffer 中将数据赋值过来
	// size_t rx_data_length;
};



#endif //XUART_H
