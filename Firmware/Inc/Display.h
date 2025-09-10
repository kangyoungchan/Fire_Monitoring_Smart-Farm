/*
 * Display.h
 *
 *  Created on: Aug 15, 2025
 *      Author: sooju
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"
#include "I2C_LCD_cfg.h"
#include "Time.h"
#include "string.h"


void displayFireSensorDetect();

void displaySmokeSenosrDetect();

void displayFire_SmokeSensorDetect();

void displaySensorData(char Temp[], char Humidty[], char Moisture[] , uint8_t mode);

void displayLedTime(LED_Time_set *LED_ON_TIME , LED_Time_set *LED_OFF_TIME);

void displayTargetValue(uint8_t tempTargetValue , uint8_t humidityTargetValue, uint8_t moistureTargetValue);

void displaySuccess(uint8_t state);

#endif /* INC_DISPLAY_H_ */
