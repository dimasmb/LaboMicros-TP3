################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/BufferCirc.c \
../source/DMA.c \
../source/FSK.c \
../source/FTM.c \
../source/PIT.c \
../source/PWM.c \
../source/SysTick.c \
../source/gpio.c \
../source/template.c \
../source/timer.c 

OBJS += \
./source/BufferCirc.o \
./source/DMA.o \
./source/FSK.o \
./source/FTM.o \
./source/PIT.o \
./source/PWM.o \
./source/SysTick.o \
./source/gpio.o \
./source/template.o \
./source/timer.o 

C_DEPS += \
./source/BufferCirc.d \
./source/DMA.d \
./source/FSK.d \
./source/FTM.d \
./source/PIT.d \
./source/PWM.d \
./source/SysTick.d \
./source/gpio.d \
./source/template.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


