/*
 * ultrasonic.h
 *
 *  Created on: 18 Oct 2023
 *      Author: Abdelrhman Khaled Sobhi
 *      Group : 79
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "gpio.h"
#include "std_types.h"

/*********************************************************************
 * 						    Definitions							     *
 *********************************************************************/

#define TRIGGER_PORT PORTB_ID

#define TRIGGER_PIN  PIN5_ID

/*********************************************************************
 * 						     Functions							     *
 *********************************************************************/

void Ultrasonic_init(void);

void Ultrasonic_Trigger(void);

uint16 Ultrasonic_readDistance(void);

void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
