/*
 * callbackFunctions.c
 *
 *  Created on: 13 kwi 2021
 *      Author: JUREK
 */

#include <avr/pgmspace.h>
#include "registers.h"
#include "coils.h"
#include "callbackFunctions.h"
#include "../modbus/mb.h"


/******************************************************************************************************************************************************
 *
 * GLOBAL VARIABLES
 * definition of GLOBAL ARRAY of callback functions in program memory space
 *
 *******************************************************************************************************************************************************/

const TMBPtrCallbackFunction MB_CALLBACK_FUNCTIONS[18] PROGMEM = {	0,													//reserved
																	0,													//MB_FN_READ_COILS 				= 0x01 requesting the ON/OFF status of discrete coil
																	0,													//MB_FN_READ_DISCRET_INPUTS 	= 0x02 requesting the ON/OFF status of discrete inputs (multiple binary inputs)
																	wmCallbackReadHoldingRegisters,						//MB_FN_READ_HOLDING_REGISTERS	= 0x03 requesting the content of analog output holding registers (multiple cells memory)
																	wmCallbackReadInputRegisters,						//MB_FN_READ_INPUT_REGISTERS	= 0x04 requesting the content of analog input register
																	wmCallbacWriteCoil,									//MB_FN_WRITE_SINGLE_COIL		= 0x05 writing the contents of discrete coil (one coil)
																	0,													//MB_FN_WRITE_HOLDING_REGISTER	= 0x06 writing the contents of analog output holding register (one register or one cell memory)
																	0,													//MB_FN_READLAVE_STATUS			= 0x07 requesting status of SLAVE device
																	0,													//MB_FN_DIAGNOSTIC_TEST			= 0x08 requesting diagnostic test
																	0,													//reserved
																	0,													//reserved
																	0,													//reserved
																	0,													//reserved
																	0,													//reserved
																	0,													//reserved
																	0,													//MN_FN_WRITE_MULTIPLE_COILS	= 0x0F writing the contents of a series of n x 8 discrete coils (multiple coils)
																	wmCallbackWriteHoldingRegisters,					//MB_FN_WRITE_HOLDING_REGISTERS	= 0x10 writing the contents of multiple holding registers
																	0													//MB_FN_IDENTIFY_SLAVE			= 0x11 requesting SLAVE identify
};



/******************************************************************************************************************************************************
 *
 * handle read holding registers 0x03 modbus function
 *
 * read configuration of water meter from RAM memory - GLOBAL VARIABLE CFG_RAMMEM
 *
 *@param	void
 *
 *@return	uint8_t	MB_PROTOCOL_STATE.codeError
 *
 *******************************************************************************************************************************************************/

uint8_t wmCallbackReadHoldingRegisters( void ) {
	uint16_t registersStartAddress;
	uint16_t registersNumber;
	uint8_t  bytesNumber, n;
	registersStartAddress = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn03.startAddress);
	registersNumber = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn03.numberRegs);
	bytesNumber = registersNumber * 2;

	if((registersStartAddress >= CONFIGURATION_REGS_ADR_START) && (registersStartAddress + registersNumber - 1  <= CONFIGURATION_REGS_ADR_END)) {
		MB_FRAME_BUFFER.TxFn03.numberBytes = bytesNumber;
		registersStartAddress = registersStartAddress - CONFIGURATION_REGS_ADR_OFFSET;
		for(n=0;n<(registersNumber);n++) {
			MB_FRAME_BUFFER.TxFn03.dataUint16[n] = mbUtilSwapUint16(CFG_RAMMEM.dataUint16[registersStartAddress+n]);
		}
		return MB_ERR_OK;
	} else {
		MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_DATA_ADDR;
		return MB_PROTOCOL_STATE.codeError;
	};

}


/******************************************************************************************************************************************************
 *
 * handle write holding registers 0x10 modbus function
 *
 * write configuration of water meter into RAM memory - GLOBAL VARIABLE CFG_RAMMEM
 *
 *@param	void
 *
 *@return	uint8_t	MB_PROTOCOL_STATE.codeError
 *
 *******************************************************************************************************************************************************/

