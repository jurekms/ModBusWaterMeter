/*
 * mb.c
 *
 *  Created on: 26 lip 2015
 *  Author: Jerzy Maczenski JMS
 */

#include "mb.h"
#include "config.h"
#include "mbIO.h"
#include "../waterMeter/LEDStatus.h"
#include "../waterMeter/callbackFunctions.h"




//volatile TMBCallbackFunction	MB_CALLBACK_FUNCTIONS[ MB_MAX_NUMER_CALLBACK_FUNCTIONS ]; 	// table with pointer to callback functions
volatile TMBFrame				MB_FRAME_BUFFER ;											// transsmision buffer RS232/485
volatile uint8_t				MB_RX_BUFFER_INDEX;											// RX-buffer pointer
volatile uint8_t				MB_TX_BUFFER_INDEX;											// TX-buffer pointer
volatile uint8_t				MB_ACTUAL_FRAME_SIZE;										// size of modbus frame (max 256 bytes)
volatile TMBState				MB_PROTOCOL_STATE;											// actual state of protocol (for state-machine)
volatile TmbError				MB_ERR;														// errors due to handle protocol
		 uint8_t				MB_MY_SLAVE_ADDR;											//my modbus address


/******************************************************************************************************************************************************
 *
 *Init UART and modbus protocol.
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/
void mbInit(uint8_t modBusAddress) {
	MB_MY_SLAVE_ADDR = modBusAddress;
	initUART();
	initTimer();
	MB_PROTOCOL_STATE.state = MB_ST_IS_IDDLE;
}



/******************************************************************************************************************************************************
 *
 *Calculate CRC16 for data in MB_FRAME_BUFFER.
 *
 *@param	void
 *
 *@return	calculated CRC16
 *
 *******************************************************************************************************************************************************/
uint16_t mbUtilCalculateCRC16(void) {
	uint8_t i, n;
	uint16_t crc = 0xffff;
	for (i = 0; i < MB_ACTUAL_FRAME_SIZE; i++) {
		crc ^= MB_FRAME_BUFFER.mbFrame[i];
		for (n = 0; n < 8; n++)
			crc = (crc & 0x0001) ? ((crc >> 1) ^ 0xa001) : (crc >> 1);
	}
	return crc;
}




/******************************************************************************************************************************************************
 *
 *Analyze recived modbus packet.
 *It checks:
 *	-the SLAVE address recive in modbus packet equals to set in MB_MY_SLAVE_ADDR global variable
 *	-the number of recive packet equals to 4 bytes (1 byte address SLAVE + 1byte modbus function code + 2 bytes CRC)
 *	-is CRC correct
 *
 *	@param	void
 *
 *	@return in MB_PROTOCOL_STATE structure
 *
 *******************************************************************************************************************************************************/
void mbAnalizeRecivedPacket(void) {
	MB_PROTOCOL_STATE.state = MB_ST_IS_PROCESSING_RX_FRAME;
	if (MB_FRAME_BUFFER.slaveAddr != MB_MY_SLAVE_ADDR) {
		MB_PROTOCOL_STATE.state = MB_ST_ERR_PROCESSING_RX_FRAME;
		MB_PROTOCOL_STATE.codeError = MB_ERR_INVALID_SLAVE_ADDR;
	} else {
		MB_PROTOCOL_STATE.codeError = MB_ERR_VALID_SLAVE_ADDR;
		if (MB_FRAME_BUFFER.codeFunction > 0x11 ){
			MB_PROTOCOL_STATE.state = MB_ST_ERR_PROCESSING_RX_FRAME;
			MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_FUNCTION;
		} else {
			if (MB_ACTUAL_FRAME_SIZE < 4) {
				MB_PROTOCOL_STATE.state = MB_ST_ERR_PROCESSING_RX_FRAME;
				MB_PROTOCOL_STATE.codeError = MB_ERR_INVALID_SIZE_FRAME;
			} else {
				MB_PROTOCOL_STATE.codeError = MB_ERR_VALID_SIZE_FRAME;
				if (mbUtilCalculateCRC16() != MB_FRAME_BUFFER.crc) {
					MB_PROTOCOL_STATE.state = MB_ST_ERR_PROCESSING_RX_FRAME;
					MB_PROTOCOL_STATE.codeError = MB_ERR_INVALID_CRC;
				} else {
					MB_PROTOCOL_STATE.state = MB_ST_DONE_PROCESSING_RX_FRAME;
					MB_PROTOCOL_STATE.codeError = MB_ERR_OK;
				}
			}
		}
	}
}



