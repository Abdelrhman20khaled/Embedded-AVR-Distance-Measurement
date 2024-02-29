/*
 * ultrasonic.c
 *
 *  Created on: 18 Oct 2023
 *      Author: Abdelrhman Khaled Sobhi
 *      Group : 79
 */

#include "ultrasonic.h"
#include "util/delay.h"
#include "icu.h"
#include "gpio.h"
#include "std_types.h"
#include "avr/io.h"

// this global variable for in the function and they used in many functions
static uint8 edges_count = 0;
static uint16 echo_time = 0;

// this structure will passed to ICU driver to setup the ICU
ICU_ConfigType ultrasonic_config = {F_CPU_8,RAISING};

/*
 * Description:
 * 1- when this function calling by ICU driver this will increment the number of
 * 	  edges by 1 and check about the number of edges.
 * 2- if the number of edges is 1 that means the raising edge detected by and the
 *	  it i the start edge, then clear time from TCNT1 register and setup the ICU
 *	  to work with falling edge.
 * 3- if the number of edges reach to 2 that means the falling edge apply to the pin
 *    and then save the value in ICR register in global variable and clear time again
 */

void Ultrasonic_edgeProcessing (void){

	edges_count++;

	if(edges_count == 1){
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
	}

	else if(edges_count == 2){
		echo_time = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RAISING);
		ICU_clearTimerValue();
	}
}

/*
 * Description:
 * 1- setup the ICU driver by call ICU_init function.
 * 2- setup the callback function that defines before this function.
 * 3- setup the trigger pin direction to output pin for send 10us pulse.
 *
 */
void Ultrasonic_init(void){

	/* this function defined in ICU driver that take a pointer to type of structure
	 * this structure was configured to take a edge and prescalar frequency to setup
	 * the ICU according to the frequency the application needs.
	 */

	ICU_init(&ultrasonic_config);

	/* this function take a pointer to function (name of the function) to call this
	 * function when the specific edge is applied to ICP, it will implement the
	 * function that it is name is passed as an argument to the call bake function.
	 */

	ICU_setCallBack(Ultrasonic_edgeProcessing);

	// setup the direction of the trigger pin as output pin.

	GPIO_setupPinDirection(TRIGGER_PORT,TRIGGER_PIN,PIN_OUTPUT);
}

/*
 *  Description:
 *  this function used for send the pulse function used for sending the 10us pulse
 *  by put logic high on the pin for 10 us or more then apply logic low on the
 *  signal again.
 *
 */

void Ultrasonic_Trigger(void){

	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_HIGH);
	// delay 10us or more.
	_delay_us(50);
	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_LOW);

}

/*
 * Description:
 * 1- this function call trigger function for send the 10us pulse.
 * 2- wait until the falling edge for calculate the all time of the Echo.
 * 3- reset the number of edges for the measurement.
 * 4- return the value of distance equation to use it in application layer.
 *
 */

uint16 Ultrasonic_readDistance(void){

	// call the trigger function for send the pulse with 10us pulse to start measure
	Ultrasonic_Trigger();
	// wait until falling edge come (listen to echo)
	while(edges_count != 2);
	//reset the number of edges for new measure.
	edges_count = 0;
	/*
	 * return the distance value to use it in application layer and add 1 to return
	 * the real value of the distance an avoid any wrong in distance
	 */
	return (echo_time/58)+1;
}


