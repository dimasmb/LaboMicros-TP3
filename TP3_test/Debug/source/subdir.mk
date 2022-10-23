################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/BufferCirc.c \
../source/DAC.c \
../source/FIR.c \
../source/FSK.c \
../source/SysTick.c \
../source/gpio.c \
../source/template.c \
../source/timer.c \
../source/uart_fsk.c 

OBJS += \
./source/BufferCirc.o \
./source/DAC.o \
./source/FIR.o \
./source/FSK.o \
./source/SysTick.o \
./source/gpio.o \
./source/template.o \
./source/timer.o \
./source/uart_fsk.o 

C_DEPS += \
./source/BufferCirc.d \
./source/DAC.d \
./source/FIR.d \
./source/FSK.d \
./source/SysTick.d \
./source/gpio.d \
./source/template.d \
./source/timer.d \
./source/uart_fsk.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


