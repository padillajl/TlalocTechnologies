################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ESP8266Driver.c" \
"../Sources/Events.c" \
"../Sources/MainDriver.c" \
"../Sources/Shell.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/ESP8266Driver.c \
../Sources/Events.c \
../Sources/MainDriver.c \
../Sources/Shell.c \
../Sources/main.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/ESP8266Driver.o \
./Sources/Events.o \
./Sources/MainDriver.o \
./Sources/Shell.o \
./Sources/main.o \
./Sources/sa_mtb.o \

C_DEPS += \
./Sources/ESP8266Driver.d \
./Sources/Events.d \
./Sources/MainDriver.d \
./Sources/Shell.d \
./Sources/main.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/ESP8266Driver.o" \
"./Sources/Events.o" \
"./Sources/MainDriver.o" \
"./Sources/Shell.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/ESP8266Driver.d" \
"./Sources/Events.d" \
"./Sources/MainDriver.d" \
"./Sources/Shell.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/ESP8266Driver.o \
./Sources/Events.o \
./Sources/MainDriver.o \
./Sources/Shell.o \
./Sources/main.o \
./Sources/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ESP8266Driver.o: ../Sources/ESP8266Driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ESP8266Driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ESP8266Driver.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Events.o: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Events.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Events.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MainDriver.o: ../Sources/MainDriver.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MainDriver.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MainDriver.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Shell.o: ../Sources/Shell.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Shell.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Shell.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