/******************************************************************************************************************************************************
 *
 *Swap LSB with MSB in uint16_t variable.
 *
 *@param	_uint16		variable to swap
 *
 *@return				swaping variable
 *
 *******************************************************************************************************************************************************/

uint16_t mbUtilSwapUint16(uint16_t _uint16) {
	return (((uint16_t) ((uint8_t) (_uint16))) << 8) + (uint8_t) (_uint16 >> 8);
}



/******************************************************************************************************************************************************
 *
 *Prepare modbus exeption packet - when something goes wrong.
 *
 *@param	void	error code read from global variable MB_PROTOCOL_STATE.codeError
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/

void mbHandleExceptionRespons(void) {

	MB_FRAME_BUFFER.codeFunction = MB_FRAME_BUFFER.codeFunction | 0x80;
	MB_FRAME_BUFFER.exception.exceptionCode = MB_PROTOCOL_STATE.codeError - 0x50;
	MB_ACTUAL_FRAME_SIZE = 3;
	MB_PROTOCOL_STATE.state = MB_ST_DONE_EXCEPTION;
	MB_PROTOCOL_STATE.codeError = MB_ERR_OK;
}



/******************************************************************************************************************************************************
 *
 *Processes recived modbus frame and call - callback function for recived modbus request.
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/
void mbFunctionHandler(void) {

	TMBPtrCallbackFunction ptrFunction = 0;

	MB_PROTOCOL_STATE.state = MB_ST_IS_HANDLING_FUNCTIONS;
	MB_PROTOCOL_STATE.codeError = MB_ERR_OK;

	ptrFunction = (TMBPtrCallbackFunction)pgm_read_word(&MB_CALLBACK_FUNCTIONS[MB_FRAME_BUFFER.codeFunction]);

	if (ptrFunction == 0) {
		MB_PROTOCOL_STATE.state = MB_ST_THROW_EXCEPTION;
		MB_PROTOCOL_STATE.codeError = MB_ERR_ILLEGAL_FUNCTION;
	} else {
		if (ptrFunction != 0) {
			if (ptrFunction() == MB_ERR_OK) {
				switch (MB_FRAME_BUFFER.mbFrame[1]) {
				case MB_FN_WRITE_SINGLE_COIL:
					MB_ACTUAL_FRAME_SIZE = 6;
					break;
				case MB_FN_WRITE_HOLDING_REGISTERS:
					MB_ACTUAL_FRAME_SIZE = 6;
					break;
				default:
					MB_ACTUAL_FRAME_SIZE = MB_FRAME_BUFFER.mbFrame[2] + 3;
					break;
				}
				MB_PROTOCOL_STATE.state = MB_ST_DONE_HANDLING_FUNCTIONS;
				MB_PROTOCOL_STATE.codeError = MB_ERR_OK;

			} else {
				MB_PROTOCOL_STATE.state = MB_ST_THROW_EXCEPTION;
			}
		} else {
			MB_PROTOCOL_STATE.state = MB_ST_ERR_HANDLING_FUNCTIONS;
			MB_PROTOCOL_STATE.codeError = MB_ERR_INVALID_CALLBACK_FUNCTION;
		}
	}
}



/******************************************************************************************************************************************************
 *
 *Polling state-machine modbus protocol, it should be called as often as possible!
 *
 *@param	void
 *
 *@return	void
 *
 *******************************************************************************************************************************************************/
void mbPoll(void) {

	switch (MB_PROTOCOL_STATE.state) {
	case MB_ST_DONE_RX:
		mbAnalizeRecivedPacket();
		break;
	case MB_ST_DONE_PROCESSING_RX_FRAME:
		mbFunctionHandler();
		break;
	case MB_ST_DONE_HANDLING_FUNCTIONS:
		wmGreenLEDBlink(1, 1);
		mbSendFrame();
		break;
	case MB_ST_THROW_EXCEPTION:
		mbHandleExceptionRespons();
		break;
	case MB_ST_DONE_EXCEPTION:
		mbSendFrame();
		break;
	case MB_ST_ERR_PROCESSING_RX_FRAME:
		//UART_ENABLE_RX_INT;
		//MB_PROTOCOL_STATE.state = MB_ST_IS_IDDLE;
		MB_PROTOCOL_STATE.state = MB_ST_THROW_EXCEPTION;
		break;
	case MB_ST_ERR_HANDLING_FUNCTIONS:
//			to do when the need
	case MB_ST_ERR_RX:
//			to do when the need
	case MB_ST_ERR_TX:
//			to do when the need
	case MB_ST_DONE_TX:
		MB_PROTOCOL_STATE.state = MB_ST_IS_IDDLE;
		break;
	default:
		break;
	};
}
