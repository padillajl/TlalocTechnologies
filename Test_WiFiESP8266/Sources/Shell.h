/*
 * Shell.h
 *
 *  Created on: Jun 4, 2019
 *      Author: johernandezpc
 */

#ifndef SHELL_H_
#define SHELL_H_
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
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
void vfnShellInit(void);
void vfnShellTask(void);
void SHELL_vfnSendString(const u08 *lbString);
/*************************************************************************************************/
#endif /* SHELL_H_ */
