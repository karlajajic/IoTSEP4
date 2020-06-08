#include <ATMEGA_FreeRTOS.h>
#include <rcServo.h>
#include "Servo.h"


void servo_initialise()
{
	rcServoCreate();		
}

void servo_open()
{
	rcServoSet(0, 100);
}
void servo_close()
{
	rcServoSet(0, -100);
}