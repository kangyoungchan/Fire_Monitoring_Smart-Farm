/*
 * Display.c
 *
 *  Created on: Aug 15, 2025
 *      Author: sooju
 */

#include "Display.h"

uint8_t displayState = 0;
uint8_t lastDisplayState = 100;

void displayFireSensorDetect() {
	displayState = 0;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, "Fire Detect!!!");

	lastDisplayState = displayState;
}

void displaySmokeSenosrDetect() {
	displayState = 1;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, "Smoke Detect!!!");

	lastDisplayState = displayState;
}

void displayFire_SmokeSensorDetect() {
	displayState = 2;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, "Fire Detect!!!");
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	I2C_LCD_WriteString(I2C_LCD_1, "Smoke Detect!!!");

	lastDisplayState = displayState;

}

void displaySensorData(char Temp[], char Humidty[], char Moisture[], uint8_t mode) {
	static uint8_t lastMode = 0;
	static uint8_t last_temp = 0;
	static uint8_t last_humidty = 0;
	static uint8_t last_moisture = 0;
	static uint8_t tempData = 0;
	static uint8_t humidtyData = 0;
	static uint8_t moistureData = 0;
	char Temp_text[15] = "";
	char Humidty_text[15] ="";
	char Moisture_text[15] ="";

	displayState = 3;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}

	tempData = atoi(Temp);
	humidtyData = atoi(Humidty);
	moistureData = atoi(Moisture);

	if (tempData < 10 && last_temp >= 10) {
		I2C_LCD_Clear(I2C_LCD_1);
	} else if (tempData < 100 && last_temp >= 100) {
		I2C_LCD_Clear(I2C_LCD_1);
	}
	if (humidtyData < 10 && last_humidty >= 10) {
		I2C_LCD_Clear(I2C_LCD_1);
	} else if (humidtyData < 100 && last_humidty >= 100) {
		I2C_LCD_Clear(I2C_LCD_1);
	}
	if (moistureData < 10 && last_moisture >= 10) {
		I2C_LCD_Clear(I2C_LCD_1);
	} else if (moistureData < 100 && last_moisture >= 100) {
		I2C_LCD_Clear(I2C_LCD_1);
	}
	if (mode != lastMode) {
		I2C_LCD_Clear(I2C_LCD_1);
	}

	sprintf(Temp_text,"%s%s%s", "T:", Temp, "C");
	sprintf(Humidty_text,"%s%s%s", "H:", Humidty, "%");
	sprintf(Moisture_text,"%s%s%s", "M:", Moisture, "%");


	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, Temp_text);
	I2C_LCD_SetCursor(I2C_LCD_1, 8, 0);
	I2C_LCD_WriteString(I2C_LCD_1, Humidty_text);
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	I2C_LCD_WriteString(I2C_LCD_1, Moisture_text);

	if (mode == 0) {
		I2C_LCD_SetCursor(I2C_LCD_1, 8, 1);
		I2C_LCD_WriteString(I2C_LCD_1, "AUTO");
	} else {
		I2C_LCD_SetCursor(I2C_LCD_1, 8, 1);
		I2C_LCD_WriteString(I2C_LCD_1, "CON");
	}

	last_temp = tempData;
	last_humidty = humidtyData;
	last_moisture = moistureData;
	lastMode = mode;
	lastDisplayState = displayState;

}

void displayLedTime(LED_Time_set *LED_ON_TIME , LED_Time_set *LED_OFF_TIME){
	displayState = 4;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}

	char ledOnTime[15] = "";
	char ledOffTime[15] = "";

	sprintf(ledOnTime,"%s %02d%s%02d", "ON: ", LED_ON_TIME->Hours, ":", LED_ON_TIME->Minutes);
	sprintf(ledOffTime,"%s%02d%s%02d", "OFF: ", LED_OFF_TIME->Hours, ":", LED_OFF_TIME->Minutes);

	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, ledOnTime);
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	I2C_LCD_WriteString(I2C_LCD_1,ledOffTime);

	lastDisplayState = displayState;
}


void displayTargetValue(uint8_t tempTargetValue , uint8_t humidityTargetValue, uint8_t moistureTargetValue){
	displayState = 5;
	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}

	char tempTarget[15] = "";
	char humidityTarget[15] = "";
    char moistureTarget[15] = "";

	sprintf(tempTarget, "%s%d%s", "T->", tempTargetValue, "C");
	sprintf(humidityTarget, "%s%d%s", "H->:", humidityTargetValue, "%");
	sprintf(moistureTarget, "%s%d%s", "M->:", moistureTargetValue, "%");

	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	I2C_LCD_WriteString(I2C_LCD_1, tempTarget);
	I2C_LCD_SetCursor(I2C_LCD_1, 8, 0);
	I2C_LCD_WriteString(I2C_LCD_1, humidityTarget);
	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	I2C_LCD_WriteString(I2C_LCD_1, moistureTarget);

	lastDisplayState = displayState;
}

void displaySuccess(uint8_t state){
	switch(state){
	    case 6:
	    	displayState = state;
	    	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	    	I2C_LCD_WriteString(I2C_LCD_1, "MODE CHANGE");
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	lastDisplayState = displayState;
	    	HAL_Delay(1000);
	    	break;
	    case 7:
	    	displayState = state;
	    	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	        I2C_LCD_WriteString(I2C_LCD_1, "LED CHANGE");
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	lastDisplayState = displayState;
	    	HAL_Delay(1000);
	    	break;
	    case 8:
	    	displayState = state;
	    	if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	    	I2C_LCD_WriteString(I2C_LCD_1, "FAN CHANGE");
	    	I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	lastDisplayState = displayState;
	    	HAL_Delay(1000);
	    	break;

	    case 9:
	    	 displayState = state;
	    	 if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "WATER CHANGE");
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	 lastDisplayState = displayState;
	    	 HAL_Delay(1000);
	    	 break;

	    case 10:
	    	 displayState = state;
	    	 if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "LED Time Set");
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	 lastDisplayState = displayState;
	    	 HAL_Delay(1000);
	    	 break;
	    case 11:
	    	 displayState = state;
	    	 if(displayState != lastDisplayState){I2C_LCD_Clear(I2C_LCD_1);}
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "Target Set");
	    	 I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
	    	 I2C_LCD_WriteString(I2C_LCD_1, "SUCCESS!");
	    	 lastDisplayState = displayState;
	    	 HAL_Delay(1000);
	    	 break;

	}



}
