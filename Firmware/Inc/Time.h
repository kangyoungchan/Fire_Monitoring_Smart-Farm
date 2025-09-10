/*
 * Time.h
 *
 *  Created on: Aug 17, 2025
 *      Author: sooju
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include "main.h"

typedef struct{
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
}LED_Time_set;

extern RTC_HandleTypeDef hrtc;

void ledOnTimeSet(LED_Time_set *LED_ON_TIME, uint8_t hour, uint8_t minute, uint8_t second);

void ledOffTimeSet(LED_Time_set *LED_OFF_TIME, uint8_t hour, uint8_t minute, uint8_t second);

void ledOnTimeGet(LED_Time_set *LED_ON_TIME, uint16_t *ledOnTime);

void ledOffTimeGet(LED_Time_set *LED_OFF_TIME, uint16_t *ledOffTime);

void getTime(RTC_TimeTypeDef *gTime, uint16_t *currentGetTime);

void getDate(RTC_DateTypeDef *gDate, uint16_t *currentGetDate);

void getTimeStamp(RTC_TimeTypeDef *gTime , char *timeStamp);
#endif /* INC_TIME_H_ */
