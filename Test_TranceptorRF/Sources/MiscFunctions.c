/*
 * MiscFunctions.c
 *
 *  Created on: Oct 20, 2014
 *      Author: Administrador
 */


#include "MiscFunctions.h"

/*******************************************************************************/
/*******************************************************************************/
u16 wRi505CRC16(u16 wliCRC,u08 lbData,u16 lwTruncatedPolynomial);

/*******************************************************************************/
/*******************************************************************************/

#define CRCMask	0x73
/*******************************************************************************/
/*******************************************************************************/
#define Cien                  	20
#define _11a15                	9
#define Veinte                	15
#define _Y_                  	16
#define Cero                  	10
#define mil                   	0
#define millones              	1
#define millon                	2

const u08 _Currency[] = {"PESOS "};
const u08 _Cero[] = {"CERO "};
const u08 _Un[] = {"UN "};
const u08 _Dos[] = {"DOS "};
const u08 _Tres[] = {"TRES "};
const u08 _Cuatro[] = {"CUATRO "};
const u08 _Cinco[] = {"CINCO "};
const u08 _Seis[] = {"SEIS "};
const u08 _Siete[] = {"SIETE "};
const u08 _Ocho[] = {"OCHO "};
const u08 _Nueve[] = {"NUEVE "};
const u08 _Diez[] = {"DIEZ "};
const u08 _Once[] = {"ONCE "};
const u08 _Doce[] = {"DOCE "};
const u08 _Trece[] = {"TRECE "};
const u08 _Catorce[] = {"CATORCE "};
const u08 _Quince[] = {"QUINCE "};
const u08 _Veinte[] = {"VEINTE "};
const u08 _Veinti[] = {"VEINTI"};
const u08 _Treinta[] = {"TREINTA "};
const u08 _Cuarenta[] = {"CUARENTA "};
const u08 _Cincuenta[] = {"CINCUENTA "};
const u08 _Sesenta[] = {"SESENTA "};
const u08 _Setenta[] = {"SETENTA "};
const u08 _Ochenta[] = {"OCHENTA "};
const u08 _Noventa[] = {"NOVENTA "};
const u08 _Cien[] = {"CIEN "};
const u08 _Ciento[] = {"CIENTO "};
const u08 _Cientos[] = {"CIENTOS "};
const u08 _Quinientos[] = {"QUINIENTOS "};
const u08 _Sete[] = {"SETE"};
const u08 _Nove[] = {"NOVE"};
const u08 _Mil[] = {"MIL "};
const u08 _Millon[] = {"MILLON "};
const u08 _Millones[] = {"MILLONES "};
const u08 _Y[] = {"Y "};
 
const u08* const UPointer[] = 		//Apuntadores a unidades
{
	0,
	&_Un[0],
	&_Dos[0],
	&_Tres[0],	
	&_Cuatro[0],
	&_Cinco[0],
	&_Seis[0],
	&_Siete[0],
	&_Ocho[0],
	&_Nueve[0],
	&_Cero[0]
};

const u08* const DPointer[] = 		//Apuntadores a decenas y la 'y'
{
	0,
	&_Diez[0],
	&_Veinti[0],
	&_Treinta[0],
	&_Cuarenta[0],	
	&_Cincuenta[0],
	&_Sesenta[0],
	&_Setenta[0],
	&_Ochenta[0],
	&_Noventa[0],
	&_Once[0],
	&_Doce[0],
	&_Trece[0],
	&_Catorce[0],
	&_Quince[0],
	&_Veinte[0],
	&_Y[0]
};

const u08* const CPointer[] = 		//Apuntadores a centenas
{
	0,
	0,
	&_Ciento[0],
	0,
	&_Dos[0],
	&_Cientos[0],
	&_Tres[0],
	&_Cientos[0],
	&_Cuatro[0],
	&_Cientos[0],
	&_Quinientos[0],
	0,
	&_Seis[0],
	&_Cientos[0],
	&_Sete[0],
	&_Cientos[0],
	&_Ocho[0],
	&_Cientos[0],
	&_Nove[0],
	&_Cientos[0],
	&_Cien[0]
};

