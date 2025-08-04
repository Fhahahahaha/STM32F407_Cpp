//
// Created by 86188 on 25-8-4.
//

#include "esp8266.h"

#include <cstring>

void esp8266::SendString(string &a) {
	transmit((uint8_t*)a.data(), a.size());
}

void esp8266::SendString(const char *a, size_t len) {
	transmit((uint8_t*)a, len);
}

uint8_t esp8266::SendAT(string &pcStr, string &pcRes, uint16_t time_out) {
	uart_ctrl.rx_data_length = 0;

	SendString(pcStr);

	int start_tick = HAL_GetTick();
	while (uart_ctrl.rx_data_length == 0) {
		if (HAL_GetTick() - start_tick > time_out) {
			return -1;
		}
	}

	string temp(reinterpret_cast<char *>(uart_ctrl.rx_data), uart_ctrl.rx_data_length);

	return temp == pcRes;	// 比较期望的字符串，以判断命令是否发送成功
}

uint8_t esp8266::SendAT(const char *str, const char *res, int time_out) {
	uart_ctrl.rx_data_length = 0;

	SendString(str, strlen(str));

	int start_tick = HAL_GetTick();
	while (uart_ctrl.rx_data_length == 0) {
		if (HAL_GetTick() - start_tick > time_out) {
			return -1;
		}
	}
	char* temp = (char*)malloc(uart_ctrl.rx_data_length + 1);
	int temp_len = read_rx_data((uint8_t*)temp);
	temp[temp_len] = '\0';

	uint8_t res = strcmp(temp, res);
	free(temp);

	return res;	// 比较期望的字符串，以判断命令是否发送成功
}

uint8_t esp8266::AT() {
	return SendAT("AT\r\n", "OK", 1000);
}

uint8_t esp8266::AT_RESTORE() {
	return SendAT("AT+RESTORE\r\n", "ready", 3000);   // 恢复模块的出厂设置
}

uint8_t esp8266::AT_RST() {
	return SendAT("AT+RST\r\n", "ready", 3000);
}

uint8_t esp8266::AT_CWMODE(Working_Mode mode) {
	string temp("AT+CWMODE=");
	temp += to_string((int)mode);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, 3000)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_CIPMUX(uint8_t value) {
	string temp("AT+CIPMUX=");
	temp += to_string(value);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, 3000)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_CIPMODE(uint8_t value) {
	string temp("AT+CIPMODE=");
	temp += to_string(value);
	temp += "\r\n";

	string res("OK");

	return SendAT(temp, res, 3000)  ;    // 发送给8266，并判断指令是否执行成功
}

uint8_t esp8266::AT_SetPassThrough(uint8_t value) {
	if (value == 1)
	{
		uint8_t res = 0;
		res += SendAT("AT+CIPMODE=1\r\n", "OK", 3000);
		res += SendAT("AT+CIPSEND\r\n",    ">", 3000);
		return res;
	}

	SendString("+++", strlen("+++"));
	return 0;
}

