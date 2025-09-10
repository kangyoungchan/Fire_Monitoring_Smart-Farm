/*
 * YL-69.h
 *
 *  Created on: Aug 13, 2025
 *      Author: Administrator
 */

#ifndef INC_YL_69_H_
#define INC_YL_69_H_

#include "main.h"


typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	ADC_HandleTypeDef *hadc;
	uint16_t moisture_ADC_value;
	float moistureValue;
}YL69_t;

void init_YL69(YL69_t *yl69, ADC_HandleTypeDef *hadc, GPIO_TypeDef* port, uint16_t pin);
void readYL69(YL69_t *yl69);
void moisturePercentage(YL69_t *yl69);

#endif /* INC_YL_69_H_ */
