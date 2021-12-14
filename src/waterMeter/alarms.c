/*
 * alarms.c
 *
 *  Created on: 15 kwi 2021
 *      Author: JUREK
 */
#include <avr/pgmspace.h>

#include "registers.h"
#include "alarms.h"
#include "waterMeter.h"
#include "LEDStatus.h"
#include "timers.h"
#include "OUTTransistor.h"
#include "../modbus/mb.h"


/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *******************************************************************************************************************************************************/

volatile uint8_t WATER_METER_IS_WATER_FLOW_FLAG;
uint32_t WM_CURRENT_COUNTER_CONTINUOUSLY_TMP;					//temporary of actual water meter counter use to identify continuously flowing water
uint32_t WM_ALARM_LED;											//state of blinking LED register bit 0 -  connection time out, bit 1- no flow water, bit 2 - continuously flowing water
uint16_t WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD_COUNTER;			//actual counter of numbers of continuously flowing water periods





/******************************************************************************************************************************************************
 *
 * Initialize alarm environment
 * switch OFF alarm RED LED
 * sample water meter counter into tmp variables
 * reset alarms timers
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmAlarmsInit(void) {
	WATER_METER_IS_WATER_FLOW_FLAG = 0;
	WM_CURRENT_COUNTER_CONTINUOUSLY_TMP =  WATER_METER_COUNTER.waterMeter.wmCurrentCounter;
	WM_ALARM_NO_WATER_FLOW_LED_OFF;
	WM_ALARM_CONTINUOUS_WATER_FLOW_LED_OFF;
	WATER_METER_COUNTER.waterMeter.wmNoWaterFlowAlarmFlag = 0;
	WATER_METER_COUNTER.waterMeter.wmContinuousWaterFlowAlarmFlag = 0;
	WM_TIMERS.wmNoWaterFlowTimer = 0;
}


/******************************************************************************************************************************************************
 *
*alarms pooling
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmAlarmsPoll(void) {

	uint8_t isWaterFlowContinuously;





	if(CFG_RAMMEM.configuration.wmNoWaterFlowAlarmAllow) {
		if(WM_TIMERS.wmNoWaterFlowTimer >= CFG_RAMMEM.configuration.wmNoWaterFlowAlarmTime) {
			WATER_METER_COUNTER.waterMeter.wmNoWaterFlowAlarmFlag++;
			WM_TIMERS.wmNoWaterFlowTimer = 0;
			WM_ALARM_NO_WATER_FLOW_LED_ON;
		};

		if(WATER_METER_IS_WATER_FLOW_FLAG) {
			WM_ALARM_NO_WATER_FLOW_LED_OFF;
			WM_TIMERS.wmNoWaterFlowTimer = 0;
		}
	}



	if(CFG_RAMMEM.configuration.wmContinuousWaterFlowAlarmAllow) {
		if(WM_TIMERS.wmContinuousWaterFlowPeriodTimer >= CFG_RAMMEM.configuration.wmContinuousWaterFlowAlarmPeriod ) {
			WM_TIMERS.wmContinuousWaterFlowPeriodTimer = 0;
			isWaterFlowContinuously = WM_CURRENT_COUNTER_CONTINUOUSLY_TMP ^ WATER_METER_COUNTER.waterMeter.wmCurrentCounter;
			WM_CURRENT_COUNTER_CONTINUOUSLY_TMP = WATER_METER_COUNTER.waterMeter.wmCurrentCounter;

			if(isWaterFlowContinuously) {
				WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD_COUNTER++;
			} else {
				WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD_COUNTER = 0;
				WM_ALARM_CONTINUOUS_WATER_FLOW_LED_OFF;
			}
		}

		if(WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD_COUNTER > CFG_RAMMEM.configuration.wmContinuousWaterFlowAlarmTimes) {
			WM_TIMERS.wmContinuousWaterFlowPeriodTimer = 0;
			WM_CONTINUOUS_WATER_FLOW_ALARM_PERIOD_COUNTER = 0;
			WM_ALARM_CONTINUOUS_WATER_FLOW_ON;
			WATER_METER_COUNTER.waterMeter.wmContinuousWaterFlowAlarmFlag++;
		}
	}


	if(WATER_METER_IS_WATER_FLOW_FLAG) {
		WATER_METER_IS_WATER_FLOW_FLAG = 0;
		wmRedLEDBlink(1, 1);
	}

	if(WM_RED_LED.blinkStatus == LED_BLINKING_IDLE && WM_ALARM_LED ) {
		wmRedLEDBlink(50, WM_ALARM_LED);

		if(CFG_RAMMEM.configuration.wmOUTTransistorAlarmAllow) {
			OUT_TRANSISTOR_ON;
		}
	}

	if((! WM_ALARM_LED) && (CFG_RAMMEM.configuration.wmOUTTransistorAlarmAllow == 0x002)) {
		OUT_TRANSISTOR_OFF;
	}

}
