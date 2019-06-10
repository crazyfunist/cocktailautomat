/*
 Name:		Motorsteuerung.ino
 Created:	10.06.2019 12:58:23
 Author:	Johannes Langner
*/
#include <Stepper.h>

const int stepsPerRevolution = 2048;

Stepper pumpe1(stepsPerRevolution, 8, 9, 10, 11);
//Stepper pumpe2(..

int stepCount_pumpe1 = 0;
//int stepCount_pumpe2..

void setup() {

}



void loop() {
	motortest(pumpe1);
}






/*
Rotates stepper with Poti-defined speed
*/
void motortest(Stepper stepper) {
	// read the sensor value:
	int sensorReading = analogRead(A0);
	// map it to a range from 0 to 100:
	int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
	// set the motor speed:
	if (motorSpeed > 0) {
		stepper.setSpeed(motorSpeed);
		// step 1/100 of a revolution:
		stepper.step(stepsPerRevolution / 100);
	}
}