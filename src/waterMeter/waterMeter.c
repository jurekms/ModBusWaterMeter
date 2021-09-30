/*
 * waterMeter.c
 *
 *  Created on: 1 kwi 2021
 *      Author: JUREK
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>




#include "waterMeter.h"
#include "registers.h"
#include "timers.h"


#include "../modbus/mb.h"

/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

volatile uint8_t			STATE;			// is using for sample state of input from IR module




/******************************************************************************************************************************************************
 *
 * initialize water meter environment,
 * after loading configuration from EEPROM checks if this is the first launch, if yes then load default configuration from PROGRAM MEMORY to RAM and store its as a actual configuration into EEPROM
 * prepare WATER_METER_COUNTER.waterMeter.wmCurrentCounter and WATER_METER_COUNTER.waterMeter.wmPulsesCounte as twice value of  WATER_METER_COUNTER.waterMeter.wmCurrentCounter
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmConfigurationInit(void) {

	wmReadConfigurationFromEEMEM();

	switch(CFG_RAMMEM.configuration.wmFirstTimeRunFlag) {

	case WM_FIRST_TIME_RUN_FLAG:
		break;
	default:
		memcpy_P( &CFG_RAMMEM, &CFG_PROGMEM, sizeof( TConfiguration ) );
		CFG_RAMMEM.configuration.wmFirstTimeRunFlag = WM_FIRST_TIME_RUN_FLAG;
		wmWriteConfigurationToEEMEM();
		break;

	}
	WATER_METER_COUNTER.waterMeter.wmCurrentCounter = CFG_RAMMEM.configuration.wmCurrentCounter;
	WATER_METER_COUNTER.waterMeter.wmPulsesCounter = (CFG_RAMMEM.configuration.wmCurrentCounter << 1) - 1; //multiple wmCurrentCounter and -1 becouse off first probing IMP port
}




/******************************************************************************************************************************************************
 *
 *initialize debouncer which prevents erroneous readings due to contact interference and vibration when using a mechanical transducer
 *and setup imput PIN from IR module
 *and
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/


void wmDebouncerInit( void ) {
	OCR2A = 116;											//wielkosc do porównania liczymy od 0-do OCR2 10ms przy 12MHz
    TCCR2A |= (1 << WGM21);								//ustaw CTC mode;

    TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22);			// ustaw preskaler na 1024
    TIMSK2 |= (1 << OCIE2A);								// ustaw zezwolenie na przerwania od CTC2 (porównania

    SET_IMP_AS_IN_PIN;												//sets as IN PIN
    PULL_UP_IMP_PIN;												//PULL UP VCC

}




/******************************************************************************************************************************************************
 *
 * function that supports interrupts from TIMER 2
 *
 *sample and debounce input
 *incremennts all of timers every 10 ms
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

ISR( TIMER2_COMPA_vect ) {

	    static uint8_t  cnt0, cnt1;
	    uint8_t sample, toggle, delta;

	    sample =  (PINB & IMP_PIN) ;


	    delta = sample ^ STATE;
	    cnt1 = (cnt1 ^ cnt0) & delta;
	    cnt0 = ~cnt0 & delta;

	    toggle = delta & ~(cnt0 | cnt1);
	    STATE ^= toggle;

	    WATER_METER_COUNTER.waterMeter.wmPulsesCounter = WATER_METER_COUNTER.waterMeter.wmPulsesCounter + ((toggle>>2)  & 0x01);
	    WATER_METER_COUNTER.waterMeter.wmCurrentCounter = WATER_METER_COUNTER.waterMeter.wmPulsesCounter >> 1;

	    WM_TIMERS.LEDGreenTimer++;
	    WM_TIMERS.LEDRedTimer++;
	    WM_TIMERS.wmContinuousWaterFlowPeriodTimer++;
	    WM_TIMERS.wmNoWaterFlowTimer++;

}














