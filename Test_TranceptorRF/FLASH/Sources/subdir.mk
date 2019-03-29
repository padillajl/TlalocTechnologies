################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Events.c" \
"../Sources/MainDriver.c" \
"../Sources/MiscFunctions.c" \
"../Sources/NRF24L01Driver.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/Events.c \
../Sources/MainDriver.c \
../Sources/MiscFunctions.c \
../Sources/NRF24L01Driver.c \
../Sources/main.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/Events.o \
./Sources/MainDriver.o \
./Sources/MiscFunctions.o \
./Sources/NRF24L01Driver.o \
./Sources/main.o \
./Sources/sa_mtb.o \

C_DEPS += \
./Sources/Events.d \
./Sources/MainDriver.d \
./Sources/MiscFunctions.d \
./Sources/NRF24L01Driver.d \
./Sources/main.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/Events.o" \
"./Sources/MainDriver.o" \
"./Sources/MiscFunctions.o" \
"./Sources/NRF24L01Driver.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/Events.d" \
"./Sources/MainDriver.d" \
"./Sources/MiscFunctions.d" \
"./Sources/NRF24L01Driver.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/Events.o \
./Sources/MainDriver.o \
./Sources/MiscFunctions.o \
./Sources/NRF24L01Driver.o \
./Sources/main.o \
./Sources/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Events.o: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Events.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Events.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MainDriver.o: ../Sources/MainDriver.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MainDriver.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MainDriver.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MiscFunctions.o: ../Sources/MiscFunctions.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MiscFunctions.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MiscFunctions.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NRF24L01Driver.o: ../Sources/NRF24L01Driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/NRF24L01Driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/NRF24L01Driver.o"
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


