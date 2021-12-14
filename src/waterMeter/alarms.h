/*
 * alarms.h
 *
 *  Created on: 15 kwi 2021
 *      Author: JUREK
 */

#ifndef WATERMETER_ALARMS_H_
#define WATERMETER_ALARMS_H_


/******************************************************************************************************************************************************
 *
 * functions declarations
 *
 *******************************************************************************************************************************************************/

void wmAlarmsInit(void);
void wmAlarmsPoll(void);

/******************************************************************************************************************************************************
 *
 *
 * macros use in order to set or reset bits in WM_ALARM_LED GLOBAL VARIABLE
 *
 *******************************************************************************************************************************************************/


#define WM_ALARM_NO_WATER_FLOW_LED_ON				WM_ALARM_LED |= 1
#define WM_ALARM_CONTINUOUS_WATER_FLOW_ON			WM_ALARM_LED |= 2


#define WM_ALARM_NO_WATER_FLOW_LED_OFF				WM_ALARM_LED &= ~1
#define WM_ALARM_CONTINUOUS_WATER_FLOW_LED_OFF		WM_ALARM_LED &= ~2


volatile extern uint8_t WATER_METER_IS_WATER_FLOW_FLAG;

#endif /* WATERMETER_ALARMS_H_ */
