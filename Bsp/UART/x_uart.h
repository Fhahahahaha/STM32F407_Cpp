//
// Created by 86188 on 25-8-4.
//

#ifndef XUART_H
#define XUART_H

#include "usart.h"
#include <string>
#include <streambuf>
#include <ostream>

class x_uart_stream :public std::streambuf {
private:
	UART_HandleTypeDef* huartx;
public:
	explicit x_uart_stream(UART_HandleTypeDef* uart_x):huartx(uart_x) {}
protected:
	// 核心函数：处理单个字符输出
	int_type overflow(int_type c) override {
		if (c != traits_type::eof()) {
			auto ch = static_cast<uint8_t>(c);
			HAL_UART_Transmit(&huart1, &ch, 1, 1);
		}
		return c;
	}

	// 默认实现会循环调用overflow()，重写可直接处理多字符
	std::streamsize xsputn(const char* s, std::streamsize n) override {
		HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>(s), n, n);	// 平均每个字节最多等1ms
		return n;
	}
};

class x_uart{
private:
	x_uart_stream			uart_stream_buf;
	std::ostream			uart_ostream;
	UART_HandleTypeDef*		this_uart{};

public:
	explicit x_uart(UART_HandleTypeDef* uart_x, uart_ctrl_type ** uart_ctrl_linker);
	~x_uart();

	/** API **/
	HAL_StatusTypeDef		transmit(uint8_t* buffer, size_t length, uint16_t time_out) const;	// 发送数据
	HAL_StatusTypeDef		transmit(std::string &str, uint16_t time_out) const;				// 发送数据
	std::ostream&			stream();		// 用于流式输出

	[[nodiscard]] uint8_t*	get_rx_buffer() const;				// 直接获取buffer地址
	size_t					read_rx_data(uint8_t* target);		// 读取数据，并清除 rx_data_length
	[[nodiscard]] size_t	get_rx_data_length() const;
	void					begin_receive();					// 打开接收中断

	/** 对外类成员 **/
	uart_ctrl_type			uart_ctrl{};		/// 用于与C文件中的uart回调函数联系
};




#endif //XUART_H
