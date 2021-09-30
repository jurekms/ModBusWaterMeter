/*
 * main.c
 *
 *  Created on: 1 kwi 2021
 *      Author: JUREK
 */


#include <avr/io.h>
#include <avr/interrupt.h>

#include "./modbus/mb.h"

#include "waterMeter/waterMeter.h"
#include "waterMeter/LEDStatus.h"
#include "waterMeter/coils.h"
#include "waterMeter/OUTTransistor.h"
#include "waterMeter/alarms.h"
#include "waterMeter/timers.h"
#include "waterMeter/registers.h"




int main(void) {

	/******************************************************************************************************************************************************
	 *
	 * initialize all of modules
	 *
	 *******************************************************************************************************************************************************/

	wmConfigurationInit();
	mbInit(CFG_RAMMEM.configuration.wmModbusSlaveAddress);
	wmDebouncerInit();
	wmAlarmsInit();
	wmLEDSInit();
	wmOUTTransisitorInit();
	wmCoilsInit();
	wmTimersInit();

	/******************************************************************************************************************************************************
	 *
	 * start interrupts
	 *
	 *******************************************************************************************************************************************************/

	sei();


	/******************************************************************************************************************************************************
	 *
	 * start STATE MACINEs
	 *
	 *******************************************************************************************************************************************************/

	while(1) {

		mbPoll();

		wmLEDPoll();

		wmCoilsPoll();

		wmAlarmsPoll();


	}

	return 0;
}
