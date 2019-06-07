/*
 * Accelerometer.c
 *
 *  Created on: 18/10/2017
 *      Author: Toxic
 */


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "fsl_i2c.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "pin_mux.h"
/* Includes used in this file */

/* Own includes */
#include "Accelerometer.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define ACCEL_I2C_CLK_SRC I2C0_CLK_SRC
#define ACCEL_I2C_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24
#define I2C_RELEASE_BUS_COUNT 100
#define I2C_BAUDRATE 100000U

#define MMA8451_WHOAMI 				0x1AU
#define FXOS8700_WHOAMI 			0xC7U
#define ACCEL_STATUS 				0x00U
#define ACCEL_XYZ_DATA_CFG 			0x0EU
#define ACCEL_CTRL_REG1 			0x2AU

/* FXOS8700 and MMA8451 have the same who_am_i register address. */
#define ACCEL_WHOAMI_REG 0x0DU

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
i2c_master_handle_t g_m_handle;
uint8_t g_accel_addr_found = 0x00;
/*  FXOS8700 and MMA8451 device address */
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

volatile bool completionFlag = false;
volatile bool nakFlag = false;



/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
void ACCEL_vfnInit(void)
{
	bool isThereAccel = false;


	BOARD_I2C_ReleaseBus();
	BOARD_I2C_ConfigurePins();

	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, NULL);

	isThereAccel = I2C_ReadAccelWhoAmI();

	if(true == isThereAccel)
	{
		/* Standby */
		I2C_WriteAccelReg(I2C0, g_accel_addr_found , ACCEL_CTRL_REG1, 0);
		I2C_WriteAccelReg(I2C0, g_accel_addr_found , ACCEL_XYZ_DATA_CFG, 0x01);
		I2C_WriteAccelReg(I2C0, g_accel_addr_found , ACCEL_CTRL_REG1, 0x0d);
	}
	PRINTF(" Accel Initialized \r\n");
}
void ACCEL_vfnStart(void)
{
	int16_t x, y, z;
	uint8_t readBuff[7];
	uint8_t status0_value = 0;


	I2C_ReadAccelRegs(I2C0, 0x1D, ACCEL_STATUS, readBuff, 7);

	status0_value = readBuff[0];
	x = ((int16_t)(((readBuff[1] * 256U) | readBuff[2]))) / 4U;
	y = ((int16_t)(((readBuff[3] * 256U) | readBuff[4]))) / 4U;
	z = ((int16_t)(((readBuff[5] * 256U) | readBuff[6]))) / 4U;

	PRINTF("status_reg = 0x%x , x = %5d , y = %5d , z = %5d \r\n", status0_value, x, y, z);

	PRINTF(" Accel Started \r\n");

}
void ACCEL_vfnStop(void)
{
	PRINTF(" Accel Stopped \r\n ");
}


bool I2C_ReadAccelRegs(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t *rxBuff, uint32_t rxSize)
{
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = device_addr;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = reg_addr;
    masterXfer.subaddressSize = 1;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxSize;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /*  direction=write : start+device_write;cmdbuff;xBuff; */
    /*  direction=recive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */
    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag))
    {
    }

    nakFlag = false;

    if (completionFlag == true)
    {
        completionFlag = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool I2C_WriteAccelReg(I2C_Type *base, uint8_t device_addr, uint8_t reg_addr, uint8_t value)
{
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = device_addr;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = reg_addr;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &value;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /*  direction=write : start+device_write;cmdbuff;xBuff; */
    /*  direction=recive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */

    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag))
    {
    }

    nakFlag = false;

    if (completionFlag == true)
    {
        completionFlag = false;
        return true;
    }
    else
    {
        return false;
    }
}
void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0; i < 9; i++)
    {
        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak))
    {
        nakFlag = true;
    }
}

bool I2C_ReadAccelWhoAmI(void)
{
    /*
    How to read the device who_am_I value ?
    Start + Device_address_Write , who_am_I_register;
    Repeart_Start + Device_address_Read , who_am_I_value.
    */
    uint8_t who_am_i_reg = ACCEL_WHOAMI_REG;
    uint8_t who_am_i_value = 0x00;
    uint8_t accel_addr_array_size = 0x00;
    bool find_device = false;
    uint8_t i = 0;
    uint32_t sourceClock = 0;

    i2c_master_config_t masterConfig;

    /*
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    sourceClock = ACCEL_I2C_CLK_FREQ;

    I2C_MasterInit(I2C0, &masterConfig, sourceClock);

    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = g_accel_address[0];
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = &who_am_i_reg;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferNoStopFlag;

    accel_addr_array_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);

    for (i = 0; i < accel_addr_array_size; i++)
    {
        masterXfer.slaveAddress = g_accel_address[i];

        I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

        /*  wait for transfer completed. */
        while ((!nakFlag) && (!completionFlag))
        {
        }

        nakFlag = false;

        if (completionFlag == true)
        {
            completionFlag = false;
            find_device = true;
            g_accel_addr_found = masterXfer.slaveAddress;
            break;
        }
    }

    if (find_device == true)
    {
        masterXfer.direction = kI2C_Read;
        masterXfer.subaddress = 0;
        masterXfer.subaddressSize = 0;
        masterXfer.data = &who_am_i_value;
        masterXfer.dataSize = 1;
        masterXfer.flags = kI2C_TransferRepeatedStartFlag;

        I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

        /*  wait for transfer completed. */
        while ((!nakFlag) && (!completionFlag))
        {
        }

        nakFlag = false;

        if (completionFlag == true)
        {
            completionFlag = false;
            if (who_am_i_value == FXOS8700_WHOAMI)
            {
                PRINTF("Found an FXOS8700 on board , the device address is 0x%x . \r\n", masterXfer.slaveAddress);
                return true;
            }
            else if (who_am_i_value == MMA8451_WHOAMI)
            {
                PRINTF("Found an MMA8451 on board , the device address is 0x%x . \r\n", masterXfer.slaveAddress);
                return true;
            }
            else
            {
                PRINTF("Found a device, the WhoAmI value is 0x%x\r\n", who_am_i_value);
                PRINTF("It's not MMA8451 or FXOS8700. \r\n");
                PRINTF("The device address is 0x%x. \r\n", masterXfer.slaveAddress);
                return false;
            }
        }
        else
        {
            PRINTF("Not a successful i2c communication \r\n");
            return false;
        }
    }
    else
    {
        PRINTF("\r\n Do not find an accelerometer device ! \r\n");
        return false;
    }
}
