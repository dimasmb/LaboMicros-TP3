################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/BufferCirc.c \
../source/DMA.c \
../source/FTM.c \
../source/PWM.c \
../source/buffer.c \
../source/encoder.c \
../source/gpio.c \
../source/receptor2.c \
../source/uart2fsk.c \
../source/uart_drv.c 

OBJS += \
./source/App.o \
./source/BufferCirc.o \
./source/DMA.o \
./source/FTM.o \
./source/PWM.o \
./source/buffer.o \
./source/encoder.o \
./source/gpio.o \
./source/receptor2.o \
./source/uart2fsk.o \
./source/uart_drv.o 

C_DEPS += \
./source/App.d \
./source/BufferCirc.d \
./source/DMA.d \
./source/FTM.d \
./source/PWM.d \
./source/buffer.d \
./source/encoder.d \
./source/gpio.d \
./source/receptor2.d \
./source/uart2fsk.d \
./source/uart_drv.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


