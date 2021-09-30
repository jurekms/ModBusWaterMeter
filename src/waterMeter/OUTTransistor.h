/*
 * OUTtransistor.h
 *
 *  Created on: 22 wrz 2021
 *      Author: JUREK
 */
#include <avr/pgmspace.h>

#ifndef SRC_WATERMETER_OUTTRANSISTOR_H_
#define SRC_WATERMETER_OUTTRANSISTOR_H_




/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmOUTTransisitorInit(void);


/******************************************************************************************************************************************************
 *
 * macros use in order to set configure out port which is use for transistor control
 *
 *******************************************************************************************************************************************************/

#define OUT_TRANSISTOR_PIN 		(1<<PB0)
#define OUT_TRANSISTOR_CONFIG	DDRB |= (OUT_TRANSISTOR_PIN )
#define OUT_TRANSISTOR_ON		PORTB |= OUT_TRANSISTOR_PIN
#define OUT_TRANSISTOR_OFF		PORTB &= ~OUT_TRANSISTOR_PIN
#define OUT_TRANSISTOR_CHANGE 	PORTB ^= OUT_TRANSISTOR_PIN




#endif /* SRC_WATERMETER_OUTTRANSISTOR_H_ */
