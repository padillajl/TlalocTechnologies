/*
 * NRF24L01Driver.h
 *
 *  Created on: Jan 14, 2019
 *      Author: josel
 */

#ifndef NRF24L01DRIVER_H_
#define NRF24L01DRIVER_H_

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "MyTypes.h"
#include "PE_Types.h"
#include "SM1.h"
#include "RF_CE.h"
#include "RF_CSN.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
//Macros to hide low level functionality
#define NRF24L01DRIVER_CE_LOW()		 RF_CE_ClrVal()
#define NRF24L01DRIVER_CE_HIGH()	 RF_CE_SetVal()
#define NRF24L01DRIVER_CSN_LOW()	 RF_CSN_ClrVal()
#define NRF24L01DRIVER_CSN_HIGH()	 RF_CSN_SetVal()


// Command Name Mnemonics (Instructions) 
#define NRF24L01DRIVER_R_REGISTER     0x00
#define NRF24L01DRIVER_W_REGISTER     0x20
#define NRF24L01DRIVER_REGISTER_MASK  0x1F
#define NRF24L01DRIVER_R_RX_PAYLOAD   0x61
#define NRF24L01DRIVER_W_TX_PAYLOAD   0xA0
#define NRF24L01DRIVER_FLUSH_TX       0xE1
#define NRF24L01DRIVER_FLUSH_RX       0xE2
#define NRF24L01DRIVER_REUSE_TX_PL    0xE3
#define NRF24L01DRIVER_NOP            0xFF

/* Memory Map - register address defines */
#define NRF24L01DRIVER_REG_CONFIG      0x00 /* CONFIG register */
#define NRF24L01DRIVER_REG_EN_AA       0x01 /* EN_AA register */
#define NRF24L01DRIVER_REG_EN_RXADDR   0x02 /* EN_RXADDR register */
#define NRF24L01DRIVER_REG_SETUP_AW    0x03 /* SETUP_AW register */
#define NRF24L01DRIVER_REG_SETUP_RETR  0x04
#define NRF24L01DRIVER_REG_RF_CH       0x05
#define NRF24L01DRIVER_REG_RF_SETUP    0x06 /* SETUP register */
#define NRF24L01DRIVER_REG_STATUS      0x07
#define NRF24L01DRIVER_REG_OBSERVE_TX  0x08
//#define RF24_RPD     0x09    /* Mnemonic for nRF24L01+ */
#define NRF24L01DRIVER_CD          0x09   /* Mnemonic from nNRF24L01DRIVERL01, new is RPD */
#define NRF24L01DRIVER_RX_ADDR_P0  0x0A
#define NRF24L01DRIVER_RX_ADDR_P1  0x0B
#define NRF24L01DRIVER_RX_ADDR_P2  0x0C
#define NRF24L01DRIVER_RX_ADDR_P3  0x0D
#define NRF24L01DRIVER_RX_ADDR_P4  0x0E
#define NRF24L01DRIVER_RX_ADDR_P5  0x0F
#define NRF24L01DRIVER_TX_ADDR     0x10
#define NRF24L01DRIVER_RX_PW_P0    0x11
#define NRF24L01DRIVER_RX_PW_P1    0x12
#define NRF24L01DRIVER_RX_PW_P2    0x13
#define NRF24L01DRIVER_RX_PW_P3    0x14
#define NRF24L01DRIVER_RX_PW_P4    0x15
#define NRF24L01DRIVER_RX_PW_P5    0x16
#define NRF24L01DRIVER_FIFO_STATUS 0x17


/* Bit Mnemonics */
/*----------------------------------------------------------------------------------------------------------*/
/* CONFIG Register Bits for address NRF24L01DRIVER_CONFIG*/
#define NRF24L01DRIVER_MASK_RX_DR  (1<<6)  /* Mask interrupt caused by RX_DR: 1: interrupt masked. 0: interrupt enabled */
#define NRF24L01DRIVER_MASK_TX_DS  (1<<5)  /* Mask interrupt caused by TX_DS: 1: interrupt masked. 0: interrupt enabled */
#define NRF24L01DRIVER_MASK_MAX_RT (1<<4)  /* Mask interrupt caused by MAX_RT. 1: interrupt not reflected on IRQ pin. 0: reflect MAX_RT as active low interrupt on IRQ pin */
#define NRF24L01DRIVER_EN_CRC      (1<<3)  /* Enable CRC. Forced high if on of the bits in EN_AA is high */
#define NRF24L01DRIVER_CRCO        (1<<2)  /* CRC encoding scheme, 0: 1 byte, 1: 2 bytes */
#define NRF24L01DRIVER_PWR_UP      (1<<1)  /* 1: Power up, 0: Power down */
#define NRF24L01DRIVER_PRIM_RX     (1<<0)  /* 1: PRX, 0: PTX */
#define NRF24L01DRIVER_PRIM_TX     (0)     /* 0: PTX */

