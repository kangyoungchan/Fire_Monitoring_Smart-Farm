/*
 * UART.h
 *
 *  Created on: Aug 20, 2025
 *      Author: Administrator
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "MOTOR_RELAY.h"
#include "Time.h"
#include "Display.h"

extern uint8_t controlState;
extern uint8_t tempTargetValue;
extern uint8_t humidityTargetValue;
extern uint8_t moistureTargetValue;
extern LED_Time_set LED_ON_TIME;
extern LED_Time_set LED_OFF_TIME;
extern UART_HandleTypeDef huart2;
extern uint8_t rx_char;
extern uint8_t visualFire;

#define RX_BUFFER_SIZE 64
#define ON 0
#define OFF 1


void uartInterruptEnable();

void uartInterruptDisable();

void commandParsing(char *commandString);

void dataRecive_and_operate();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_UART_H_ */
