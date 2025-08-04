//
// Created by 86188 on 25-8-4.
//

#include "led.h"

led::led(GPIO_TypeDef * gpiox, uint16_t pin) {
	GPIOX = gpiox;
	GPIO_Pin = pin;
}

void led::on() {
	HAL_GPIO_WritePin(GPIOX, GPIO_Pin, GPIO_PIN_RESET);
}

void led::off() {
	HAL_GPIO_WritePin(GPIOX, GPIO_Pin, GPIO_PIN_SET);
}

void led::toggle() {
	HAL_GPIO_TogglePin(GPIOX, GPIO_Pin);
}


