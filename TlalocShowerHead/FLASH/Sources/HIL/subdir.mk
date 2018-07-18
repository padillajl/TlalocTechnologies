################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/HIL/InfraredSensors.c" \
"../Sources/HIL/LCD.c" \
"../Sources/HIL/SensorsControl.c" \
"../Sources/HIL/SwTimers.c" \

C_SRCS += \
../Sources/HIL/InfraredSensors.c \
../Sources/HIL/LCD.c \
../Sources/HIL/SensorsControl.c \
../Sources/HIL/SwTimers.c \

OBJS += \
./Sources/HIL/InfraredSensors.o \
./Sources/HIL/LCD.o \
./Sources/HIL/SensorsControl.o \
./Sources/HIL/SwTimers.o \

C_DEPS += \
./Sources/HIL/InfraredSensors.d \
./Sources/HIL/LCD.d \
./Sources/HIL/SensorsControl.d \
./Sources/HIL/SwTimers.d \

OBJS_QUOTED += \
"./Sources/HIL/InfraredSensors.o" \
"./Sources/HIL/LCD.o" \
"./Sources/HIL/SensorsControl.o" \
"./Sources/HIL/SwTimers.o" \

C_DEPS_QUOTED += \
"./Sources/HIL/InfraredSensors.d" \
"./Sources/HIL/LCD.d" \
"./Sources/HIL/SensorsControl.d" \
"./Sources/HIL/SwTimers.d" \

OBJS_OS_FORMAT += \
./Sources/HIL/InfraredSensors.o \
./Sources/HIL/LCD.o \
./Sources/HIL/SensorsControl.o \
./Sources/HIL/SwTimers.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/HIL/InfraredSensors.o: ../Sources/HIL/InfraredSensors.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/HIL/InfraredSensors.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/HIL/InfraredSensors.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/HIL/LCD.o: ../Sources/HIL/LCD.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/HIL/LCD.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/HIL/LCD.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/HIL/SensorsControl.o: ../Sources/HIL/SensorsControl.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/HIL/SensorsControl.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/HIL/SensorsControl.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/HIL/SwTimers.o: ../Sources/HIL/SwTimers.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/HIL/SwTimers.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/HIL/SwTimers.o"
	@echo 'Finished building: $<'
	@echo ' '