/* CONFIG Register Bits for address NRF24L01DRIVER_EN_AA*/
#define NRF24L01DRIVER_ENAA_P5     5
#define NRF24L01DRIVER_ENAA_P4     4
#define NRF24L01DRIVER_ENAA_P3     3
#define NRF24L01DRIVER_ENAA_P2     2
#define NRF24L01DRIVER_ENAA_P1     1
#define NRF24L01DRIVER_ENAA_P0     0

/* CONFIG Register Bits for address NRF24L01DRIVER_EN_RXADDR*/
#define NRF24L01DRIVER_ERX_P5      5
#define NRF24L01DRIVER_ERX_P4      4
#define NRF24L01DRIVER_ERX_P3      3
#define NRF24L01DRIVER_ERX_P2      2
#define NRF24L01DRIVER_ERX_P1      1
#define NRF24L01DRIVER_ERX_P0      0

/* CONFIG Register Bits for address NRF24L01DRIVER_EN_RXADDR*/
#define NRF24L01DRIVER_AW          0

/* CONFIG Register Bits for address NRF24L01DRIVER_SETUP_RETR*/
#define NRF24L01DRIVER_ARD         4
#define NRF24L01DRIVER_ARC         0

/* CONFIG Register Bits for address NRF24L01DRIVER_RF_CH*/
#define NRF24L01DRIVER_RF_CH       0

/* CONFIG Register Bits for address NRF24L01DRIVER_RF_SETUP*/
#define NRF24L01DRIVER_PLL_LOCK    4
#define NRF24L01DRIVER_RF_DR  	   3
#define NRF24L01DRIVER_RF_PWR      1
#define NRF24L01DRIVER_LNA_HCURR   0

/* CONFIG Register Bits for address NRF24L01DRIVER_STATUS*/
#define NRF24L01DRIVER_RX_DR       6
#define NRF24L01DRIVER_TX_DS       5
#define NRF24L01DRIVER_MAX_RT      4
#define NRF24L01DRIVER_RX_P_NO     1
#define NRF24L01DRIVER_TX_FULL     0

/* CONFIG Register Bits for address NRF24L01DRIVER_OBSERVE_TX*/
#define NRF24L01DRIVER_PLOS_CNT    4
#define NRF24L01DRIVER_ARC_CNT     0

/* CONFIG Register Bits for address NRF24L01DRIVER_FIFO_STATUS*/
#define NRF24L01DRIVER_TX_REUSE    6
#define NRF24L01DRIVER_FIFO_FULL   5
#define NRF24L01DRIVER_TX_EMPTY    4
#define NRF24L01DRIVER_RX_FULL     1
#define NRF24L01DRIVER_RX_EMPTY    0

/* RF_SETUP register bitwise definitions */
#define NRF24L01DRIVER_RF_SETUP_RF_PLL_LOCK    	0xE0
#define NRF24L01DRIVER_RF_SETUP_RF_DR_250    	0x20
#define NRF24L01DRIVER_RF_SETUP_RF_DR_1000    	0x00
#define NRF24L01DRIVER_RF_SETUP_RF_DR_2000    	0x08
#define NRF24L01DRIVER_RF_SETUP_RF_PWR_0    	0x06
#define NRF24L01DRIVER_RF_SETUP_RF_PWR_6    	0x04
#define NRF24L01DRIVER_RF_SETUP_RF_PWR_12    	0x02
#define NRF24L01DRIVER_RF_SETUP_RF_PWR_18    	0x00

/* EN_RXADDR register bitwise definitions */
#define NRF24L01DRIVER_EN_RXADDR_ERX_ALL    	0x3F
#define NRF24L01DRIVER_EN_RXADDR_ERX_P5    		0x20
#define NRF24L01DRIVER_EN_RXADDR_ERX_P4    		0x10
#define NRF24L01DRIVER_EN_RXADDR_ERX_P3 	   	0x08
#define NRF24L01DRIVER_EN_RXADDR_ERX_P2	    	0x04
#define NRF24L01DRIVER_EN_RXADDR_ERX_P1 	   	0x02
#define NRF24L01DRIVER_EN_RXADDR_ERX_P0	    	0x01
#define NRF24L01DRIVER_EN_RXADDR_ERX_NONE    	0x00

