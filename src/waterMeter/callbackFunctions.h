/*
 * callbackFunctions.h
 *
 *  Created on: 13 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_CALLBACKFUNCTIONS_H_
#define WATERMETER_CALLBACKFUNCTIONS_H_



/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

uint8_t wmCallbackReadHoldingRegisters( void );
uint8_t wmCallbackWriteHoldingRegisters( void );
uint8_t wmCallbackReadInputRegisters( void );
uint8_t wmCallbacWriteCoil( void );





/******************************************************************************************************************************************************
 *
 * some defines
 *
 *******************************************************************************************************************************************************/

#define				CONFIGURATION_REGS_ADR_OFFSET		40001				//registers address offset
#define				CONFIGURATION_REGS_ADR_START		40001				//register start address
#define				CONFIGURATION_REGS_ADR_END			40032				//register end address

/******************************************************************************************************************************************************
 *
 * modbus map of water meter configuration registers store in GLOBAL VARIA|BLES : CFG_PROGMEM-for first use, CFG_RAMMEM - for program use, CFG_EEMEM - for story
 *
 *******************************************************************************************************************************************************/

/*
 * read configuration function 0x03 read holding registers
 *
 * 40001 - 40008		wmLabel (16 bytes)					- 8 registers			//water meter label ex. Lokal nr 1 - max 16 chars
 * 40009 - 40010		wmSerialNumber 						- 2 registers			//water meter serial number only numbers !
 * 40011				wmSoftVersion						- 1 register			//software version
 * 40012 - 40013		wmCorrectionCounter					- 2 registers			//number for add or sub from current counter in order to keep continuously count water flow ex. after water meter exchange- for use by external system
 * 40014 - 40015		wmInitialCounter					- 2 registers			//initial counter value
 * 40016 - 40017		wmStoreCounter						- 2 registers			//store counter ex. to keep some add information - for use by external system
 * 40018 - 40019		wmCurrentCounter					- 2 registers			//current counter - not actual counter ! in this register you can store current counter value
 * 40020 - 40021		wmPulseCounter						- 2 registers			//pulse counter - not actual pulse counter ! in this register you can store pulse counter value wmPulseCounter = 2 x wmCurrentCounter +/-1
 * 40022				wmCheckYear							- 1 register			//in this year you have to exchange water meter (in Poland every each 5 years)
 * 40023				wmFirstTimeRunFlag					- 1 register			//is set to 0xAA before first run - is use to set initial configuration after do this is set to 0x00
 * 40024				wmModbusSlaveAddress				- 1 register			//modbus SLAVE address default is set to 252
 * 40025				wmContinuousWaterFlowAlarmAllow		- 1 register			//if is SET (1) - continuously flow water alarm can be invoke
 * 40026				wmContinuousWaterFlowAlarmTimes 	- 1 register			//numbers of periods continuously flow water before continuously flow water alarm can be invoke
 * 40027 - 40028		wmContinuousWaterFlowAlarmPeriod	- 2 registers			//time continuously flow water of one period 1 = 10 ms => 6000 = 1 minute
 * 40029				wmNoWaterFlowAlarmAllow				- 1 register			//if is SET (1) - no flow water alarm can be invoke
 * 40030 - 40031		wmNoWaterFlowAlarmTime				- 2 registers			//time no flow water of one period 1 = 10 ms => 6000 = 1 minute => 8 640 000 = 24h
 * 40032				wmOUTTransistorAlarmAllow;			- 1 register			//if == 0x0001 any alarm sets the transistor outlet to set permanently , if == 0x002 any alarm sets the transistor display to SET only for the alarm time, if == 0 disable
 */



/******************************************************************************************************************************************************
 *
 * some defines
 *
 *******************************************************************************************************************************************************/

#define				WATER_METER_REGS_ADR_OFFSET		30001      //registers address offset
#define				WATER_METER_REGS_ADR_START		30001      //register start address
#define				WATER_METER_REGS_ADR_END		30006      //register end address

/******************************************************************************************************************************************************
 *
 * modbus map of water meter counter registers store in GLOBAL VARIA|BLE WATER_METER_COUNTER
 *
 *******************************************************************************************************************************************************/
/*
* 30001 - 30002		wmCurrentCounter						- 2 registers			//actual water meter counter sample every 20ms
* 30003 - 30004		wmPulseCounter							- 2	register			//actual pulses number ~ 2 x wmCurrentCounter +/-1
* 30005				wmContinuousWaterFlowAlarmFlag			- 1 register			//numbers of continuously flow water alarms - it can be reset by set COIL wmAlarmsReset
* 30006				wmNoWaterFlowAlarmFlag					- 1 register			//numbers of no flow water alarms - it can be reset by set COIL wmAlarmsRese
*/




#endif /* WATERMETER_CALLBACKFUNCTIONS_H_ */
