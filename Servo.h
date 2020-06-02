/*
 * Servo.h
 *
 * Created: 2.6.2020 г. 18:25:27 ч.
 *  Author: user
 */ 

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <rcServo.h>

void servo_initialise();
void servo_open();
void servo_close();