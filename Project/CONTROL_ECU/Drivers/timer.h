 /******************************************************************************
 *
 *
 * File Name: timer1.h
 *
 * Description: Header file for the timer1 driver
 *
 * Author: Habiba
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum{
	no_prescale, prescale_1, prescale_8 , prescale_64  ,
	prescale_256 , prescale_1024
}Timer1_Prescaler;

typedef enum{
	normal_mode=0, ctc_mode=4
}Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBackFunction(void(*a_ptr)(void));


#endif /* TIMER1_H_ */
