/*
 * YL-69.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Administrator
 */


#include "YL-69.h"

void init_YL69(YL69_t *yl69, ADC_HandleTypeDef *hadc, GPIO_TypeDef* port, uint16_t pin){
	yl69->hadc = hadc;
	yl69->port = port;
	yl69->pin = pin;

	HAL_ADC_Init(yl69->hadc);
	HAL_ADC_Start(yl69->hadc);
}



void readYL69(YL69_t *yl69){
	if(HAL_ADC_PollForConversion(yl69->hadc, 100) == HAL_OK){
		yl69->moisture_ADC_value = HAL_ADC_GetValue(yl69->hadc);
	}else{yl69->moisture_ADC_value = 0;}
}

void moisturePercentage(YL69_t *yl69){
	const float MAX_MOISTURE_ADC = 4095.0f;
	const float MIN_MOISTURE_ADC = 2000.0f;
	float current_ADC = (float)yl69->moisture_ADC_value;
	yl69->moistureValue = ((MAX_MOISTURE_ADC - current_ADC) / (MAX_MOISTURE_ADC - MIN_MOISTURE_ADC))*100.0f;
	if(yl69->moistureValue < 0){
		yl69->moistureValue = 0;
	}else if(yl69->moistureValue > 100){
		yl69->moistureValue = 100;
	}

}
