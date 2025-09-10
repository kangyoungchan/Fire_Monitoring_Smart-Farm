/*
 * MOTOR_RELAY.c
 *
 *  Created on: Aug 16, 2025
 *      Author: sooju
 */

#include "MOTOR_RELAY.h"

void relayLed(uint8_t state){
	HAL_GPIO_WritePin(RELAY_LED_PIN_GPIO_Port, RELAY_LED_PIN_Pin, state);
}

void relayWarningLight(uint8_t state){
	HAL_GPIO_WritePin(RELAY_WARNING_LIGHT_PIN_GPIO_Port, RELAY_WARNING_LIGHT_PIN_Pin, state);
}

void relayBuzzer(uint8_t state){
	HAL_GPIO_WritePin(BUZZER_PIN_GPIO_Port, BUZZER_PIN_Pin, state);
}

void fanState(uint8_t state){
	HAL_GPIO_WritePin(MOTOR_IN1_PIN_GPIO_Port, MOTOR_IN1_PIN_Pin, !state);
	lastFanState = !state;
}

void waterPumpState(uint8_t state){
	HAL_GPIO_WritePin(MOTOR_IN3_PIN_GPIO_Port, MOTOR_IN3_PIN_Pin, !state);
}

