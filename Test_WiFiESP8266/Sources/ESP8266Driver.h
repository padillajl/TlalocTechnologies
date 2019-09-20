/*
 * ESP8266Driver.h
 *
 *  Created on: Jun 4, 2019
 *      Author: johernandezpc
 */

#ifndef ESP8266DRIVER_H_
#define ESP8266DRIVER_H_
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "CLS1.h"
#include "MyTypes.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/


/*************************************************************************************************/
/*********************					Extern Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnESP8266DriverInit(void);
/*!
 * \brief Command line parser routine
 * \param lbcmd Pointer to command line string
 * \param lhandled Return value if command has been handled
 * \param lInputOutput Standard Shell I/O handler
 * \return Error code, ERR_OK for no failure
 */
u08 ESP8266Driver_ParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType *lInputOutput);
/*!
 * \brief Send a string to th ESP8266 module
 * \param lbString String to send, "\r\n" will be appended
 * \param lInputOutput Shell I/O handler or NULL if not used
 * \return Error code, ERR_OK for no failure
 */
u08 ESP8266Driver_SendString(const u08 *lbString, CLS1_ConstStdIOType *lInputOutput);
/*!
 * \brief Used to send an AT command to the ESP8266 module
 * \param lbCmd Command string to send
 * \param lbRxBufffer Buffer for the response, can be NULL
 * \param lRxBufferSize Size of response buffer
 * \param lbExpectedTailString Expected response from the module, can be NULL
 * \param lwMsTimeout Timeout time in milliseconds
 * \param lInputOutput Shell I/O handler or NULL if not used
 * \return Error code, ERR_OK for no failure
 */
u08 ESP8266Driver_SendATCommand(u08 *lbCmd, u08 *lbRxBuffer, size_t lRxBufferSize, u08 *lbExpectedTailString, u16 lwMsTimeOut, const CLS1_StdIOType *lInputOutput);
/*************************************************************************************************/
#endif /* ESP8266DRIVER_H_ */
