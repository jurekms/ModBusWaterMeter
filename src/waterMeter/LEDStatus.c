/*
 * LEDStatus.c
 *
 *  Created on: 1 kwi 2021
 *      Author: JUREK
 */

#include "LEDStatus.h"
#include "coils.h"
#include "timers.h"


/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

volatile TLED	WM_RED_LED;
volatile TLED	WM_GREEN_LED;



/******************************************************************************************************************************************************
 *
 * initialize LEDs and BLINKING STATE MACHINE
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmLEDSInit(void) {
	RED_LED_CONFIG;
	GREEN_LED_CONFIG;
	WM_RED_LED.blinkStatus = LED_BLINKING_COMPLETE;
	WM_GREEN_LED.blinkStatus = LED_BLINKING_COMPLETE;
	RED_LED_OFF;
	GREEN_LED_OFF;
}




/******************************************************************************************************************************************************
 *
 *set parameters for BLINKING STATE MACHINE for RED LED
 *
 *@param	void
 *
 *@return	uint8_t speed (speed of blinking LED), uint8_t times (number of blinkings)
 *
 *******************************************************************************************************************************************************/

void wmRedLEDBlink(uint8_t speed, uint8_t times) {
	WM_RED_LED.blinkSpeed = speed;
	WM_RED_LED.blinkTimes = times*2;
	WM_RED_LED.blinkStatus = LED_BLINKING_START;
	WM_RED_LED.blinkIdle = 4 * speed;
}



/******************************************************************************************************************************************************
 *
 *set parameters for BLINKING STATE MACHINE for GREEN LED
 *
 *@param	void
 *
 *@return	uint8_t speed (speed of blinking LED), uint8_t times (number of blinkings)
 *
 *******************************************************************************************************************************************************/

void wmGreenLEDBlink(uint8_t speed, uint8_t times) {
	WM_GREEN_LED.blinkSpeed = speed;
	WM_GREEN_LED.blinkTimes = times*2;
	WM_GREEN_LED.blinkStatus = LED_BLINKING_START;
	WM_GREEN_LED.blinkIdle = 4 * speed;
}




/******************************************************************************************************************************************************
 *
 *BLINKING STATE MACHINE for both green and RED LEDs
 *
  *@param	void
 *
 *@return	uint8_t speed (speed of blinking LED), uint8_t times (number of blinkings)
 *
 *******************************************************************************************************************************************************/

void wmLEDPoll(void) {
	switch(WM_RED_LED.blinkStatus) {

	case LED_BLINKING_START:
		RED_LED_OFF;
		WM_RED_LED.blinkCounter = 0;
		WM_RED_LED.blinkStatus = LED_BLINKING_IN_PROGRESS;
		break;

	case LED_BLINKING_IN_PROGRESS:
		if(WM_RED_LED.blinkCounter == 0) {
			RED_LED_ON;
			WM_TIMERS.LEDRedTimer = 0;
			WM_RED_LED.blinkCounter ++;
		}
		if(WM_RED_LED.blinkTimes-WM_RED_LED.blinkCounter > 0) {
			if(WM_TIMERS.LEDRedTimer > WM_RED_LED.blinkSpeed) {
				RED_LED_CHANGE;
				WM_TIMERS.LEDRedTimer = 0;
				WM_RED_LED.blinkCounter ++;
			}
		}else {
			if(WM_TIMERS.LEDRedTimer >  WM_RED_LED.blinkIdle) {
				WM_TIMERS.LEDRedTimer = 0;
				WM_RED_LED.blinkCounter = 0;
				RED_LED_OFF;
				WM_RED_LED.blinkStatus = LED_BLINKING_COMPLETE;
			}
		}
		break;

	case LED_BLINKING_COMPLETE:
		if(WATER_METER_COILS.wmREDLed) {
			RED_LED_ON;
		}else RED_LED_OFF;
		WM_RED_LED.blinkStatus = LED_BLINKING_IDLE;
		break;


	case LED_BLINKING_IDLE:
		break;
	};

	switch(WM_GREEN_LED.blinkStatus) {

	case LED_BLINKING_START:
		GREEN_LED_OFF;
		WM_GREEN_LED.blinkCounter = 0;
		WM_GREEN_LED.blinkStatus = LED_BLINKING_IN_PROGRESS;
		break;

		case LED_BLINKING_IN_PROGRESS:
			if(WM_GREEN_LED.blinkCounter == 0) {
				GREEN_LED_ON;
				WM_TIMERS.LEDGreenTimer = 0;
				WM_GREEN_LED.blinkCounter ++;
			}
			if(WM_GREEN_LED.blinkTimes-WM_GREEN_LED.blinkCounter > 0) {
				if(WM_TIMERS.LEDGreenTimer > WM_GREEN_LED.blinkSpeed) {
					GREEN_LED_CHANGE;
					WM_TIMERS.LEDGreenTimer = 0;
					WM_GREEN_LED.blinkCounter ++;
				}
			}else {
				if(WM_TIMERS.LEDGreenTimer >  WM_GREEN_LED.blinkIdle) {
					WM_TIMERS.LEDGreenTimer = 0;
					WM_GREEN_LED.blinkCounter = 0;
					GREEN_LED_OFF;
					WM_GREEN_LED.blinkStatus = LED_BLINKING_COMPLETE;
				}
			}
		break;

		case LED_BLINKING_COMPLETE:
			if(WATER_METER_COILS.wmGREENLed) {
				GREEN_LED_ON;
			}else GREEN_LED_OFF;

			WM_GREEN_LED.blinkStatus = LED_BLINKING_IDLE;
			break;


		case LED_BLINKING_IDLE:
		break;
	}


}
