/*
 * WaterMeter.h
 *
 *  Created on: 1 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_WATERMETER_H_
#define WATERMETER_WATERMETER_H_


#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <string.h>


/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmConfigurationInit(void);
void wmDebouncerInit( void );




/******************************************************************************************************************************************************
 *
 * macros use in order to set configure out port which is use for input from IR module 2 impulses for every 1 liter water
 *
 *******************************************************************************************************************************************************/



#define IMP_PIN							(1<<PB2)
#define SET_IMP_AS_IN_PIN				DDRB &= ~(IMP_PIN); 	// jako wejscie
#define PULL_UP_IMP_PIN					PORTB |= IMP_PIN ; 		//pull up






#endif /* WATERMETER_WATERMETER_H_ */

