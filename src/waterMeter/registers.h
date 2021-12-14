/*
 * registers.h
 *
 *  Created on: 25 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_REGISTERS_H_
#define WATERMETER_REGISTERS_H_
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmReadConfigurationFromEEMEM(void);
void wmWriteConfigurationToEEMEM(void);
void wmReadCurrentCounterFromEEMEM(void);
void wmWriteCurrentCounterToEEMEM(void);



/******************************************************************************************************************************************************
 *
 * structure for CFG_RAMMEM CFG_EEMEM CFG_PROGMEM GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

typedef struct {
	char		wmLabel[ 16 ];                         	//water meter label ex. Lokal nr 1 - max 16 chars
	uint32_t	wmSerialNumber;                        	//water meter serial number only numbers !
	uint16_t	wmSoftVersion;                        	//software version
	uint32_t	wmCorrectionCounter;                  	//number for add or sub from current counter in order to keep continuously count water flow ex. after water meter exchange- for use by external system
	uint32_t	wmInitialCounter;                      	//initial counter value
	uint32_t	wmStoreCounter;                        	//store counter ex. to keep some add information - for use by external system
	uint32_t	wmCurrentCounter;                      	//current counter - not actual counter ! in this register you can store current counter value
	uint32_t	wmPulseCounter;							//pulse counter - not actual pulse counter ! in this register you can store pulse counter value wmPulseCounter = 2 x wmCurrentCounter +/-1
	uint16_t	wmCheckYear;                           	//in this year you have to exchange water meter (in Poland every each 5 years)
	uint16_t	wmFirstTimeRunFlag;                    	//if it equal to 0xAA it means that correct configuration is store in EPROM memory, if you want load default configuration from PROGRAM memory set it to another value
	uint16_t	wmModbusSlaveAddress;                  	//modbus SLAVE address default is set to 252
	uint16_t	wmContinuousWaterFlowAlarmAllow;		//if is SET (1) - continuously flow water alarm can be invoke
	uint16_t	wmContinuousWaterFlowAlarmTimes;        //numbers of periods continuously flow water before continuously flow water alarm can be invoke
	uint32_t	wmContinuousWaterFlowAlarmPeriod;		//time continuously flow water of one period 1 = 10 ms => 6000 = 1 minute
	uint16_t	wmNoWaterFlowAlarmAllow;				//if is SET (1) - no flow water alarm can be invoke
	uint32_t	wmNoWaterFlowAlarmTime;					//time no flow water of one period 1 = 10 ms => 6000 = 1 minute => 8 640 000 = 24h
	uint16_t	wmOUTTransistorAlarmAllow;				//if == 0x0001 any alarm sets the transistor outlet to set permanently , if == 0x002 any alarm sets the transistor outlet to set only for the alarm time, if == 0 disable
} TConfiguration;

typedef union {
	uint16_t		dataUint16[ 32 ];					//number of 16 bits registers
	TConfiguration	configuration;
} TUConfiguration;


/******************************************************************************************************************************************************
 *
 * default configuration for water meter store in CFG_PROGMEM in program memory read only once during first start.
 *
 *******************************************************************************************************************************************************/

#define		WM_LABEL								"WaterMeter LABEL"
#define		WM_SERIAL_NUMBER						70211805
#define 	WM_SOFT_VERSION							1012
#define		WM_CORRECTION_COUNTER					0
#define 	WM_INITIAL_COUNTER						0
#define 	WM_STORE_COUNTER						200
#define		WM_CURRENT_COUNTER						200
#define 	WM_PULSE_COUNTER						2 * WM_CURRENT_COUNTER
#define		WM_CHECK_YEAR							2025
#define		WM_FIRST_TIME_RUN_FLAG					0xAA
#define		WM_MODBUS_ADDRESS						0x10
#define 	WM_CONTINUOUS_WATER_FLOW_ALARM_ALLOW	1
#define		WM_CONTINUOUS_WATER_FLOW_ALARM_TIMES	10
#define		WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD	6000
#define		WM_NO_WATER_FLOW_ALARM_ALLOW			1
#define 	WM_NO_WATER_FLOW_ALARM_TIME				8640000
#define 	WM_OUT_TRANSISTOR_ALARM_ALLOW			2


/******************************************************************************************************************************************************
 *
 * structure for WATER_METER_COUNTER GLOBAL VARIABLE
 *
 *******************************************************************************************************************************************************/

typedef struct {
	uint32_t	wmCurrentCounter;                       //actual water meter counter sample every 20ms
	uint32_t	wmPulsesCounter;                        //actual pulses number ~ 2 x wmCurrentCounter +/-1
	uint16_t	wmContinuousWaterFlowAlarmFlag;			//numbers of continuously flow water alarms - it can be reset by set COIL wmAlarmsReset
	uint16_t	wmNoWaterFlowAlarmFlag;					//numbers of no flow water alarms - it can be reset by set COIL wmAlarmsRese
} TWaterMeter ;

typedef union {
	uint16_t	dataUint16[ 6 ];						//number of 16 bits registers
	TWaterMeter	waterMeter;
} TUWaterMeter;


/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES declarations , definitions in registers.c
 *
 *******************************************************************************************************************************************************/
extern					TUConfiguration			CFG_EEMEM EEMEM;
extern					TUConfiguration			CFG_RAMMEM;
extern 		const 		TConfiguration 			CFG_PROGMEM PROGMEM;
extern		volatile 	TUWaterMeter			WATER_METER_COUNTER;



#endif /* WATERMETER_REGISTERS_H_ */
