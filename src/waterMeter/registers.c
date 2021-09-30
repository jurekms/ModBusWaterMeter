/*
 * registers.c
 *
 *  Created on: 25 kwi 2021
 *      Author: JUREK
 */
#include <avr/pgmspace.h>

#include "registers.h"




/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

const		TConfiguration 	CFG_PROGMEM PROGMEM = {		WM_LABEL,
														WM_SERIAL_NUMBER,
														WM_SOFT_VERSION,
														WM_CORRECTION_COUNTER,
														WM_INITIAL_COUNTER,
														WM_STORE_COUNTER,
														WM_CURRENT_COUNTER,
														WM_PULSE_COUNTER,
														WM_CHECK_YEAR,
														WM_FIRST_TIME_RUN_FLAG ,
														WM_MODBUS_ADDRESS,
														WM_CONTINUOUS_WATER_FLOW_ALARM_ALLOW,
														WM_CONTINUOUS_WATER_FLOW_ALARM_TIMES,
														WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD,
														WM_NO_WATER_FLOW_ALARM_ALLOW,
														WM_NO_WATER_FLOW_ALARM_TIME,
														WM_OUT_TRANSISTOR_ALARM_ALLOW
													} ;											//store default configuration in PROGRAM memory

			TUConfiguration	CFG_EEMEM EEMEM;													//store actual configuration in EEPROM memory
			TUConfiguration	CFG_RAMMEM;															//store actual configuration in RAM memory - this configuration is accessible by modbus READ/WRITE Holding Register functions (0x03, 0x10)

volatile 	TUWaterMeter	WATER_METER_COUNTER;												//store actual value of water meter counter and alarms states in RAM memory - is accessible by modbus READ input Register functions (0x04)




/******************************************************************************************************************************************************
 *
 *read configuration from EEPROM memory and write in to RAM memory, CFG_EEMEM -> CFG_RAMMEM
 *read every start water meter or after SET wmReadConfiguration COIL
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmReadConfigurationFromEEMEM(void) {
	eeprom_read_block( &CFG_RAMMEM, &CFG_EEMEM, sizeof( TUConfiguration ) );
}




/******************************************************************************************************************************************************
 *
 *write configuration from RAM memory in to EEPROM memory, CFG_RAMMEM -> CFG_EEMEM
 *write after SET wmReadConfiguration COIL
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmWriteConfigurationToEEMEM(void) {
	eeprom_write_block(&CFG_RAMMEM, &CFG_EEMEM, sizeof( TUConfiguration ));
}




/******************************************************************************************************************************************************
 *
 *read water meter counter from EEPROM memory and write in to RAM memory, CFG_EEMEM.configuration.wmCurrentCounter -> WATER_METER_COUNTER.waterMeter.wmCurrentCounter and (2x) WATER_METER_COUNTER.waterMeter.wmPulsesCounter
 *read every start water meter or after SET wmReadCurrentCounter COIL
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmReadCurrentCounterFromEEMEM(void) {
	uint32_t currentCounter, pulseCounter;
	eeprom_read_block( &currentCounter, &(CFG_EEMEM.configuration.wmCurrentCounter), sizeof( uint32_t ));
	eeprom_read_block( &pulseCounter, &(CFG_EEMEM.configuration.wmPulseCounter), sizeof( uint32_t ));
	WATER_METER_COUNTER.waterMeter.wmCurrentCounter = currentCounter;
	WATER_METER_COUNTER.waterMeter.wmPulsesCounter = pulseCounter;
}




/******************************************************************************************************************************************************
 *
 *write water meter counter from RAM memory and write in to EEPROM memory,  WATER_METER_COUNTER.waterMeter.wmCurrentCounter -> CFG_EEMEM.configuration.wmCurrentCounter
 *write after SET wmWriteCurrentCounter COIL
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/
void wmWriteCurrentCounterToEEMEM(void) {
	uint32_t currentCounter, pulseCounter;
	currentCounter = WATER_METER_COUNTER.waterMeter.wmCurrentCounter;
	pulseCounter = WATER_METER_COUNTER.waterMeter.wmPulsesCounter;
	eeprom_write_block( &currentCounter, &(CFG_EEMEM.configuration.wmCurrentCounter), sizeof( uint32_t ));
	eeprom_write_block( &pulseCounter, &(CFG_EEMEM.configuration.wmPulseCounter), sizeof( uint32_t ));

}







