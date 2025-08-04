//
// Created by 86188 on 25-8-4.
//

#ifndef LED_H
#define LED_H

#include "main.h"

class led {
public:
	led(GPIO_TypeDef * gpiox, uint16_t pin);

	void on();
	void off();
	void toggle();
private:
	GPIO_TypeDef *GPIOX;
	uint16_t GPIO_Pin;
};



#endif //LED_H
