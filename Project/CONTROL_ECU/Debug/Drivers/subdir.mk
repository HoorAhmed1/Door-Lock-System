################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/buzzer.c \
../Drivers/external_eeprom.c \
../Drivers/gpio.c \
../Drivers/i2c.c \
../Drivers/motor.c \
../Drivers/timer.c \
../Drivers/timer_pwm.c \
../Drivers/uart.c 

OBJS += \
./Drivers/buzzer.o \
./Drivers/external_eeprom.o \
./Drivers/gpio.o \
./Drivers/i2c.o \
./Drivers/motor.o \
./Drivers/timer.o \
./Drivers/timer_pwm.o \
./Drivers/uart.o 

C_DEPS += \
./Drivers/buzzer.d \
./Drivers/external_eeprom.d \
./Drivers/gpio.d \
./Drivers/i2c.d \
./Drivers/motor.d \
./Drivers/timer.d \
./Drivers/timer_pwm.d \
./Drivers/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c Drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


