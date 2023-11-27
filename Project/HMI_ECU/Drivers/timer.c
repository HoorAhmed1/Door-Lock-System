 /******************************************************************************
 *
 *
 * File Name: timer1.c
 *
 * Description: Source file for the timer1 driver
 *
 * Author: Habiba
 *
 *******************************************************************************/

#include <util/delay.h> /* For the delay functions */
#include <avr/io.h>
#include "common_macros.h" /* For GET_BIT Macro */
#include "timer.h"
#include "gpio.h"
#include <avr/interrupt.h>


static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	if (Config_Ptr -> mode == ctc_mode)
	{
		SET_BIT(TIMSK, OCIE1A); /* Enable Timer1 Compare A Interrupt */
		OCR1A = Config_Ptr-> compare_value;

	}
	if (Config_Ptr->mode == normal_mode) {
		TIMSK |= (1 << TOIE0); // Enable Timer0 Overflow Interrupt
	}
	// 1110 0111
	TCCR1A = (1<<FOC1A);

	//Mode
	TCCR1A = (TCCR1A & 0xFC) | (Config_Ptr->mode);
	TCCR1B = (TCCR1B & 0xE7) | (Config_Ptr->mode);

	//prescaler 1111 1000
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);

	TCNT1 = Config_Ptr->initial_value;

}


void Timer1_deInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	g_callBackPtr = NULL_PTR;

}

void Timer1_setCallBackFunction(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}


ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
