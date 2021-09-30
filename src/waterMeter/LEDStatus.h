/*
 * LEDStatus.h
 *
 *  Created on: 1 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_LEDSTATUS_H_
#define WATERMETER_LEDSTATUS_H_

#include <avr/pgmspace.h>
#include "coils.h"


/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmLEDSInit(void);
void wmRedLEDBlink(uint8_t speed, uint8_t times);
void wmGreenLEDBlink(uint8_t speed, uint8_t times);
void wmLEDPoll(void);




/******************************************************************************************************************************************************
 *
 * macros use in order to set configure ports for LEDs, and LED status
 *
 *******************************************************************************************************************************************************/

#define RED_LED_PIN 			(1<<PD6)
#define RED_LED_CONFIG			DDRD |= (RED_LED_PIN)
#define RED_LED_ON 				PORTD &= ~RED_LED_PIN
#define RED_LED_OFF 			PORTD |= RED_LED_PIN
#define RED_LED_CHANGE 			PORTD ^= RED_LED_PIN

#define GREEN_LED_PIN 			(1<<PD5)
#define GREEN_LED_CONFIG		DDRD |= (GREEN_LED_PIN)
#define GREEN_LED_ON 			PORTD &= ~GREEN_LED_PIN
#define GREEN_LED_OFF 			PORTD |= GREEN_LED_PIN
#define GREEN_LED_CHANGE 		PORTD ^= GREEN_LED_PIN

#define LED_ST_OFF						0x00
#define LED_ST_ON						0x01



/******************************************************************************************************************************************************
 *
 * macros use in order to set configure ports for LED's STATE MACHINE
 *
 *******************************************************************************************************************************************************/

#define LED_BLINKING_START			0x21
#define	LED_BLINKING_IN_PROGRESS	0x22
#define LED_BLINKING_COMPLETE		0x23
#define	LED_BLINKING_IDLE			0x10



/******************************************************************************************************************************************************
 *
 * struct for LED's blinking STATE MACHINE
 *
 *******************************************************************************************************************************************************/

typedef struct {
	uint8_t		LEDState;
	uint8_t		blinkStatus;
	uint8_t		blinkCounter;
	uint16_t	blinkSpeed;
	uint8_t		blinkTimes;
	uint16_t	blinkIdle;
} TLED;


/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES declarations , definitions in LEDStatus.c
 *
 *******************************************************************************************************************************************************/

extern	volatile TLED	WM_RED_LED;
extern	volatile TLED	WM_GREEN_LED;



#endif /* WATERMETER_LEDSTATUS_H_ */
