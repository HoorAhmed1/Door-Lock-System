/*
 * motor.c
 *
 *  Created on: Oct 15, 2023
 *      Author: Dell
 */



#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>	/* Include Delay header file */
#include "motor.h"
#include "common_macros.h"
#include "timer_pwm.h"

/*Description:
 *  Set up DC motor input pins (output pins for the microcontroller).
 *  Make the DC motor OFF initially.
 */
void DcMotor_Init(void) {
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_IN1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_IN2_PIN, PIN_OUTPUT);

	/*DC motor initial state is OFF*/
	GPIO_writePin(DC_MOTOR_PORT, DC_IN1_PIN, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT, DC_IN2_PIN, LOGIC_LOW);

}

/*Description:
 *  Configure DC motor state.
 *  Call PWM function with the required duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed) {
	switch(state){
	case Stop:
		GPIO_writePin(DC_MOTOR_PORT, DC_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT, DC_IN2_PIN, LOGIC_LOW);
		break;
	case Clockwise:
		GPIO_writePin(DC_MOTOR_PORT, DC_IN1_PIN, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT, DC_IN2_PIN, LOGIC_LOW);
		break;
	case Anticlockwise:
		GPIO_writePin(DC_MOTOR_PORT, DC_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT, DC_IN2_PIN, LOGIC_HIGH);
		break;
	}

	Timer0_PWM_Init(speed);
}
