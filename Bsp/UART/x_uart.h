//
// Created by 86188 on 25-8-4.
//

#ifndef XUART_H
#define XUART_H

#include "usart.h"
#include <string>
#include <streambuf>
#include <ostream>
#include <sstream>
#include <charconv>
#include <cstring>

class x_uart_stream_buf :public std::streambuf {
private:
	UART_HandleTypeDef* huartx;
public:
	explicit x_uart_stream_buf(UART_HandleTypeDef* uart_x):huartx(uart_x) {}
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


typedef struct {
  enum rx_sta_enum rx_state;		// uart接收数据状态变量
  uint8_t rx_buf;				// uart采用中断接收，一次只接收一个字节

  uint8_t* rx_buffer;			// 接收缓存
  size_t rx_buffer_length;

  uint8_t* rx_data;			// 数据缓存，接收完成了由 rx_buffer 中将数据赋值过来
  size_t rx_data_length;
}uart_ctrl_default_type;

void uart_data_init_default_func(uart_ctrl_default_type* uart);

void uart_data_de_init_default_func(uart_ctrl_default_type* uart);

inline void uart_data_process_default_func(uart_ctrl_default_type *uart_ctrl) {
	if (uart_ctrl->rx_state == Receiving) {
	  if (uart_ctrl->rx_buf == '\r') {			// 接收到了 \r 表明包结束
	    uart_ctrl->rx_state = Waiting_n;
	  }
	  else if (uart_ctrl->rx_buf == '\n') {		// 还未接收到 \r 就接收到了 \n，接收错误，重新接收
	    uart_ctrl->rx_state = Receiving;
	    uart_ctrl->rx_buffer_length = 0;
	  }
	  else {
	    uart_ctrl->rx_buffer[uart_ctrl->rx_buffer_length++] = uart_ctrl->rx_buf;	// 持续接收数据
	  }
	}
	else if (uart_ctrl->rx_state == Waiting_n) {

	  if (uart_ctrl->rx_buf == '\n') {		// 成功接收，将数据从buffer赋值到data
	    memcpy(uart_ctrl->rx_data, uart_ctrl->rx_buffer, uart_ctrl->rx_buffer_length);		// memcpy(target, src, length)
	    uart_ctrl->rx_data_length = uart_ctrl->rx_buffer_length;
	  }
	  else {		// 接收错误，重新开始
	    uart_ctrl->rx_state = Receiving;
	    uart_ctrl->rx_buffer_length = 0;
	  }
	}
	else {		// 严重出错，重置状态
	  uart_ctrl->rx_state = Receiving;
	  uart_ctrl->rx_buffer_length = 0;
	  uart_ctrl->rx_data_length = 0;
	}
}

template <typename rx_data_type = uart_ctrl_default_type>
class x_uart: public std::ostream{
	typedef void (*DataInitFuncPtr)(rx_data_type &rx_data_str);
	typedef void (*DataDeInitFuncPtr)(rx_data_type &rx_data_str);
	typedef void (*DataProcFuncPtr)(rx_data_type &rx_data_str);
private:
	inline static x_uart* 	UART1_register = nullptr;
	inline static x_uart* 	UART2_register = nullptr;
	inline static x_uart* 	UART3_register = nullptr;
	inline static x_uart* 	UART4_register = nullptr;
	inline static x_uart* 	UART5_register = nullptr;
	inline static x_uart* 	UART6_register = nullptr;
	inline static x_uart* 	UART7_register = nullptr;
	inline static x_uart* 	UART8_register = nullptr;
	inline static x_uart* 	UART9_register = nullptr;
	inline static x_uart* 	UART10_register = nullptr;

	x_uart_stream_buf		uart_stream_buf;
	UART_HandleTypeDef*		this_uart;
	uint8_t*				rx_buffer;
	size_t					rx_buffer_length;

protected:
	rx_data_type			rx_data{};		/// 用于与C文件中的uart回调函数联系

	DataInitFuncPtr			init_func_;
	DataDeInitFuncPtr		de_init_func_;
	DataProcFuncPtr			proc_func_;

public:
	explicit x_uart(UART_HandleTypeDef* uart_x,
					DataInitFuncPtr		init_func=uart_data_init_default_func,
					DataDeInitFuncPtr	de_init_func=uart_data_de_init_default_func,
					DataProcFuncPtr		proc_func=uart_data_process_default_func);

	~x_uart() override;

	/** API **/
	[[nodiscard]] uint8_t*	get_rx_buffer() const;				// 直接获取buffer地址
	[[nodiscard]] size_t	get_rx_buffer_length() const;			// 直接获取缓存数据量

	HAL_StatusTypeDef		transmit(uint8_t* buffer, size_t length, uint16_t time_out) const;	// 发送数据
	HAL_StatusTypeDef		transmit(std::string &str, uint16_t time_out) const;				// 发送数据

	void					begin_receive() const;					// 打开接收中断

	// template<typename ItemType>
	// x_uart&					operator>>(ItemType & value) {
	// 	while (uart_ctrl.rx_data_length == 0) {}	// 等待数据接收
	//
	// 	std::from_chars(reinterpret_cast<char *>(uart_ctrl.rx_data),
	// 					reinterpret_cast<char *>(uart_ctrl.rx_data+uart_ctrl.rx_data_length),
	// 					value);
	//
	// 	return *this;
	// }

	// x_uart&					operator>>(std::string& str);


	/** 友元函数接口 **/
	// friend void USER_UART_RxCallback(uart_ctrl_type *uart_ctrl);	// 在uart接收中断里调用，用于访问uart_ctrl
};




#endif //XUART_H
