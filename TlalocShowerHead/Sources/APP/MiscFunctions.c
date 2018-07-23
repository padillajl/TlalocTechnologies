/*
 * MiscFunctions.c
 *
 *  Created on: Jul 12, 2018
 *      Author: Toxic
 */


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */

/* Includes used in this file */

/* Own includes */
#include "MiscFunctions.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/



/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 baBCD[20];

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
/*************************************************************************************************/
/*!
      \fn       bfnBCDToASCII
      \brief
      \param    u08 lbNible
      \return   u08 ASCII of the Nibble
*/
/*******************************************************************************/
u08 bfnBCDToASCII(u08 lbNibble)
{
	lbNibble &= 0x0F;
	if(lbNibble<10)	
		return(lbNibble+'0');	
	else	
		return(lbNibble + 'A' - 10);
}

/*******************************************************************************/
/*!
      \fn             vfnBin8ToBCD
      \brief
      \param          u08 lbBin8
      \return         void
      \n\rLocals:\n\r	
      \li             u08 lbBitCounter = 8
	    \li             u08 lbIndex      
      \n\rModifica:\n\r
      \li             u08 baBCD[]
*/
/*******************************************************************************/
void vfnBin8ToBCD (u08 lbBin8)
{
	u08 lbBitCounter = 8;
	u08 lbIndex;
	
	baBCD[0] = 0;
	baBCD[1] = 0;
	baBCD[2] = 0;
	do
	{
		baBCD[0] *= 2;
		if(lbBin8&(1<<7))
		{
			baBCD[0]|=1;
		}
		lbBin8 *=2;
		baBCD[1]*=2;
		baBCD[2]*=2;
		lbIndex = 0;
		do
		{		
			if(baBCD[lbIndex]>=10)
			{
				baBCD[lbIndex] -=10;
				baBCD[lbIndex+1]+=1;
			}
		}while(++lbIndex<2);
	}while(--lbBitCounter);
}
/*******************************************************************************/
/*!
      \fn         vfnBin16ToBCD
      \brief
      \param      u16 lwBin16
      \return     void
      \n\rLocals:\n\r
      \li         u08 lbBitCounter = 16
	    \li         u08 lbIndex
      \n\rModifica:\n\r
      \li         u08 baBCD[]
*/      
/*******************************************************************************/
void vfnBin16ToBCD(u16 lwBin16)
{
	u08 lbBitCounter = 16;
	u08 lbIndex;
	
	baBCD[0] = 0;
	baBCD[1] = 0;
	baBCD[2] = 0;
	baBCD[3] = 0;
	baBCD[4] = 0;	
	do
	{
		baBCD[0] *= 2;
		if(lwBin16&(1<<15))
		{
			baBCD[0]|=1;
		}
		lwBin16 *=2;
		baBCD[1]*=2;
		baBCD[2]*=2;
		baBCD[3]*=2;
		baBCD[4]*=2;
		lbIndex = 0;
		do
		{		
			if(baBCD[lbIndex]>=10)
			{
				baBCD[lbIndex] -=10;
				baBCD[lbIndex+1]+=1;
			}
		}while(++lbIndex<4);
	}while(--lbBitCounter);
}
/*******************************************************************************/
/*!
      \fn         vfnBin32ToBCD
      \brief
      \param      u32 ldwBin32
      \return     void      
*/
/*******************************************************************************/
void vfnBin32ToBCD (u32 ldwBin32)
{
	u08 lbBitCounter = 32;
	u08 lbIndex = 9;	
	u08 lbAux;
	do
	{
		baBCD[lbIndex] = 0;
	}while(lbIndex--);
	do
	{
		baBCD[0] *= 2;
		lbAux = (u08)(ldwBin32>>24);
		if(lbAux&(1<<7))
		{
			baBCD[0]|=1;
		}
		ldwBin32 *=2;
		lbIndex = 1;
		do
		{
			baBCD[lbIndex]*=2;
		}while(++lbIndex<10);
		lbIndex = 0;
		do
		{		
			if(baBCD[lbIndex]>=10)
			{
				baBCD[lbIndex] -=10;
				baBCD[lbIndex+1]+=1;
			}
		}while(++lbIndex<10);
	}while(--lbBitCounter);
}

/*******************************************************************************/
/*!
	\fn		vfnBin16ToASCIIBCD(u16 lwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
	\brief
	\param  u16 lwValue: Is the value that is going to be converter as BCD Num
	\param	u08* lbptrBuffer: Is the pointer to the array where de BCD value will be saved
	\param	u08 lbCopySize: Is the size of the data that is going to be copied to the array		
	\param	u08 lbBCDInit: Is the location where the copy starts
	\return     void      
*/
/*******************************************************************************/
void vfnBin16ToASCIIBCD(u16 lwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
{
	u08 lbAux;
	u08 *lbptr1 = lbptrBuffer;
	vfnBin16ToBCD(lwValue);
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(baBCD[lbAux+lbBCDInit]);
		lbAux++;
	}
	vfnInvertString(lbptrBuffer,lbCopySize);
}

