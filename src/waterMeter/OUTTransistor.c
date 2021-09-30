/*
 * OUTTransistor.c
 *
 *  Created on: 22 wrz 2021
 *      Author: JUREK
 */


#include "OUTTransistor.h"



/******************************************************************************************************************************************************
 *
 *initialize out port for transistor control and sets it OFF
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void wmOUTTransisitorInit(void) {
	OUT_TRANSISTOR_CONFIG;
	OUT_TRANSISTOR_OFF;

}
