/*
 * UART.c
 *
 *  Created on: Aug 20, 2025
 *      Author: Administrator
 */

#include "UART.h"

volatile char rx_line_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_line_index = 0;
volatile uint8_t new_line_received = 0;
const uint8_t modeDisplaySuccess = 6;
const uint8_t ledChangeDisplaySuccess = 7;
const uint8_t fanChangeDisplaySuccess = 8;
const uint8_t waterChangeDisplaySuccess = 9;
const uint8_t timeSetDisplaySuccess = 10;
const uint8_t targetSetDisplaySuccess = 11;

void uartInterruptEnable() {
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void uartInterruptDisable() {
	HAL_NVIC_DisableIRQ(USART2_IRQn);
}

void commandParsing(char *commandString) {
	int ledOnTime = 0;
	int ledOffTime = 0;
	int tempTarget = 0;
	int humidityTarget = 0;
	int moistureTarget = 0;

	if (strstr(commandString, "MODE") != NULL) {
		if (strstr(commandString, "AUTO") != NULL) {
			controlState = 0;
		} else if (strstr(commandString, "CONTROL") != NULL) {
			controlState = 1;
		}
		displaySuccess(modeDisplaySuccess);
	}


	if (controlState == 1) {
		if (strstr(commandString, "ACTIVE") != NULL) {
			if (strstr(commandString, "LED_ON") != NULL) {
				relayLed(ON);
				displaySuccess(ledChangeDisplaySuccess);
			} else if (strstr(commandString, "LED_OFF") != NULL) {
				relayLed(OFF);
				displaySuccess(ledChangeDisplaySuccess);
			} else if (strstr(commandString, "FAN_ON") != NULL) {
				fanState(ON);
				displaySuccess(fanChangeDisplaySuccess);
			} else if (strstr(commandString, "FAN_OFF") != NULL) {
				fanState(OFF);
				displaySuccess(fanChangeDisplaySuccess);
			} else if (strstr(commandString, "WATER_ON") != NULL) {
				waterPumpState(ON);
				displaySuccess(waterChangeDisplaySuccess);
			} else if (strstr(commandString, "WATER_OFF") != NULL) {
				waterPumpState(OFF);
				displaySuccess(waterChangeDisplaySuccess);
			}
		}

			if(strstr(commandString, "TIME") != NULL) {
				if (sscanf(commandString, "TIME:ON_%d/OFF_%d", &ledOnTime, &ledOffTime) == 2) {
					LED_ON_TIME.Hours = ledOnTime;
					LED_OFF_TIME.Hours = ledOffTime;
					displaySuccess(timeSetDisplaySuccess);
				}
			}
			if (strstr(commandString, "VALUE") != NULL) {
				if (sscanf(commandString, "VALUE:T_%d/H_%d/M_%d", &tempTarget, &humidityTarget, &moistureTarget) == 3) {
					tempTargetValue = tempTarget;
					humidityTargetValue = humidityTarget;
					moistureTargetValue = moistureTarget;
					displaySuccess(targetSetDisplaySuccess);
				}
            }


	    }
	}

	void dataRecive_and_operate() {

		char command_string[RX_BUFFER_SIZE];

		if (new_line_received) {

			uartInterruptDisable();
			new_line_received = 0;
			strcpy(command_string, (const char*) rx_line_buffer);
			uartInterruptEnable();

			commandParsing(command_string);
		}

	}

	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

		if (huart->Instance == USART2) {
			if (rx_char == '\n' || rx_char == '\r') {
				if (rx_line_index > 0) {
					rx_line_buffer[rx_line_index] = '\0';
					new_line_received = 1;
				}
				rx_line_index = 0;
			} else {
				if (rx_line_index < (RX_BUFFER_SIZE - 1)) {
					rx_line_buffer[rx_line_index++] = rx_char;
				}

			}
			HAL_UART_Receive_IT(&huart2, &rx_char, 1);
		}

	}
