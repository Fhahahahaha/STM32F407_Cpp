//
// Created by 86188 on 25-8-4.
//

#ifndef ESP8266_H
#define ESP8266_H

#include "UART/x_uart.h"
#include <string>

using namespace std;

class esp8266 : public x_uart{
public:
	enum Working_Mode {STA=1, AP=2, STA_AP=3};	// AT参数：1-STA、2-AP、3-STA+AP

	esp8266(UART_HandleTypeDef* uart_x, uart_ctrl_type ** uart_ctrl_linker):x_uart(uart_x, uart_ctrl_linker) {

	}

	/** 基本指令 **/
	void	SendString(string &a);                  // 发送字符串;
	void	SendString(const char *a, size_t len);	// 发送字符串的 char* 重构
	uint8_t	SendAT(string &pcStr, string &pcRes, uint16_t time_out);		// 成功则返回 0
	uint8_t	SendAT(const char * str, const char * res, int time_out);	// 成功则返回 0
	/** 扩展函数：常用AT指令 **/
	uint8_t	AT();                                                // 发送AT命令，可用于测试线路连接是否正常，和测试代码收发机制的正确性
	uint8_t	AT_RESTORE();                                        // 恢复出厂设置; 可用于清除芯片中已保存的wifi名称、密码
	uint8_t	AT_RST();                                            // 复位、重启
	uint8_t	AT_CWMODE(Working_Mode mode);                                 // 设置工作模式; 参数：1-STA、2-AP、3-STA+AP
	uint8_t	AT_CIPMUX(uint8_t value);                                // 设置：多连接，0_单连接，1_多连接;备注：透传必须AT+CIPMUX=0;
	uint8_t	AT_CIPMODE(uint8_t value);                               // 设置：传输方式，0_普通传输，1_透明传输
	uint8_t	AT_SetPassThrough(uint8_t value);                        // 设置 透传; 参数：0-关、1-开; 返回：0-成功

private:

};



#endif //H
