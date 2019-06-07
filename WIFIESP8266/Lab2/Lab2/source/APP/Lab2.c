/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Lab2.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "fsl_shell.h"
#include "task.h"
#include "fsl_debug_console.h"
#include "Shell.h"


/* TODO: insert other include files here. */
#define KEY_ESC (0x1BU)
#define KET_DEL (0x7FU)

#define LED_NUMBERS        3U
#define LED_3_INIT()       LED_RED_INIT(LOGIC_LED_OFF)
#define LED_2_INIT()       LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED_1_INIT()       LED_BLUE_INIT(LOGIC_LED_OFF)

/* TODO: insert other definitions and declarations here. */
static int32_t ACCEL_Control(p_shell_context_t context, int32_t argc, char **argv);
static int32_t Clear(p_shell_context_t context, int32_t argc, char **argv);
static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv);



static const shell_command_context_t xAccelerometerCommand = {"accel",
                                                    "\r\n\"Accel arg1 \":\r\n Usage:\r\n    arg1: start|stop|init...         "
                                                    "   \r\n           \r\n",ACCEL_Control, 1};

static const shell_command_context_t xClearCommand = {"clear",
                                                    "\r\n\" Clear screen \r\n ",Clear, 0};

static const shell_command_context_t xLedCommand = {"led",
                                                    "\r\n\"led arg1 \":\r\n Usage:\r\n    arg1: White|Red|Blue|Yellow"
                                                    " \r\n",
                                                    LedControl, 1};

void vfnShellTask(void);
void vfnAccelerometerTask(void);
void vfnApplicationTask( void * pvParameters );
void Led_Init(void);

/*
 * @brief   Application entry point.
 */
int main(void) {

	shell_context_struct user_context;

  	/* Init board hardware. */
	BOARD_InitPins();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    Led_Init();

    /* Init SHELL */
	SHELL_Init(&user_context, SHELL_SendDataCallback, SHELL_ReceiveDataCallback, PRINTF, ">> ");

	/* Add new command to commands list */
	SHELL_RegisterCommand(&xAccelerometerCommand);
	SHELL_RegisterCommand(&xClearCommand);
	SHELL_RegisterCommand(&xLedCommand);

	SHELL_Main(&user_context);
    while(1) {

    }

    return 0;
}



void vfnShellTask(void)
{

}


static int32_t ACCEL_Control(p_shell_context_t context, int32_t argc, char **argv)
{
	char *AccelCommand = argv[1];

	 if (strcmp(AccelCommand, "start") == 0)
	 {
		 ACCEL_vfnStart();

	 }
	 else if (strcmp(AccelCommand, "stop") == 0)
	 {
		 ACCEL_vfnStop();
	 }
	 else if (strcmp(AccelCommand, "init") == 0)
	 {
		 ACCEL_vfnInit();
	 }
	 else
	 {
		 PRINTF("Control conmmand is wrong!\r\n");
	 }
	 return 0;
}

static int32_t Clear(p_shell_context_t context, int32_t argc, char **argv)
{
	PRINTF(" \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n");
	PRINTF(" \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n");
	PRINTF(" \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n \r\n");
	return 0;
}

static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv)
{
	char *kLedColor = argv[1];


    if ((strcmp(kLedColor, "red") == 0) )
	{
		LED_RED_ON();
		LED_BLUE_OFF();
		LED_GREEN_OFF();
	}

	else if ((strcmp(kLedColor, "blue") == 0) )
	{
		LED_RED_OFF();
		LED_BLUE_ON();
		LED_GREEN_OFF();
	}

	else if ((strcmp(kLedColor, "green") == 0) )
	{
		LED_RED_OFF();
		LED_BLUE_OFF();
		LED_GREEN_ON();
	}
	else if ((strcmp(kLedColor, "purple") == 0) )
	{
		LED_RED_ON();
		LED_BLUE_ON();
		LED_GREEN_ON();
	}
	else if ((strcmp(kLedColor, "orange") == 0) )
	{
		LED_RED_ON();
		LED_BLUE_OFF();
		LED_GREEN_ON();
	}


    return 0;
}

void Led_Init(void)
{
    LED_1_INIT();
    LED_2_INIT();
    LED_3_INIT();
}

