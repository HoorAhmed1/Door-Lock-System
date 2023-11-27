/*
 * motor.h
 *
 *  Created on: Oct 15, 2023
 *      Author: Dell
 */
#include "gpio.h"

#ifndef MOTOR_H_
#define MOTOR_H_
#define DC_MOTOR_PORT PORTB_ID
#define DC_IN1_PIN PIN0_ID
#define DC_IN2_PIN PIN1_ID

/*DC motor states, it is either OFF ,clockwise or anti-clockWise*/
typedef enum {
	Stop, Clockwise, Anticlockwise
} DcMotor_State;

/*Description:
 *  Set up DC motor input pins (output pins for the microcontroller).
 *  Make the DC motor OFF initially.
 */
void DcMotor_Init(void);

/*Description:
 *  Configure DC motor state.
 *  Call PWM function with the required duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
