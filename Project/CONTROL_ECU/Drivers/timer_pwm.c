/*
 * Timer.c
 *
 *  Created on: Oct 16, 2023
 *      Author: Hour Ahmed
 */
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>
#include "timer_pwm.h"
#include "gpio.h"
#include "common_macros.h"

void Timer0_PWM_Init(uint8 duty_cycle_percent)
{

	TCNT0 = 0; // Set Timer Initial Value to 0
	uint8 duty_cycle = (uint16)(255*duty_cycle_percent)/100;
	OCR0  = duty_cycle; //Set Compare value

	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT );

	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,WGM01);
	SET_BIT(TCCR0,COM01);
	SET_BIT(TCCR0,CS01);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

}
