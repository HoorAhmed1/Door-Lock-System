/*
 * CONTROL_ECU_MC.c
 *
 *  Created on: Nov 6, 2023
 *      Author: Dell
 */

#include "Drivers/timer.h"
#include "Drivers/buzzer.h"
#include "Drivers/common_macros.h"
#include "Drivers/gpio.h"
#include "Drivers/std_types.h"
#include "Drivers/motor.h"
#include "Drivers/uart.h"
#include "Drivers/i2c.h"
#include "Drivers/external_eeprom.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define MOTOR_SPEED 100
#define PASSWORD_ADDRESS_LOCATION 0x50

int password_is_set = 0;
int PASS_SIZE  = 6;
uint8 saved_password[6];
char* ask_for_password = "plz enter pass: ";
int g_ticks = 0;

Timer1_ConfigType timer_configurations = { 0, 8000, prescale_64, ctc_mode };

typedef enum {
	CREATE_PASSWORD, VALIDATE_PASSWORD, OPEN_DOOR, CHANGE_PASSWORD, UART_ERROR
}UART_message;

int compare(uint8 pass1[PASS_SIZE], uint8 pass2[PASS_SIZE]){
	for(int i=0; i<PASS_SIZE; i++){
		if(pass1[i]!=pass2[i])
			return -1;
	}
	return 0;
}

int validate_password(char p1[PASS_SIZE]){
	for(int i=0; i<5; i++){
		if(p1[i]!=saved_password[i])
			return 0;
	}
	return 1;
}
void save_password(uint8 password[PASS_SIZE]){
	for(int i=0; i<PASS_SIZE; i++){
		uint8 state = EEPROM_writeByte(PASSWORD_ADDRESS_LOCATION + i, password[i]);
		_delay_ms(400);
	}
}

void createPassword(){
	uint8 password1[PASS_SIZE], password2[PASS_SIZE];
	//UART_recieveString(password1);
	uint8 rec_byte = '-';
	for(int i=0; i<PASS_SIZE; i++){
		rec_byte = UART_recieveByte();
		password1[i] = rec_byte;
		_delay_ms(100);
	}

	//GPIO_writePin(PORTD_ID, PIN7_ID, 1);
	_delay_ms(300);
	//GPIO_writePin(PORTD_ID, PIN7_ID, 0);

	uint8 rec_byte2 = '-';
	for(int i=0; i<PASS_SIZE; i++){
		rec_byte2 = UART_recieveByte();
		password2[i] = rec_byte2;
		_delay_ms(100);
	}

	//GPIO_writePin(PORTD_ID, PIN7_ID, 1);
	_delay_ms(300);
	//GPIO_writePin(PORTD_ID, PIN7_ID, 0);

	int match = compare(password1,password2);
	if(match==0){
		UART_sendByte(1);
		save_password(password2);
		password_is_set = 1;
	}
	else{
		while(match != 0){
			UART_sendByte(0);
			//_delay_ms(500);
			for(int i=0; i<PASS_SIZE ; i++){
				rec_byte = UART_recieveByte();
				password2[i] = rec_byte;
				_delay_ms(100);
			}
			match = compare(password2,password1);
		}
		save_password(password2);
		UART_sendByte(1);
		password_is_set = 1;
	}


}
void countOneSecond()
{
	/* the call back function to count 1 second */
	g_ticks++;
	if(g_ticks%1==0){
		GPIO_writePin(PORTD_ID, PIN7_ID, 1);
	}else{
		GPIO_writePin(PORTD_ID, PIN7_ID, 0);
	}
}
void countSeconds(uint8 seconds){
	// start counting seconds
	//Timer1_ConfigType Timer1_Config = {0,31249,PRESCALER_256,COMPARE_MODE};
	Timer1_setCallBackFunction(countOneSecond);
	Timer1_init(&timer_configurations);

	g_ticks = 0;
	while (g_ticks < seconds) {
	}

	Timer1_deInit();
	g_ticks=0;
}

void alarmBuzzer(){
	Buzzer_on();
	countSeconds(60);
	Buzzer_off();
}
int compare_to_stored(uint8 pass[PASS_SIZE]){
	uint8 data;
	for(int i=0; i<PASS_SIZE; i++){
		uint8 result = EEPROM_readByte(PASSWORD_ADDRESS_LOCATION + i,&data);
		if(data != pass[i])
			return -1;
	}
	return 0 ;
}
void verifyPassword(){
	uint8 valid=1;
	uint8 password_to_check[PASS_SIZE];
	uint8 byte_rec = '-';
	for(int i=0; i<PASS_SIZE; i++){
		byte_rec = UART_recieveByte();
		password_to_check[i] = byte_rec;
		_delay_ms(100);
	}
	valid = compare_to_stored(password_to_check);
	if(valid==0)
		UART_sendByte(1);
	else
		UART_sendByte(0);
}

void openDoor(){
	/*rotate clockwise for 15 seconds (Unlocking the door)*/
	DcMotor_Rotate(Clockwise, 99);
	countSeconds(15);

	DcMotor_Rotate(Stop, 0);
}
void closeDoor(){
	/*rotate clockwise for 15 seconds (locking the door)*/
	DcMotor_Rotate(Anticlockwise, 99);
	countSeconds(15);
	DcMotor_Rotate(Stop, 0);
}

int main(void){

	SREG |= (1<<7);
	UART_ConfigType UART_Config = {EIGHT,DISABLED,ONE,BAUD_RATE_9600};
	TWI_ConfigType twi_config_ptr = { address_1, bit_rate_400Kbits };
	Timer1_ConfigType timer_configurations = { 0, 8000, prescale_8, ctc_mode };

	TWI_init(&twi_config_ptr);
	DcMotor_Init();
	Buzzer_init();
	UART_init(&UART_Config);

	Timer1_setCallBackFunction(countOneSecond);

	while(1){
		uint8 choice = UART_recieveByte();
		//GPIO_writePin(PORTA_ID, PIN0_ID, 1);
		_delay_ms(100);
		//GPIO_writePin(PORTA_ID, PIN0_ID, 0);
		if(choice == CREATE_PASSWORD){
			createPassword();
		}
		if(choice == VALIDATE_PASSWORD){
			verifyPassword();
		}
		if(choice == OPEN_DOOR){
			openDoor();
			countSeconds(3);
			closeDoor();
		}
		if(choice == UART_ERROR){
			alarmBuzzer();
		}
	}
	return 0;
}
