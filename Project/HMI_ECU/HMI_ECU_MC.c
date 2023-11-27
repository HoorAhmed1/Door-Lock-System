/*
 * Project_4.c
 *
 *  Created on: Nov 5, 2023
 *      Author: Hour
 */

#include "Drivers/uart.h"
#include "Drivers/common_macros.h"
#include "Drivers/gpio.h"
#include "Drivers/lcd.h"
#include "Drivers/keypad.h"
#include "Drivers/std_types.h"
#include "Drivers/timer.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>


int PASS_SIZE  = 6;
char* ask_for_password = "plz enter pass: ";
int g_ticks = 0;

Timer1_ConfigType timer_configurations = { 0, 8000, prescale_64, ctc_mode };

typedef enum {
	CREATE_PASSWORD, VALIDATE_PASSWORD, OPEN_DOOR, CHANGE_PASSWORD, UART_ERROR
}UART_message ;

int Password_Set = 0; //global variable to check if password is set;

void read_password(char pass[PASS_SIZE], char* state){
	//_delay_ms(500);
	int i=0;
	char digit;
	//LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,state);
	_delay_ms(500);
	while(i<PASS_SIZE){
		digit = KEYPAD_getPressedKey();
		if(digit!='=' && isdigit(digit)){
			LCD_moveCursor(1,i+1);
			LCD_displayCharacter('*');
		}
		if(digit == '='){
			if(i>=5)
				break;
			else{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,state);
				i=-1;
				_delay_ms(500);
			}
		}
		pass[i] = digit;
		//UART_sendByte(pass[i]);
		i++;
		_delay_ms(500);
	}
	_delay_ms(500);
	pass[5]='#';
	//LCD_displayStringRowColumn(1,0,pass);
}

void create_password(char password[PASS_SIZE], char* state){

	LCD_displayStringRowColumn(0,0,state);
	read_password(password, state);
	//_delay_ms(500);
	//_delay_ms(500);

	for(int i=0; i<PASS_SIZE; i++){
		UART_sendByte(password[i]);
		_delay_ms(100);
	}

	char* confirm_password = "enter same pass:";
	char password2[PASS_SIZE];

	_delay_ms(500);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,confirm_password);
	read_password(password2, confirm_password);
	password2[5]='#';

	//UART_sendString((uint8*)password2);
	for(int i=0; i<PASS_SIZE; i++){
		UART_sendByte(password2[i]);
		_delay_ms(100);
	}

	uint8 valid;
	valid = UART_recieveByte();
	while(valid == 0){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,confirm_password);
		read_password(password2, confirm_password);
		password2[5]='#';
		//_delay_ms(100);
		//UART_sendString((uint8*)password2);
		for(int i=0; i<PASS_SIZE; i++){
			UART_sendByte(password2[i]);
			_delay_ms(100);
		}
		_delay_ms(100);
		valid = UART_recieveByte();
	}
	Password_Set=1;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"PASS SET");
	_delay_ms(500);

	return;
}


void display_main_screen(){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0, "+ : Open Door");
	LCD_displayStringRowColumn(1,0, "- : Change Pass");
}
//void save_password(char pass[7]){
//	Password = pass;
//}

//send password to another control
uint8 verify_user(){
	char password [PASS_SIZE];
	LCD_clearScreen();
	read_password(password, ask_for_password);

	//UART_sendString((uint8*)password);
	for(int i=0; i<PASS_SIZE; i++){
		UART_sendByte(password[i]);
		_delay_ms(100);
	}
	uint8 state = UART_recieveByte();
	if(state==1)
		return state;
	else{
		int counter=0;
		while(state == 0 && counter<2){
			//_delay_ms(100);
			LCD_clearScreen();
			read_password(password, ask_for_password);
			UART_sendByte(VALIDATE_PASSWORD);
			for(int i=0; i<PASS_SIZE; i++){
				UART_sendByte(password[i]);
				_delay_ms(100);
			}
			state = UART_recieveByte();
			counter++;
		}
	}

	return state;
}


void countOneSecond()
{
	/* the call back function to count 1 second */
	g_ticks++;
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

void openCloseDoor(){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door opening");
	// start counting 15 seconds
	countSeconds(15);

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"HOLD");
	countSeconds(3);
	LCD_clearScreen();

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door closing");
	countSeconds(15);
	LCD_clearScreen();
}

void displayError(){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"LOCKED");
	countSeconds(60);
	LCD_clearScreen();
}
int main(void){
	SREG|=(1<<7);

	UART_ConfigType UART_config = {EIGHT, DISABLED, ONE, BAUD_RATE_9600  };

	LCD_init();
	UART_init(&UART_config);
	//	uint8 dummy = UART_recieveByte();
	//	UART_sendByte(1);
	// Timer1_setCallBackFunction(countOneSecond);
	while(1){
		char pass[7];
		if(Password_Set==0){
			UART_sendByte(CREATE_PASSWORD);
			create_password(pass,ask_for_password);
		}
		else{
			display_main_screen();
			uint8 choice='0';
			while(1){
				choice = KEYPAD_getPressedKey();
				if(choice == '+' || choice == '-'){
					_delay_ms(100);
					break;
				}

			}
			UART_sendByte(VALIDATE_PASSWORD);
			//_delay_ms(500);
			int verified = verify_user();

			if(verified==0){
				UART_sendByte(UART_ERROR);
				displayError();
			}
			else{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Verified");
				//_delay_ms(200);
				if(choice == '+'){
					UART_sendByte(OPEN_DOOR);
					openCloseDoor();
				}
				else if(choice == '-'){
					UART_sendByte(CREATE_PASSWORD);
					char password[7];
					create_password(password, "Enter new pass:");

				}
			}
		}
		//LCD_clearScreen();
		//LCD_displayStringRowColumn(1,1,pass);
		void display_main_screen();

	}
	return 0;
}
