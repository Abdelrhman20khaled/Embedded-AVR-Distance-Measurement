/*
 * ultrasonic_main.c
 *
 *  Created on: 19 Oct 2023
 *      Author: AbdoS
 */
#include "ultrasonic.h"
#include "lcd.h"
#include "std_types.h"
#include "avr/io.h" // include it for enable I - BIT

int main(){
	//this global variable for
	uint16 distance=0;
	// Enable I - Bit for ICU
	SREG |= (1<<7);
	// Calling the Ultra-sonic initialization function
	Ultrasonic_init();
	// Calling the LCD initialization function
	LCD_init();
	// Display as default the Distance in cm on LCD
	LCD_displayString("Distance=    cm");

	while(1){
		// received the measured value of ultr-sonic
		distance = Ultrasonic_readDistance();
		// display the measured value in specific location
		LCD_moveCursor(0,10);
		// this check will display in each case by it for avoiding any error
	    if(distance >=100){
	    	LCD_intgerToString(distance);
	    }
	    else{
	    	LCD_intgerToString(distance);
	    	LCD_displayCharacter(' ');
	    }
	}

}

