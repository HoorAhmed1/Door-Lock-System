################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/gpio.c \
../Drivers/keypad.c \
../Drivers/lcd.c \
../Drivers/timer.c \
../Drivers/uart.c 

OBJS += \
./Drivers/gpio.o \
./Drivers/keypad.o \
./Drivers/lcd.o \
./Drivers/timer.o \
./Drivers/uart.o 

C_DEPS += \
./Drivers/gpio.d \
./Drivers/keypad.d \
./Drivers/lcd.d \
./Drivers/timer.d \
./Drivers/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c Drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