/*******************************************************************************/
/*!
	\fn		vfnBin32ToASCIIBCD(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
	\brief
	\param  u32 ldwValue: Is the value that is going to be converter as BCD Num
	\param	u08* lbptrBuffer: Is the pointer to the array where de BCD value will be saved
	\param	u08 lbCopySize: Is the size of the data that is going to be copied to the array		
	\param	u08 lbBCDInit: Is the location where the copy starts
	\return     void      
*/
/*******************************************************************************/
void vfnBin32ToASCIIBCD(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
{
	u08 lbAux;
	u08 *lbptr1 = lbptrBuffer;
	vfnBin32ToBCD(ldwValue);
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(baBCD[lbAux+lbBCDInit]);
		lbAux++;
	}
	vfnInvertString(lbptrBuffer,lbCopySize);
}

/*******************************************************************************/
/*!
	\fn		vfnBin32ToASCIIBCDWithDecimalDot(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize)
	\brief
	\param  u32 ldwValue: Is the value that is going to be converter as BCD Num
	\param	u08* lbptrBuffer: Is the pointer to the array where de BCD value will be saved
	\param	u08 lbCopySize: Is the size of the data that is going to be copied to the array		
	\param	u08 lbBCDInit: Is the location where the copy starts
	\param	u08 lbDecimalSize: Is the number of decimals that is going to be used to represent the number in BCD format
	\return     void      
*/
/*******************************************************************************/
void vfnBin32ToASCIIBCDWithDecimalDot(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize)
{
	u08 lbAux;
	u08 *lbptr1;
	u08 *lbptr2;
	vfnBin32ToBCD(ldwValue);
	vfnInvertString(&baBCD[lbBCDInit],lbCopySize);
	
	lbptr1 = lbptrBuffer;
	lbptr2 = &baBCD[lbBCDInit]; 
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(*lbptr2++);
		lbAux++;
		if(lbAux == lbCopySize-lbDecimalSize)
			*lbptr1++='.';
	}	
}
/*******************************************************************************/
/*!
	\fn 	void vfnInvertString(u08 *lbpSource,u08 lbSize)
	\brief	Function that inverts the buffer elements
	\param	lbpSource: Is the start of the source buffer 
			lbSize: Quantity of data to invert
	\return	
*/
/*******************************************************************************/
void vfnInvertString(u08 *lbpSource,u08 lbSize)
{
	u08 lbAux;
	u08 lbEnd = lbSize-1;
	u08 lbAuxString[25];

	lbAux = 21;
	//Copy of the original array
	while(lbAux--)
		lbAuxString[lbAux] = *(lbpSource+lbAux);	
	
	while(lbSize--)
	{			
		*(lbpSource+(lbEnd-lbSize))=lbAuxString[lbSize];
	}
}

/*******************************************************************************/
/*!
	\fn		u08 bfnAsciiZeroSupressAndShiftToLeft(u08* lbptrBuffer, u08 lbSize, u08 lbZerosToDelete)
	\brief	Rutina que elimina '0' de un buffer hasta encontrar un valor distinto de '0'
			dentro de los lbSize datos del buufer y reescribe desde la primera posicion
			la cadena pero ya sin '0'.
	\param 	u08* lbptrBuffer Buffer donde se encuentran los datos a validar.
	\param  u08 lbSize Cantidad de datos en el string, incluyendo los 0.
	\param  u08 lbZerosToDelete Cantidad sugerida de ceros a eliminar.
	\return Regresa 1 cuando se hizo la comparacion satisfactoria
			Regresa 0 cuando no existian 0 para comparar
*/
/*******************************************************************************/
u08 bfnAsciiZeroSupressAndShiftToLeft(u08* lbptrBuffer, u08 lbSize, u08 lbZerosToDelete) //<JLHP>
{
	//Validamos que la primera posicion sea ascii zero
	if(*(lbptrBuffer) == '0')
	{
		u08 lbaArrayTemp[10];
		u08 lbZerosToDeleteTemp = lbZerosToDelete;
		u08 *lbptrTemp;
		u08 *lbptrOriginalBuffer;		
		
		//Copiar arreglo original a arreglo temporal
		memcpy(&lbaArrayTemp[0],lbptrBuffer,lbSize);
		
		lbptrTemp = &lbaArrayTemp[0];
		lbptrOriginalBuffer = lbptrBuffer;
		
		//Limpiar apuntador original
		memset(&lbptrOriginalBuffer[0],' ',lbSize);
		
		while(lbZerosToDeleteTemp)
		{			
			if(*(lbptrTemp) == '0')
			{				
				lbptrTemp++;
			}				
			else			
				break;
			lbZerosToDeleteTemp--;
		}		
		
		//Validar que el tamaño del string sea mayor que los ceros encontrados
		if(lbSize > lbZerosToDelete)
		{
			//Definir el nuevo valor de ceros eliminados
			lbZerosToDelete -= lbZerosToDeleteTemp;
			//Definir el nuevo tamaño del buffer sin los zeros
			lbSize -=  lbZerosToDelete;
			lbZerosToDeleteTemp = lbSize;
			//Copiar al buffer original los datos sin ceros 
			while(lbZerosToDeleteTemp--)
			{
				*lbptrOriginalBuffer++ = *lbptrTemp++;
			}
		}
		else
		{
			//Regresar los datos originales al arreglo original
			memcpy(lbptrOriginalBuffer,&lbaArrayTemp[0],lbSize);
			return lbSize;
		}
		
		//Regresar la nueva cantidad de datos
		return lbSize;
	}
	else
	{ 	//Regresar la cantidad recibida si el primero no es '0'
		return lbSize;
	}	
}
