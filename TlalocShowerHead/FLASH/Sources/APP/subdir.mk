################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/APP/MiscFunctions.c" \
"../Sources/APP/main.c" \
"../Sources/APP/sa_mtb.c" \

C_SRCS += \
../Sources/APP/MiscFunctions.c \
../Sources/APP/main.c \
../Sources/APP/sa_mtb.c \

OBJS += \
./Sources/APP/MiscFunctions.o \
./Sources/APP/main.o \
./Sources/APP/sa_mtb.o \

C_DEPS += \
./Sources/APP/MiscFunctions.d \
./Sources/APP/main.d \
./Sources/APP/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/APP/MiscFunctions.o" \
"./Sources/APP/main.o" \
"./Sources/APP/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/APP/MiscFunctions.d" \
"./Sources/APP/main.d" \
"./Sources/APP/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/APP/MiscFunctions.o \
./Sources/APP/main.o \
./Sources/APP/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/APP/MiscFunctions.o: ../Sources/APP/MiscFunctions.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/APP/MiscFunctions.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/APP/MiscFunctions.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/APP/main.o: ../Sources/APP/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/APP/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/APP/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/APP/sa_mtb.o: ../Sources/APP/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/APP/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/APP/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '

