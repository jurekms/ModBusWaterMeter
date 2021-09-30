/*
 * coils.h
 *
 *  Created on: 15 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_COILS_H_
#define WATERMETER_COILS_H_

#include <avr/pgmspace.h>




/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmInitCoils(void);
void wmCoilsPoll(void);
void wmCoilsInit(void);





/******************************************************************************************************************************************************
 *
 * structure for WATER_METER_COILS, is use for store states of COILS which are set by 0x05 modbus function and state of STAE MACHINE which is control COILS in water meter - wmCoilStateChanged
 *
 *******************************************************************************************************************************************************/
typedef struct {
	uint8_t		wmCoilStateChanged;			 			//is SET when any coil has been changed - is use for COILS STATE MACHINE control
	uint16_t	wmCoilAdrress;							//address of coil which has been changed
	uint8_t		wmReadConfiguration;					//SET - reread configuration from EEPROM to RAM - without current water meter counter
	uint8_t		wmWriteConfiguration;					//SET - write configuration  from RAM to EEPROM - without current water meter counter
	uint8_t		wmReadCurrentCounter;					//SET - read current water meter counter from EEPROM - from configuration area in to RAM
	uint8_t		wmWriteCurrentCounter;					//SET - write current water meter counter from RAM in to EEPROM - to configuration area
	uint8_t		wmREDLed;								//SET - RED LED ON 										RESET - RED LED OFF - not function when RED LED blinking in alarm state
	uint8_t		wmGREENLed;								//SET - GREEN LED ON									RESET - GREEN LED OFF
	uint8_t		wmOUTTransistor;						//SET - transistor out SET ON is connected to ground !	RESET - transistor out SET OFF
	uint8_t		wmAlarmsReset;							//SET - RESET alarms counters in WATER_METER_COUNTER
} TCoils;


/******************************************************************************************************************************************************
 *
 * states of COILS STAE MACHINE when wmCoilStateChanged == WM_COILS_ST_CHANGE then state of coil which is mapping into correct field in WATER_METER_COILS GLOBAL VARIABLES triggers the corresponding action ex. SET GREEn LED OFF
 *
 *******************************************************************************************************************************************************/

#define			WM_COILS_ST_IDLE							0x00
#define			WM_COILS_ST_CHANGE							0x01


/******************************************************************************************************************************************************
 *
 * states of COILS
 *
 *******************************************************************************************************************************************************/


#define 		WM_COIL_IS_SET_BY_MODBUS					0xFF00  //coil is set to 0xFF00 by the modbus
#define			WM_COIL_SET									0x01
#define			WM_COIL_RESET								0x00




/******************************************************************************************************************************************************
 *
 *map of COILS
 *
 *******************************************************************************************************************************************************/

#define WM_COIL_READ_CONFIGURATION							10001		//SET - reread configuration from EEPROM to RAM - without current water meter counter
#define	WM_COIL_WRITE_CONFIGURATION							10002		//SET - write configuration  from RAM to EEPROM - without current water meter counter
#define	WM_COIL_READ_CURRENT_COUNTER						10003		//SET - read current water meter counter from EEPROM - from configuration area in to RAM
#define WM_COIL_WRITE_CURRENT_COUNTER						10004		//SET - write current water meter counter from RAM in to EEPROM - to configuration area
#define WM_COIL_RED_LED										10005		//SET - RED LED ON 										RESET - RED LED OFF - not function when RED LED blinking in alarm state
#define WM_COIL_GREEN_LED									10006		//SET - GREEN LED ON									RESET - GREEN LED OFF
#define	WM_COIL_OUT_TRANSISTOR								10007		//SET - transistor out SET ON is connected to ground !	RESET - transistor out SET OFF
#define WM_COIL_ALARMS_COUNTER_RESET						10008		//SET - RESET alarms counters in WATER_METER_COUNTER




/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES declarations , definitions i coils.cc
 *
 *******************************************************************************************************************************************************/

extern	volatile TCoils			WATER_METER_COILS;

#endif /* WATERMETER_COILS_H_ */