const u08* const UnitsPointer[] = 	//Apuntadores a las unidades de magnitud
{
	&_Mil[0],
	&_Millones[0],
	&_Millon[0],
};

/*******************************************************************************/
/*******************************************************************************/
u08 baBCD[20];
u08 *bptrTextArray[18];
u08 bBCDCharacters;
/*******************************************************************************/
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
      \fn         bfnASCIIToDec
      \brief
      \param      u08 lbAscii
      \return     u08
*/
/*******************************************************************************/
u08 bfnASCIIToBCD(u08 lbAscii)
{	
	if((lbAscii>='0')&&(lbAscii<='9'))  
		return(lbAscii - '0');
	//Support of dots of 6x14 system font
	else if(lbAscii == '.')
		return 12;
	//Suport of spaces of 6x14 system font
	else if(lbAscii == ' ')	
		return 13;	
	else
	{
		if((lbAscii>='A')&&(lbAscii<='Z'))		
			return(lbAscii- 'A' + 10);	
		else if((lbAscii>='a')&&(lbAscii<='z'))
			return(lbAscii- 'a' + 10);	
		else
			return(0xFF);	
	}
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

void vfnBin8ToASCIIBCD(u08 lbValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
{
	u08 lbAux;
	u08 *lbptr1 = lbptrBuffer;
	vfnBin8ToBCD(lbValue);
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(baBCD[lbAux+lbBCDInit]);
		lbAux++;
	}
	vfnInvertString(lbptrBuffer,lbCopySize);
}
void vfnBin8ToASCIIBCDWithDecimalDot(u08 lbValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit, u08 lbDecimalSize)
{
	u08 lbAux;
	u08 *lbptr1 = lbptrBuffer;
	vfnBin8ToBCD(lbValue);
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(baBCD[lbAux+lbBCDInit]);
		lbAux++;
		if(lbAux == lbCopySize-lbDecimalSize)
			*lbptr1++= 60;

	}
	vfnInvertString(lbptrBuffer,lbCopySize);
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

void vfnBin16ToHEXASCII(u16 lwValue, u08* lbptrBuffer)
{
	u08 *lbptr1 = lbptrBuffer;
		
	*lbptr1++=bfnBCDToASCII((lwValue>>12)&0x0f);	
	*lbptr1++=bfnBCDToASCII((lwValue>>8)&0x0f);
	*lbptr1++=bfnBCDToASCII((lwValue>>4)&0x0f);
	*lbptr1  =bfnBCDToASCII((lwValue)&0x0f);
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
      \fn         vfnBin64ToBCD
      \brief
      \param      u64 ldwBin64
      \return     void      
*/
/*******************************************************************************/
void vfnBin64ToBCD(u64 lqwBin64)
{
	u08 lbBitCounter = 64;
	u08 lbIndex;	
	u08 lbAux;
	
	//Se rellena a ceros el buffer de BCD
	memset(&baBCD[0],0,sizeof(baBCD));
	
	//Se realiza el barrido para la conversion a BCD
	do
	{
		baBCD[0] <<= 1;
		lbAux = (u08)(lqwBin64>>56);
		if(lbAux&(1<<7))
		{
			baBCD[0]|=1;
		}
		lqwBin64 <<=1;
		lbIndex = 1;
		do
		{
			baBCD[lbIndex]<<=1;
		}while(++lbIndex<20);
		
		lbIndex = 0;
		do
		{		
			if(baBCD[lbIndex]>=10)
			{
				baBCD[lbIndex] -=10;
				baBCD[lbIndex+1]+=1;
			}
		}while(++lbIndex<20);
	}while(--lbBitCounter);
}

void vfnBin64ToASCIIBCD(u64 lqwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit)
{
	u08 lbAux;
	u08 *lbptr1 = lbptrBuffer;
	vfnBin64ToBCD(lqwValue);
	lbAux = 0;
	while(lbAux<lbCopySize)
	{
		*lbptr1++=bfnBCDToASCII(baBCD[lbAux+lbBCDInit]);
		lbAux++;
	}
	vfnInvertString(lbptrBuffer,lbCopySize);
}

void vfnBin64ToASCIIBCDWithDecimalDot(u64 lqwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize)
{
	u08 lbAux;
	u08 *lbptr1;
	u08 *lbptr2;
	vfnBin64ToBCD(lqwValue);
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
	\fn     bfnASCIIFrameToBin8
	\brief
	\param	u08*  lbpASCII
	\param  u08*  lwpDecResult
	\param  u08   lbASCIIFrameSize
	\return	u08
*/
/*******************************************************************************/
u08 bfnASCIIBCDToBin8(u08* lbpASCII, u08* lbpDecResult, u08 lbASCIIFrameSize)
{
  u08 lbTemp;
  *lbpDecResult = 0;
  while(lbASCIIFrameSize--)
	{
		lbTemp = bfnASCIIToBCD(*lbpASCII++);
		if(lbTemp < 10)
		{
			*lbpDecResult *= 10;
			*lbpDecResult += lbTemp;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}
/*******************************************************************************/
/*!
	\fn     bfnASCIIFrameToBin16
	\brief
	\param	u08*  lbpASCII
	\param  u08*  lwpDecResult
	\param  u08   lbASCIIFrameSize
	\return	u08
*/
/*******************************************************************************/
u08 bfnASCIIBCDToBin16(u08* lbpASCII, u16* lwpDecResult, u08 lbASCIIFrameSize)
{
  u08 lbTemp;
  *lwpDecResult = 0;
  while(lbASCIIFrameSize--)
	{
		lbTemp = bfnASCIIToBCD(*lbpASCII++);
		if(lbTemp < 10)
		{
			*lwpDecResult *= 10;
			*lwpDecResult += lbTemp;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}
/*******************************************************************************/
/*!
	\fn     bfnASCIIFrameToBin32
	\brief
	\param	u08*  lbpASCII
	\param  u08*  lwpDecResult
	\param  u08   lbASCIIFrameSize
	\return	u08
*/
/*******************************************************************************/
u08 bfnASCIIBCDToBin32(u08* lbpASCII, u32* ldwpDecResult, u08 lbASCIIFrameSize)
{
  u08 lbTemp;
  *ldwpDecResult = 0;
  while(lbASCIIFrameSize--)
	{
		lbTemp = bfnASCIIToBCD(*lbpASCII++);
		if(lbTemp < 10)
		{
			*ldwpDecResult *= 10;
			*ldwpDecResult += lbTemp;
		}
		else
			return 1;		
	}
	return 0;
}

/*******************************************************************************/
/*!
	\fn     bfnHEXASCIIFrameToBin32
	\brief
	\param	u08*  lbpASCII
	\param  u08*  lwpDecResult
	\param  u08   lbASCIIFrameSize
	\return	u08
*/
/*******************************************************************************/
u08 bfnHEXASCIIBCDToBin32(u08* lbpASCII, u32* ldwpDecResult, u08 lbASCIIFrameSize)
{
  u08 lbTemp;
  *ldwpDecResult = 0;
  while(lbASCIIFrameSize--)
	{
		lbTemp = bfnASCIIToBCD(*lbpASCII++);
		if(lbTemp < 16)
		{			
			*ldwpDecResult<<=4;
			*ldwpDecResult |= lbTemp;			
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

/****************************************************************************/
/*!
	\fn		u08 bfnCheckParity(u08 lbData, u08 lbParity)
	\brief	Rutina que checa la paridad par de un dato de 8 bits
	\param	u08 lbData, Dato de 8 bits al cual se le checara la paridad
	\param	u08 lbParity, Dato de paridad para validar contra el dato de informacion
	\return	1 si la paridad impar
	\return 0 si la paridad par
*/
/****************************************************************************/
u08 bfnCheckParity(u08 lbData, u08 lbParity)
{
	u08 lbCounter = 8;
	u08 lbParityCounter = 0;
	
	while( lbCounter-- )
	{
		if( lbData & (0x80) )
			lbParityCounter++;
		
		lbData<<=1;
	}
	
	lbParityCounter += lbParity;
	
	if(!(lbParityCounter%2))
		return 0;
	return 1;
}

/*******************************************************************************/
/*!
	\fn 	void vfnInvertString(u08 *lbpSource,u08 lbSize)
	\brief	Rutina que inverte los elementos de un buffer.
	\param	lbpSource	Inicio del buffer que se desea invertir.
			lbSize		Cantidad de datos a invertir.
	\return	Dato en Decimal BCD del dato recibido.
*/
/*******************************************************************************/
void vfnInvertString(u08 *lbpSource,u08 lbSize)
{
	u08 lbAux = 0;
	u08 *lbptrSource = lbpSource;	
	u08 lbAuxString[25];
	
	while(lbAux < lbSize)
	{
		lbAuxString[lbAux] = *(lbptrSource++);
		lbAux++;
	}
	lbptrSource = lbpSource;
	lbAux = lbSize;
	while(lbAux)
	{
		*(lbpSource++) = lbAuxString[lbAux-1] ;
		lbAux--;
	}
		

}

/*******************************************************************************/
/*!
	\fn		void vfnAsciiZeroSupress(u08* lbptrBuffer, u08 lbSize)
	\brief	Rutina que elimina '0' de un buffer hasta encontrar un valor distinto de '0'
			dentro de los lbSize datos del buufer.
	\param 	u08* lbptrBuffer Buffer donde se encuentran los datos a validar.
	\param  u08 lbSize Cantidad de datos a partir del buffer para validar.
*/
/*******************************************************************************/
void vfnAsciiZeroSupress(u08* lbptrBuffer, u08 lbSize)
{
	while(lbSize--)
	{
		if( *lbptrBuffer=='0' )		
			*lbptrBuffer = ' ';		
		else		
			break;				
		lbptrBuffer++;
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

void vfnAsciiZeroSupressWithPrefix(u08* lbptrBuffer, u08 lbSize, u08 lbChar)
{	
	while(lbSize--)
	{
		if( *lbptrBuffer=='0' )		
			*lbptrBuffer = ' ';		
		else					
			break;		
		lbptrBuffer++;
	}
	lbptrBuffer-=2;
	*lbptrBuffer = lbChar;
}

/*******************************************************************************/
/*!
      \fn           bpfnFindByte
      \brief
      \param        u08 * lbpBufferStartAddress
      \param        u08 lbByteToFind
      \param        u16 lwBufferSize
      \return       u08*
*/
/*******************************************************************************/
u08* bpfnFindByte(u08 * lbpBufferStartAddress, u08 lbByteToFind, u16 lwBufferSize)
{
	u08 * lbByteAddress = lbpBufferStartAddress;
	while((lwBufferSize--)&&((*lbByteAddress)!= lbByteToFind))
	{
		lbByteAddress++;
	}
	return(lbByteAddress);
}

/*******************************************************************************/
/*!
	\fn			bfnLRC
	\brief		Rutina que calcula Checksum de una trama.
	\param		u08* lbpLRCPtr
	\param		u08 lbLRCCounter
	\return		u08 (lbLRC|0x80)
*/
/*******************************************************************************/
u08 bfnLRC(u08* lbpLRCPtr,u16 lwLRCCounter)
{
	u08 lbLRC;
	lbLRC = 0;
	do
	{
		lbLRC ^= *lbpLRCPtr;
		lbpLRCPtr++;
	}while(--lwLRCCounter);
	
	return(lbLRC);
}

/*******************************************************************************/
/*!
      \fn       bfnCRC
      \brief
      \param    u08* lbpCRCPtr
      \param    u08 lbCRCFrameSize
      \return   u08 lbCRC
*/
/*******************************************************************************/
u08 bfnCRC(u08* lbpCRCPtr, u08 lbCRCFrameSize)
{
  u08 lbtemp = 0;
  u08 lbCRC = 0;
  u08 lby = 0;
  u08 lbAu1 = 0;
  u08 lbPuente = 0;
  do
  { 
    lbtemp=0;
    do
    {
      lbPuente=*lbpCRCPtr;
      lbPuente=1&(lbPuente>>lbtemp);
			lbAu1=(lbCRC&0x01);
      lbAu1^=(lbPuente);
      if(lbAu1&0x01)
      { 		    
				lbPuente=(lbCRC&0x18)^(0x18);
				lbPuente>>=1;
        lbPuente|=0x80;
      }
      else
      {
       lbPuente=(lbCRC&0x18)^(0x00);
       lbPuente>>=1;
      }
      lbCRC=((lbCRC>>1)&CRCMask)|lbPuente;
      lbtemp++;
    }while(lbtemp<8); 
		lby++;
    lbpCRCPtr++;	
  }while(lby<lbCRCFrameSize);    	
  return lbCRC;
}

/********************************************************************************************************/
/*!
  	\fn     wRi505MonCRC16CCITT
  	\brief  Función para generar un CRC de los datos pasados en los parametros.
  	\param  lbpPointer: Puntero del inicio de los datos para generar CRC
  			lwCounter: Cantidad de datos que se usaran para generar el CRC
  			lwTruncatedPolynomial: Polinomio a usar para la operación de XOR
  	\return u16: Crc de 16 bits de los datos pasados como parametros.  	
  	\date	24/06/13
*/
/********************************************************************************************************/
u16 wRi505CRC16CCITT(u08 *lbpPointer, u16 lwCounter,u16 lwTruncatedPolynomial)
{
	u16 wTemp=0x0000;
	do
	{
		wTemp=wRi505CRC16(wTemp,*lbpPointer,lwTruncatedPolynomial);
		lbpPointer++;
	}
	while(--lwCounter);
	return wTemp;
}

/********************************************************************************************************/
/*!
  	\fn     wRi505MonCRC16
  	\brief  Función para generar un CRC de los datos pasados en los parametros.
  	\param  wliCRC: Valor inicial para generar el CRC del dato.
  			lbData: Dato que se usara para generar el CRC.
  			lwTruncatedPolynomial: Polinomio a usar para la operación de XOR
  	\return u16: Crc de 16 bits del dato pasado como parametro.  	
  	\date	24/06/13
*/
/********************************************************************************************************/
u16 wRi505CRC16(u16 wliCRC, u08 lbData,u16 lwTruncatedPolynomial)
{
	u08 lbIndex;

	wliCRC ^= (u16)(lbData)<<8;
	lbIndex=8;
	do
	{
		if(wliCRC&0x8000)
		{
			wliCRC=(wliCRC<<1)^lwTruncatedPolynomial;
		}
		else
		{
			wliCRC<<=1;
		}
	}
	while(--lbIndex);
	return wliCRC;
}

u08 bfnPointerToText(u08* lBCDPointer)
{
	u08 lBCDCounter;
	u08* plTextPointers[18];
	u08 lUDCIndex;
	u08 lWordCounter;
	u08 lTextArraySize;

	lWordCounter=0;
	lBCDCounter=bBCDCharacters;
	if(lBCDCounter)
	{
		do
		{
			lUDCIndex = lBCDCounter % 3;				//El residuo de la división entre tres, determinará en qué orden de magnitud se
			if(!lUDCIndex)											//encuentra el apuntador lBCD Pointer
			{
				/*CENTENAS*/
				if((*lBCDPointer==1)&&(*(lBCDPointer+1)==0)&&(*(lBCDPointer+2)==0))  //Si hay un 100, se hace una excepción para
				{																																		//escribir "cien" en vez de "cientos"
					plTextPointers[lWordCounter]=(u08*)CPointer[Cien];
					lWordCounter++;
					lBCDPointer++;
				}
				else
				{
					plTextPointers[lWordCounter]=(u08*)CPointer[*lBCDPointer*2];			//La dirección de multiplica por dos ya que el arreglo
					lWordCounter++;																							//de centenas es compuesto. ("valor" cientos)
					plTextPointers[lWordCounter]=(u08*)CPointer[(*lBCDPointer*2)+1];
					lWordCounter++;
					lBCDPointer++;
				}
			}
			else if(lUDCIndex==2)
			{
				/*DECENAS*/
				if((*lBCDPointer==1)&&(*(lBCDPointer+1)>=1)&&(*(lBCDPointer+1)<=5)) //Si el valor ocurre entre 11 y 15 ocurre una
				{																																		//excepción a la forma: "decenas" y "unidades"
					plTextPointers[lWordCounter]=(u08*)DPointer[*(lBCDPointer+1)+_11a15];				//Se direcciona con referencia al valor de
					lWordCounter++;																										//unidades de la excepción (por eso se usa la
					lBCDPointer++;																										//siguiente dirección del apuntador), y se suma
					*lBCDPointer=0;																										//un offset al arreglo hacia los valores para 
				}
				else if(*lBCDPointer==2)																		//El veinte también es una excepción, pues no lleva una
				{																														//'y' después (es "veinti")
					if(!*(lBCDPointer+1))																			//Si la siguiente dirección tiene un 0, entonces si es
					{																													//"veinte"
						plTextPointers[lWordCounter]=(u08*)DPointer[Veinte];
						lWordCounter++;
						lBCDPointer++;
					}
					else
					{
						plTextPointers[lWordCounter]=(u08*)DPointer[*lBCDPointer];		//Si no, es "veinti"
						lWordCounter++;
						lBCDPointer++;
					}
				}	
				else
				{
					plTextPointers[lWordCounter]=(u08*)DPointer[*lBCDPointer];			//Y si no, entonces es cualquier otro número con la
					lWordCounter++;																						//forma normal.
					lBCDPointer++;
					if((*(lBCDPointer-1))&&(*lBCDPointer))
					{
						plTextPointers[lWordCounter]=(u08*)DPointer[_Y_];
						lWordCounter++;
					}
				}
			}
			else if(lUDCIndex==1)
			{
				/*UNIDADES*/
				plTextPointers[lWordCounter]=(u08*)UPointer[*lBCDPointer];
				lWordCounter++;
				lBCDPointer++;
				if((lBCDCounter==1)||((!*(lBCDPointer-1))&&(!*(lBCDPointer-2))&&(!*(lBCDPointer-3)))){}
									//Si el apuntador se encuentra en la primera terna, o si solo había ceros en la terna actual,
									//no se escribe el orden de magnitud
				else if(!(lBCDCounter%2))		//Si sí lleva orden de magnitud, cada terna par lleva un mil
				{
					plTextPointers[lWordCounter]=(u08*)UnitsPointer[mil];
					lWordCounter++;
				}
				else if((*(lBCDPointer-1)==1)&&(!*(lBCDPointer-2))&&(!*(lBCDPointer-3))&&(!*(lBCDPointer-4)))
				{																										//Un "millón" es una excepción
					plTextPointers[lWordCounter]=(u08*)UnitsPointer[millon];
					lWordCounter++;
				}
				else
				{
					plTextPointers[lWordCounter]=(u08*)UnitsPointer[millones];
					lWordCounter++;
				}
			}
		}while(--lBCDCounter);

		lTextArraySize=lWordCounter;
		lBCDCounter=0;
		lWordCounter=0;
		do																		//Esta rutina elimina los caracteres nulos a los que quedó apuntando el arreglo
		{																			//Tranfiriendo al arreglo final solo los valores que no son cero.
			if(!plTextPointers[lWordCounter])		
			{
				lWordCounter++;
			}
			else
			{
				bptrTextArray[lBCDCounter]=plTextPointers[lWordCounter];
				lWordCounter++;
				lBCDCounter++;
			}
		}while(lWordCounter!=lTextArraySize);
	}	
	else
	{															//palabra "cero" también es una excepción.
		bptrTextArray[0]=(u08*)UPointer[Cero];
		lBCDCounter++;
	}
	return(lBCDCounter);					//La función devuelve la cantidad de bytes que se usaron del arreglo.
}

/***************************************************************************/
/*!
	\fn			vfnConvertToText
	\brief		Convierte a texto el valor guardad en sBCD de bCentenas a bCenMilMillon
	\param 		none
	\return		none		
*/
/***************************************************************************/
u08 bfnConvertToText(u08 *lbptrPrinterBuffer)
{
	u08 *lbPrintBufferPtr;
	u08 *lbWordPtr;
	u16 lbLetterCounter;	
	u16 lwWordCounter;
	
	lbPrintBufferPtr = lbptrPrinterBuffer;
	bBCDCharacters = 10;
	do
	{
		if(*lbPrintBufferPtr)
		{
			break;
		}
		else
		{
			lbPrintBufferPtr++;
			bBCDCharacters--;
		}
	}while(bBCDCharacters);
	lwWordCounter = bfnPointerToText(lbPrintBufferPtr);
	lbPrintBufferPtr = lbptrPrinterBuffer;
	lbLetterCounter = 0;
	bBCDCharacters = 0;
	do
	{
		lbWordPtr = bptrTextArray[bBCDCharacters];
		do
		{
			*lbPrintBufferPtr++ = *lbWordPtr++;
			lbLetterCounter++;
		}while(*lbWordPtr);
		bBCDCharacters++;
	}while(--lwWordCounter);

	lbWordPtr = (u08*)&_Currency[0];
	do
	{
		*lbPrintBufferPtr++ = *lbWordPtr++;
		lbLetterCounter++;
	}while(*lbWordPtr);
			
  	return lbLetterCounter;
}

u08 bfnIsASCII(u08 lbAscii)
{
	if((lbAscii>='0')&&(lbAscii<='9'))  	
    	return 1;  	
  	
	if((lbAscii>='A')&&(lbAscii<='Z'))    
		return 1;    
    
    return 0;
}

u08 bfnIsDigitASCII(u08 lbAscii)
{
	if((lbAscii>='0')&&(lbAscii<='9'))  	
    	return 1;  	
	if((lbAscii>='A')&&(lbAscii<='Z'))    
		return 1;  
	
	return 0;
}

u08 bfnBufferDatasAreDigitsASCII(u08 *lbptrBuffer,u08 lbSize)
{
	while(lbSize--)
	{
		if( !bfnIsDigitASCII(*lbptrBuffer) )		
			return 0;
		lbptrBuffer++;
	}	
    return 1;
}

u08 bfnIsLeapYear(u16 lwYear)
{
	if( (!(lwYear%4) && (lwYear%100)) || !(lwYear%400) )
		return 1;
	
	return 0;
}

u08 bfnDaysInMounth(sTime sDate)
{
	u08 lbResult;
	u08 lwYear = sDate.bYear;
	lwYear += 2000;
	
	switch(sDate.bMonth)
	{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			lbResult=31;
			break;
		case 4: case 6: case 9: case 11:
			lbResult=30; 
			break;
		case 2:
			if(bfnIsLeapYear(lwYear)) 
				lbResult=29;
			else
				lbResult=28;			
			break;
		default: lbResult = 0;
	}
	
	return lbResult;
};

sTime strAddDaystoDate(sTime lsDate, u16 lwAddDays)
{
	while( lwAddDays-- )
	{
		if( lsDate.bDay < bfnDaysInMounth(lsDate) )
			lsDate.bDay++;
		else
		{
			if(lsDate.bMonth==12)
			{
				lsDate.bMonth=1;
				lsDate.bYear++;
			}
			else
			{
				lsDate.bMonth++;
			}
			lsDate.bDay=1;
		}
	}	
	return lsDate;
}

u16 bfnModBusLRC(u08* lbpLRCMFMPtr, u08 lbLRCMFMCounter)
{
  
 	u08 lbTemp;		
	u16 lbBridge;
	
	if((lbLRCMFMCounter>=0))// && (lbLRCMFMCounter<=15))
	{
		lbBridge=0xFFFF;		
		do
		{ 
			lbBridge^=(* lbpLRCMFMPtr);
			lbTemp=0;
			do
			{
				if(lbBridge&0x01)
				{ 
					lbBridge >>= 1;		    
					lbBridge ^=0xA001;
				} 
				else
				{
					lbBridge >>= 1;
				}
				lbTemp++;
			}while(lbTemp<8);
			lbpLRCMFMPtr++;
		}while(--lbLRCMFMCounter);
	}
	return lbBridge;
}

/*************************************************************************************/
/*!
	\fn		vfnDateToASCII
	\brief	Rutina que convierte una fecha de una estructura de tipo sTime a formato ASCII DDMMAAHHMMSS
	\param	lTime 			Fecha-Hora a convertir
	\param	lbptrBuffer 	Buffer donde quedara la conversion
	\return	none
*/
/*************************************************************************************/
void vfnDateToASCII(sTime lTime, u08* lbptrBuffer)
{
	vfnBin16ToASCIIBCD((u16)lTime.bHour,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
	vfnBin16ToASCIIBCD((u16)lTime.bMinutes,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
	vfnBin16ToASCIIBCD((u16)lTime.bSeconds,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
	vfnBin16ToASCIIBCD((u16)lTime.bDay,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
	vfnBin16ToASCIIBCD((u16)lTime.bMonth,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
	vfnBin16ToASCIIBCD((u16)lTime.bYear,lbptrBuffer,2,0);
	lbptrBuffer+= 2;
}

/*************************************************************************************/
/*!
	\fn		vfnGetPositionData
	\brief	Rutina que ordena el codigo de 16 caracteres que se reciben para realizar el cambio a modo basico
	\param  lbSeed es el numero del 0 al 9 que indica el orden de los elementos
	\param  lbElement es cada uno de los elementos que se van a ordenar
	\return	none
*/
/*************************************************************************************/
u08 vfnGetPositionData(u08 lbSeed, u08 lbElement) 
{
    u08 lbaOrder[9][16] = {{0,  2,   4,   6,  8,  10, 12, 14,  1,   3,  5,  7,  9,  11,  13, 15},
						   {0,  11, 13,  12, 15,   1,  4,  6,  3,  14,  2,  8, 10,   5,   7,  9},
						   {0,  1,   3,   5,  7,   9, 11, 13, 15,   2, 12, 14,  4,   6,   8, 10},
						   {0,  4,   2,   6,  1,   3,  5,  7,  9,  11,  8, 10, 12,  14,  13, 15},
						   {0,  12, 15,   1,  4,   6,  3, 14, 10,   5,  7,  9, 11,   2,   8, 13},
						   {0,  13, 15,   2,  12, 14,  4,  6,  8,  10,  1,  3,  5,   7,   9, 11},
						   {0,  9,  11,  13,  15,  2,  4,  6,  8,  10, 12, 14,  1,   3,   5,  7},
						   {0,  8,  10,   5,   7,  9, 11, 13, 12,  15,  1,  4,  6,   3,  14,  2},
						   {0,  11, 13,  15,   2,  1,  3,  5,  7,   9, 12, 14,  4,   6,   8, 10}};

    return lbaOrder[lbSeed][lbElement];
}