uint8_t wmCallbackWriteHoldingRegisters( void ) {

	uint16_t registersStartAddress;
	uint16_t registersNumber;
	uint8_t  bytesNumber, n;

	registersStartAddress = mbUtilSwapUint16( MB_FRAME_BUFFER.RxFn16.startAddress );
	registersNumber = mbUtilSwapUint16( MB_FRAME_BUFFER.RxFn16.numberRegs );
	bytesNumber = MB_FRAME_BUFFER.RxFn16.numberBytes ;

	if((registersStartAddress >= CONFIGURATION_REGS_ADR_START) && (registersStartAddress + registersNumber -1 <= CONFIGURATION_REGS_ADR_END)) {
		registersStartAddress = registersStartAddress - CONFIGURATION_REGS_ADR_OFFSET;
		for(n=0;n<(registersNumber);n++) {
			 CFG_RAMMEM.dataUint16[registersStartAddress+n] = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn16.dataUint16[n]);
		}
		return MB_ERR_OK;
	} else {
		MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_DATA_ADDR;  // error bad request
		return MB_PROTOCOL_STATE.codeError;
	};
}


/******************************************************************************************************************************************************
 *
 * handle read input registers 0x04 modbus function
 *
 * read counters and alarms flags from RAM memory - GLOBAL VARIABLE WATER_METER_COUNTER
 *
 *@param	void
 *
 *@return	uint8_t	MB_PROTOCOL_STATE.codeError
 *
 *******************************************************************************************************************************************************/

uint8_t wmCallbackReadInputRegisters( void ) {
	uint16_t registersStartAddress;
	uint16_t registersNumber;
	uint8_t  bytesNumber, n;

	registersStartAddress = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn04.startAddress);
	registersNumber = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn04.numberRegs);
	bytesNumber = registersNumber * 2;
	if((registersStartAddress >= WATER_METER_REGS_ADR_START) && (registersStartAddress + registersNumber -1 <= WATER_METER_REGS_ADR_END)) {
		MB_FRAME_BUFFER.TxFn04.numberBytes = bytesNumber;
		registersStartAddress = registersStartAddress - WATER_METER_REGS_ADR_OFFSET;
		for(n=0;n<(registersNumber);n++) {
			MB_FRAME_BUFFER.TxFn04.dataUint16[n] = mbUtilSwapUint16(WATER_METER_COUNTER.dataUint16[registersStartAddress+n]);
		}
		return MB_ERR_OK;
	} else {
		MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_DATA_ADDR;  // error bad request
		return MB_PROTOCOL_STATE.codeError;
	};


}




/******************************************************************************************************************************************************
 *
 * handle write single coil 0x05 modbus function
 *
 *write single coil - GLOBAL VARIABLE WATER_METER_COILS
 *
 *@param	void
 *
 *@return	uint8_t	MB_PROTOCOL_STATE.codeError
 *
 *******************************************************************************************************************************************************/

uint8_t wmCallbacWriteCoil(void) {
	uint8_t coilState;

	coilState = (mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn05.coilState) == WM_COIL_IS_SET_BY_MODBUS);

	WATER_METER_COILS.wmCoilAdrress = mbUtilSwapUint16(MB_FRAME_BUFFER.RxFn05.coilAddress);

	MB_PROTOCOL_STATE.codeError = MB_ERR_OK;
	MB_PROTOCOL_STATE.state = MB_ST_DONE_HANDLING_FUNCTIONS;
	WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_CHANGE;

	switch( WATER_METER_COILS.wmCoilAdrress ){
		case WM_COIL_READ_CONFIGURATION: {
			WATER_METER_COILS.wmReadConfiguration = coilState;
			break;
		}
		case WM_COIL_WRITE_CONFIGURATION: {
			WATER_METER_COILS.wmWriteConfiguration = coilState;
			break;
		}
		case WM_COIL_READ_CURRENT_COUNTER: {
			WATER_METER_COILS.wmReadCurrentCounter = coilState;
			break;
		}
		case WM_COIL_WRITE_CURRENT_COUNTER: {
			WATER_METER_COILS.wmWriteCurrentCounter = coilState;
			break;
		}
		case WM_COIL_RED_LED: {
			WATER_METER_COILS.wmREDLed = coilState;
			break;
		}
		case WM_COIL_GREEN_LED: {
			WATER_METER_COILS.wmGREENLed = coilState;
			break;
		}
		case WM_COIL_OUT_TRANSISTOR: {
			WATER_METER_COILS.wmOUTTransistor = coilState;
			break;
		}
		case WM_COIL_ALARMS_COUNTER_RESET: {
			WATER_METER_COILS.wmAlarmsReset = coilState;
			break;
		}

		default : {
			MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_DATA_ADDR;
			MB_PROTOCOL_STATE.state = MB_ST_THROW_EXCEPTION;
			WATER_METER_COILS.wmCoilStateChanged = WM_COILS_ST_IDLE;
			break;
		}
	}
	return MB_PROTOCOL_STATE.codeError;
}
