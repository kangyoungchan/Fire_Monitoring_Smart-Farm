/*
 * Time.c
 *
 *  Created on: Aug 17, 2025
 *      Author: sooju
 */


#include "Time.h"

void ledOnTimeSet(LED_Time_set *LED_ON_TIME, uint8_t hour, uint8_t minute, uint8_t second){
	LED_ON_TIME->Hours = hour;
	LED_ON_TIME->Minutes = minute;
	LED_ON_TIME->Seconds = second;
}

void ledOffTimeSet(LED_Time_set *LED_OFF_TIME, uint8_t hour, uint8_t minute, uint8_t second){
	LED_OFF_TIME->Hours = hour;
	LED_OFF_TIME->Minutes = minute;
	LED_OFF_TIME->Seconds = second;
}

void ledOnTimeGet(LED_Time_set *LED_ON_TIME, uint16_t *ledOnTime){
	char ledOnTimeString[10];
	sprintf(ledOnTimeString, "%d%02d", LED_ON_TIME->Hours, LED_ON_TIME->Minutes);
	*ledOnTime = atoi(ledOnTimeString);
}

void ledOffTimeGet(LED_Time_set *LED_OFF_TIME, uint16_t *ledOffTime){
	char ledOffTimeString[10];
	sprintf(ledOffTimeString, "%d%02d", LED_OFF_TIME->Hours, LED_OFF_TIME->Minutes);
	*ledOffTime = atoi(ledOffTimeString);
}

void getTime(RTC_TimeTypeDef *gTime, uint16_t *currentGetTime){
	char getTimeString[10];
	HAL_RTC_GetTime(&hrtc, gTime, RTC_FORMAT_BIN);
	sprintf(getTimeString, "%d%02d", gTime->Hours, gTime->Minutes);
	*currentGetTime = atoi(getTimeString);
}

void getDate(RTC_DateTypeDef *gDate, uint16_t *currentGetDate){
	char getDateString[10];
	HAL_RTC_GetDate(&hrtc, gDate, RTC_FORMAT_BIN);
	sprintf(getDateString, "%d%s%d%s%d", gDate->Year,"-",gDate->Month,"-",gDate->WeekDay);
	*currentGetDate = atoi(getDateString);
}

void getTimeStamp(RTC_TimeTypeDef *gTime , char *timeStamp){
	HAL_RTC_GetTime(&hrtc, gTime, RTC_FORMAT_BIN);
	sprintf(timeStamp,"%d%s%d%s%d",gTime->Hours,".",gTime->Minutes,".",gTime->Seconds);
}
