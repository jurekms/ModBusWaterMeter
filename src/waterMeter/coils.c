/*
 * coils.c
 *
 *  Created on: 15 kwi 2021
 *      Author: JUREK
 */
#include "waterMeter.h"
#include "coils.h"
#include "registers.h"
#include "LEDStatus.h"
#include "alarms.h"
#include "OUTTransistor.h"
#include "../modbus/mb.h"



/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

volatile TCoils			WATER_METER_COILS;




/******************************************************************************************************************************************************
 *
 * initialize COILS of RED and GREEN LEDs
  *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmCoilsInit(void) {
	WATER_METER_COILS.wmGREENLed = WM_COIL_SET;
	WATER_METER_COILS.wmREDLed = WM_COIL_RESET;
}



/******************************************************************************************************************************************************
 *
 * STAE MACHINE of COILS, it checks if the WATER_METER_COILS.wmCoilStateChanged and triggers the corresponding action
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmCoilsPoll(void) {

	if(WATER_METER_COILS.wmCoilStateChanged) {

		switch(WATER_METER_COILS.wmCoilAdrress) {

		case WM_COIL_READ_CONFIGURATION: {
			if(WATER_METER_COILS.wmReadConfiguration) {
				wmReadConfigurationFromEEMEM();
				WATER_METER_COILS.wmReadConfiguration = WM_COIL_RESET;
			}
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_WRITE_CONFIGURATION: {
			if( WATER_METER_COILS.wmWriteConfiguration) {
				 wmWriteConfigurationToEEMEM();
				 WATER_METER_COILS.wmWriteConfiguration = WM_COIL_RESET;
			}
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_READ_CURRENT_COUNTER: {
			if(WATER_METER_COILS.wmReadCurrentCounter) {
				wmReadCurrentCounterFromEEMEM();
				WATER_METER_COILS.wmReadCurrentCounter = WM_COIL_RESET;
			}
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_WRITE_CURRENT_COUNTER: {
			if(WATER_METER_COILS.wmWriteCurrentCounter) {
				wmWriteCurrentCounterToEEMEM();
				WATER_METER_COILS.wmWriteCurrentCounter = WM_COIL_RESET;
			}
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_RED_LED: {
			WM_RED_LED.blinkStatus = LED_BLINKING_COMPLETE;
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_GREEN_LED: {
			WM_GREEN_LED.blinkStatus = LED_BLINKING_COMPLETE;
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_OUT_TRANSISTOR: {
			if(WATER_METER_COILS.wmOUTTransistor) {
				OUT_TRANSISTOR_ON;
			} else {
				OUT_TRANSISTOR_OFF;
			}

			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		case WM_COIL_ALARMS_COUNTER_RESET: {
			if(WATER_METER_COILS.wmAlarmsReset) {
				wmAlarmsInit();
				OUT_TRANSISTOR_OFF;
				WATER_METER_COILS.wmAlarmsReset = WM_COIL_RESET;
			}
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}

		default: {
			break;
		}

		}

	}


}
