################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mkl25z4.c 

OBJS += \
./startup/startup_mkl25z4.o 

C_DEPS += \
./startup/startup_mkl25z4.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -D__REDLIB__ -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\source" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\startup" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\utilities" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\drivers" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\frdmkl25z" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\freertos" -I"C:\Users\Toxic\Desktop\Ing. Biomedica\Proyectos en MCUXPresso\Lab2\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

