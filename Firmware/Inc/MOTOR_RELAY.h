/*
 * MOTOR_RELAY.h
 *
 *  Created on: Aug 16, 2025
 *      Author: sooju
 */

#ifndef MOTOR_RELAY_H_
#define MOTOR_RELAY_H_

#include "main.h"

extern uint8_t lastFanState;

void relayLed(uint8_t state);

void relayWarningLight(uint8_t state);

void relayBuzzer(uint8_t state);

void fanState(uint8_t state);

void waterPumpState(uint8_t state);



#endif /* MOTOR_RELAY_H_ */
