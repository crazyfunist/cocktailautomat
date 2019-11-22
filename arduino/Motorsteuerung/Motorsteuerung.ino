/*
 Name:    Motorsteuerung.ino
 Created: 10.06.2019 12:58:23
 Author:  Johannes Langner
 Description:
	  1) uC recieves stepper run command including duration for each stepper via SPI
	  2) requested Steppers start up simultaneously
		 ->soft acceleration results in higher speeds due to more torque in the beginning
	  3) once Steppers reach full speed they keep running until each requested time runs out
	  4) after longest request time is reached and pumps are stopped, a new run command can be recieved
	  5) while steppers are running, only stop command will be processed
Wiring:
	Arduino UNO R3 only! on any other board, ports may differ.
	Stepper 1 -> D7
	Stepper 2 -> D6
	Stepper 3 -> D5
	Stepper 4 -> D4
	Stepper 5 -> D3
	Stepper 6 -> D2
	Stepper 7 -> D13
	Stepper 8 -> D12

	DIR -> D8
	ENA -> D9
*/
#include <stdlib.h>

#define DIR_PIN            8
#define ENABLE_PIN         9

#define NUM_STEPPERS       8
#define INTERVAL_STARTSPEED 1600
#define INTERVAL_TOPSPEED 400


volatile unsigned int maxSpeed;
volatile unsigned long n = 0;
volatile float d;
volatile unsigned long stepCount = 0;
volatile unsigned long rampUpStepCount = 0;
volatile unsigned long totalSteps = 0;
unsigned int c0;

unsigned long comSteps[NUM_STEPPERS];

bool stopRequest, steppersTurning, steppersFinished, steppersStarted;

constexpr auto MESSAGELENGTH = 4;
String SerialBufferString = "";

struct Stepper {
	unsigned long timeRemaining = 0;
};

volatile Stepper steppers[NUM_STEPPERS];

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;
	}

	/* PINS */
	//Data Directory Registers
	DDRD |= B11111100;//Pin 0,1 -> RX,TX
	DDRB |= B00111111;//Pin 6,7 -> crystal
	//Port Data Registers
	PORTD &= B00000011;//2-7 -> Output
	PORTB &= B11000000;//0-5 -> Output


	steppersTurning = false;
	steppersStarted = false;
	c0 = INTERVAL_STARTSPEED;
	maxSpeed = INTERVAL_TOPSPEED;
	resetCommandedSteps();
}

/*
* Author: Johannes Langner
* Date: 20.06.19
* Purpose: calculate highest commanded steps
* State: dev
*/
unsigned long calcTotalSteps(void) {
	unsigned long maxVal = 0;
	for (int i = 0; i < NUM_STEPPERS; i++) {
		maxVal = comSteps[i] > maxVal ? comSteps[i] : maxVal;
	}
	return maxVal;
}

/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: activate timer ISR, start running steppers
* State: dev
*/
void startSteppers(void) {
	resetRemainingSteps();
	stopRequest = false;
	steppersStarted = true;
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = comSteps[i]; //insert calculation ml->steps here
	}
	resetCommandedSteps();
	digitalWrite(DIR_PIN, LOW);//pump forward
	totalSteps = calcTotalSteps();
	d = c0;
	n = 0;
	stepCount = 0;
	steppersTurning = true;
}

/*
* Author: Johannes Langner
* Date: 20.06.19
* Purpose: reset commanded steps to zero
* State: dev
*/
void resetCommandedSteps(void) {
	for (int i = 0; i < NUM_STEPPERS; i++) {
		comSteps[i] = 0;
	}
}

/*
* Author: Johannes Langner
* Date: 20.06.19
* Purpose: reset remaining steps to zero
* State: dev
*/
void resetRemainingSteps(void) {
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = 0;
	}
}

/*
* Author: Johannes Langner
* Date: 21.06.19
* Purpose: deactivate timer ISR, send message via serial
* State: dev
*/
void stopSteppers(void) {
	resetRemainingSteps();
	stepCount = 0;
	steppersTurning = false;
	Serial.println('d');
}


/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: reverse run all pumps
* State: dev
*/
void flushTubes(unsigned long duration) {
	steppersStarted = true;
	stopRequest = false;
	d = c0;
	n = 0;
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = duration;
	}
	resetCommandedSteps();
	steppersTurning = true;
	digitalWrite(DIR_PIN, HIGH);//reverse direction
}


/*
* Author: Johannes Langner
* Date: 21.06.19
* Purpose: handle incoming messages from Serial connection
* State: dev
* Description: commands: s      stop pumps
*            fX;      flush tubes for X ml
*            pX1,X2,..; pump1 X1 ml, pump2 X2 ml,..
*/
void serialEvent(void) {
	if (Serial.available()) {
		if (steppersTurning) {
			if ((char)Serial.read() == 's') {
				stopRequest = true;
				Serial.print('s');
				return;
			}
		}
		else {
			char command = (char)Serial.read();
			delay(50); //wait for Bytes to arrive
			if (Serial.available() && command == 'f') {
				while (Serial.available()) {
					char val = (char)Serial.peek();
					if (isDigit(val)) {
						Serial.read();
						if (val > 10)val -= 48;//covert from ascii to byte value
						comSteps[0] *= 10;
						comSteps[0] += val;
					}
					else {
						break;
					}
				}
				flushTubes(comSteps[0]);
				return;
			}
			if (Serial.available() && command == 'p') {
				int i = 0;
				while (Serial.available()) {
					char val = (char)Serial.peek();
					if (isDigit(val)) {
						Serial.read();
						if (val > 10)val -= 48;//covert from ascii to byte value
						comSteps[i] *= 10;
						comSteps[i] += val;
					}
					else if (val = ',') {
						Serial.read();
						i++;
						if (i >= NUM_STEPPERS) break;
					}
					else if (val = ';') {
						Serial.read();
						break;
					}
					else {
						break;
					}
				}
				startSteppers();
				return;
			}
		}
	}
	return;
}



void loop() {
	steppersFinished = true;
	if (steppers[0].timeRemaining > 0) {
		PORTD |= 0b10000000;
		steppers[0].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[1].timeRemaining > 0) {
		PORTD |= 0b01000000;
		steppers[1].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[2].timeRemaining > 0) {
		PORTD |= 0b00100000;
		steppers[2].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[3].timeRemaining > 0) {
		PORTD |= 0b00010000;
		steppers[3].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[4].timeRemaining > 0) {
		PORTD |= 0b00001000;
		steppers[4].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[5].timeRemaining > 0) {
		PORTD |= 0b00000100;
		steppers[5].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[6].timeRemaining > 0) {
		PORTB |= 0b00100000;
		steppers[6].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[7].timeRemaining > 0) {
		PORTB |= 0b00010000;
		steppers[7].timeRemaining--;
		steppersFinished = false;
	}
	delayMicroseconds(370);
	PORTD &= ~0b11111100;
	PORTB &= ~0b00110000;

	if (stopRequest || (steppersFinished && steppersStarted)) {
		Serial.println("topping..");
		steppersStarted = false;
		if (stopRequest)stopRequest = false;
		stopSteppers();
	}

	// soft acceleration
	n++;
	d = d - (80 * d) / (140 * n + 1);
	if (d <= maxSpeed) {
		d = maxSpeed;
	}
	//Serial.println(d);
	delayMicroseconds(370);
}