/* STATUS register bit definitios */
#define NRF24L01DRIVER_STATUS_RESERVED	    	0x80
#define NRF24L01DRIVER_STATUS_RX_DR		    	0x40
#define NRF24L01DRIVER_STATUS_TX_DS 	   		0x20
#define NRF24L01DRIVER_STATUS_MAX_RT 	   		0x10

/* EN_AA register bitwise definitions */
#define NRF24L01DRIVER_EN_AA_RESERVED			0xC0
#define NRF24L01DRIVER_EN_AA_ENAA_ALL			0x3F
#define NRF24L01DRIVER_EN_AA_ENAA_P5			0x20
#define NRF24L01DRIVER_EN_AA_ENAA_P4			0x10
#define NRF24L01DRIVER_EN_AA_ENAA_P3			0x08
#define NRF24L01DRIVER_EN_AA_ENAA_P2			0x04
#define NRF24L01DRIVER_EN_AA_ENAA_P1			0x02
#define NRF24L01DRIVER_EN_AA_ENAA_P0			0x01
#define NRF24L01DRIVER_EN_AA_ENAA_NONE			0x00

/* SETUP_RETR register bitwise definitions */
#define NRF24L01DRIVER_SETUP_RETR_ARD_4000_US	0xF0 /*4000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_3750_US	0xE0 /*3750 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_3500_US	0xD0 /*3500 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_3250_US	0xC0 /*3250 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_3000_US	0xB0 /*3000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_2750_US	0xA0 /*2750 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_2500_US	0x90 /*2500 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_2250_US	0x80 /*2250 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_2000_US	0x70 /*2000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_1750_US	0x60 /*1750 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_1500_US	0x50 /*1500 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_1250_US	0x40 /*1250 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_1000_US	0x30 /*1000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_750_US	0x20 /*1000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_500_US	0x10 /*1000 us retry delay */
#define NRF24L01DRIVER_SETUP_RETR_ARD_250_US	0x00 /*1000 us retry delay */

#define NRF24L01DRIVER_SETUP_RETR_ARC_15		0x0F /*15 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_14		0x0E /*14 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_13		0x0D /*13 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_12		0x0C /*12 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_11		0x0B /*11 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_10		0x0A /*10 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_09		0x09 /*09 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_08		0x08 /*08 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_07		0x07 /*07 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_06		0x06 /*06 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_05		0x05 /*05 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_04		0x04 /*04 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_03		0x03 /*03 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_02		0x02 /*02 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_01		0x01 /*01 Auto retransmit count*/
#define NRF24L01DRIVER_SETUP_RETR_ARC_00		0x00 /*00 Auto retransmit count, retransmission disabled*/

/* CONFIG register bitwise definitions */
#define NRF24L01DRIVER_CONFIG_RESERVED			0x80
#define NRF24L01DRIVER_CONFIG_MASK_RX_DR		0x40
#define NRF24L01DRIVER_CONFIG_MASK_TX_DS		0x20
#define NRF24L01DRIVER_CONFIG_MASK_MAX_RT		0x10 
#define NRF24L01DRIVER_CONFIG_EN_CRC			0x08 /* Enable CRC */
#define NRF24L01DRIVER_CONFIG_CRCO				0x04 /* 0: CRC 1 byte encoding, 1: CRC 2 bytes encoding */
#define NRF24L01DRIVER_CONFIG_PWR_UP			0x02 /* 1: POWER UP, 0: POWER DOWN */
#define NRF24L01DRIVER_CONFIG_PRIM_RX			0x01 /* RX/TX control 1:PRX, 0:PTX */


/*----------------------------------------------------------------------------------------------------------*/
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	NRF24L01DriverStatusRxDetected
}NRF24L01DriverStatusFlags;
/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/
extern u08 bNRF24L01DriverStatus;
/*************************************************************************************************/
/*********************					Extern Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnNRF24L01DriverInit(void);
void vfnNRF24L01DriverManager(void);
void vfnNRF24L01DriverWriteRegister(u08 lbRegister,u08 lbValue);
void vfnNRF24L01DriverWriteRegisterData(u08 lbRegister,const u08 *lbBufferData,u08 lbBufferSize);
void vfnNRF24L01DriverTxMessage(u08 *lbptrBuffer, u08 lbSize);
void vfnNRF24L01DriverResetStatusIRQ(u08 lbFlags);
/*************************************************************************************************/
#endif /* NRF24L01DRIVER_H_ */
