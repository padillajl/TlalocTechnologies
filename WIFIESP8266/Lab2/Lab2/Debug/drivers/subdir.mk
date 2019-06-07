################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_freertos.c \
../drivers/fsl_lpsci.c \
../drivers/fsl_lpsci_freertos.c \
../drivers/fsl_smc.c \
../drivers/fsl_tpm.c \
../drivers/fsl_uart.c \
../drivers/fsl_uart_freertos.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_freertos.o \
./drivers/fsl_lpsci.o \
./drivers/fsl_lpsci_freertos.o \
./drivers/fsl_smc.o \
./drivers/fsl_tpm.o \
./drivers/fsl_uart.o \
./drivers/fsl_uart_freertos.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_freertos.d \
./drivers/fsl_lpsci.d \
./drivers/fsl_lpsci_freertos.d \
./drivers/fsl_smc.d \
./drivers/fsl_tpm.d \
./drivers/fsl_uart.d \
./drivers/fsl_uart_freertos.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -D__REDLIB__ -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\source" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\startup" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\utilities" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\drivers" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\frdmkl25z" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\freertos" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


