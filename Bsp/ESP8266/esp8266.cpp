//
// Created by 86188 on 25-8-4.
//

#include "esp8266.h"
#include <cstring>

/** ———————————— 基本指令 ———————————— **/
void esp8266::SendString(string &a) {
	*this << a;
}

void esp8266::SendString(const char *a) {
	this->write(a, static_cast<int>(strlen(a)));
}

void esp8266::SendString(const char *a, int len) {
	this->write(a, len);
}

uint8_t esp8266::SendAT(string &pcStr, string &pcRes) {
	string temp;

	uart_ctrl.rx_data_length = 0;	// 预先清除接收的数据
	SendString(pcStr);	// 发送数据
	*this >> temp;			// 这里是无限阻塞接收

	return temp == pcRes;	// 比较期望的字符串，以判断命令是否发送成功
}

uint8_t esp8266::SendAT(const char *str, string &pcRes) {
	string temp;

	uart_ctrl.rx_data_length = 0;	// 预先清除接收的数据
	SendString(str);
	*this >> temp;			// 这里是无限阻塞接收

	return temp == pcRes;
}

uint8_t esp8266::SendAT(string &pcStr, const char * res){
	string temp;

	uart_ctrl.rx_data_length = 0;
	SendString(pcStr);
	*this >> temp;

	return temp == res;
}

uint8_t esp8266::SendAT(const char * str, const char * res) {
	uart_ctrl.rx_data_length = 0;
	SendString(str);
	char* temp = (char*)malloc(uart_ctrl.rx_data_length + 1);
	size_t temp_len = read_rx_data((uint8_t*)temp);
	temp[temp_len] = '\0';

	uint8_t result = strcmp(temp, res);
	free(temp);

	return result;
}

uint8_t esp8266::SendAT(string &pcStr, string &pcRes, uint16_t time_out) {
	uart_ctrl.rx_data_length = 0;	// 预先清除接收的数据

	SendString(pcStr);			// 发送数据

	/* 等到接收数据，直到time_out */
	size_t start_tick = HAL_GetTick();
	while (uart_ctrl.rx_data_length == 0) {
		if (HAL_GetTick() - start_tick > time_out) {
			return -1;
		}
	}

	string temp(reinterpret_cast<char *>(uart_ctrl.rx_data), uart_ctrl.rx_data_length);

	return temp == pcRes;	// 比较期望的字符串，以判断命令是否发送成功
}

uint8_t esp8266::SendAT(const char *str, const char *res, uint16_t time_out) {
	uart_ctrl.rx_data_length = 0;	// 预先清除接收的数据

	SendString(str);				// 发送数据

	/* 等到接收数据，直到time_out */
	size_t start_tick = HAL_GetTick();
	while (uart_ctrl.rx_data_length == 0) {
		if (HAL_GetTick() - start_tick > time_out) {
			return -1;
		}
	}
	char* temp = (char*)malloc(uart_ctrl.rx_data_length + 1);
	size_t temp_len = read_rx_data((uint8_t*)temp);
	temp[temp_len] = '\0';

	uint8_t result = strcmp(temp, res);
	free(temp);

	return result;	// 比较期望的字符串，以判断命令是否发送成功
}

/** ———————————— 扩展函数：常用AT指令 ———————————— **/
uint8_t esp8266::AT(uint16_t time_out) {
	return SendAT("AT\r\n", "OK", time_out);
}

uint8_t esp8266::AT_RESTORE(uint16_t time_out) {
	return SendAT("AT+RESTORE\r\n", "ready", time_out);   // 恢复模块的出厂设置
}

uint8_t esp8266::AT_RST(uint16_t time_out) {
	return SendAT("AT+RST\r\n", "ready", time_out);
}

uint8_t esp8266::AT_CWMODE(Working_Mode mode, uint16_t time_out) {
	string temp("AT+CWMODE=");
	temp += to_string((int)mode);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, time_out)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_CIPMUX(uint8_t value, uint16_t time_out) {
	string temp("AT+CIPMUX=");
	temp += to_string(value);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, time_out)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_CIPMODE(uint8_t value, uint16_t time_out) {
	string temp("AT+CIPMODE=");
	temp += to_string(value);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, time_out)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_SetPassThrough(uint8_t value, uint16_t time_out) {
	if (value == 1)
	{
		uint8_t res = 0;
		res += SendAT("AT+CIPMODE=1\r\n", "OK", time_out);
		res += SendAT("AT+CIPSEND\r\n",    ">", time_out);
		return res;
	}

	SendString("+++", strlen("+++"));
	return 0;
}

uint8_t esp8266::AT_CWJAP(const string& ssid, const string &pass, uint16_t time_out) {
	string temp("AT+CWJAP=");
	temp = temp + '"' +  ssid + '"' + ',' + '"' + pass + '"';
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, time_out)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_CIPSTART(const string& mode, const string &IP, const string &Port, uint16_t time_out) {
	string temp("AT+CIPSTART=");
	temp = temp + '"' +  mode + '"' + ',' + '"' + IP + '"' + ',' + '"' + Port + '"';
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, time_out)  ;    // 发送给8266，并判断指令是否执行成功
}

