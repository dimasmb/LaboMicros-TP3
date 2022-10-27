################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC.c \
../source/App.c \
../source/BufferCirc.c \
../source/DAC.c \
../source/FIR.c \
../source/FSK.c \
../source/SysTick.c \
../source/gpio.c \
../source/uart2fsk.c \
../source/uart_drv.c 

OBJS += \
./source/ADC.o \
./source/App.o \
./source/BufferCirc.o \
./source/DAC.o \
./source/FIR.o \
./source/FSK.o \
./source/SysTick.o \
./source/gpio.o \
./source/uart2fsk.o \
./source/uart_drv.o 

C_DEPS += \
./source/ADC.d \
./source/App.d \
./source/BufferCirc.d \
./source/DAC.d \
./source/FIR.d \
./source/FSK.d \
./source/SysTick.d \
./source/gpio.d \
./source/uart2fsk.d \
./source/uart_drv.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


