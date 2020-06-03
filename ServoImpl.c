/*
 * ServoImpl.c
 *
 * Created: 2.6.2020 г. 18:25:47 ч.
 *  Author: user
 */ 

#include <ATMEGA_FreeRTOS.h>
#include <rcServo.h>
#include "Servo.h"


void servo_initialise()
{
	rcServoCreate();		
	
	//printf("Servo is up\n");
}

void servo_open()
{
	rcServoSet(0, 100);
}
void servo_close()
{
	rcServoSet(0, -100);
}