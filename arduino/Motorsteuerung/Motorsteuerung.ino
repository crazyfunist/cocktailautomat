/*
 Name:		Motorsteuerung.ino
 Created:	10.06.2019 12:58:23
 Author:	Johannes Langner
 Description:
			1) uC recieves stepper run command including duration for each stepper via SPI
			2) requested Steppers start up simultaneously
			3) once Steppers reach full speed they keep running until each requested time runs out
			4) after longest request time is reached, a new run command can be recieved
			5) while steppers are running, only stop command will be processed
*/
#include <stdlib.h>

#define DIR_PIN            2
#define ENABLE_PIN         3

#define STEP_PIN_1         4
#define STEP_PIN_2         5
#define STEP_PIN_3         6

#define NUM_STEPPERS       8
#define INTERVALL_TOPSPEED 100

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);


volatile unsigned int maxSpeed = 80;
volatile unsigned long n = 0;
volatile float d;
volatile unsigned long stepCount = 0;
volatile unsigned long rampUpStepCount = 0;
volatile unsigned long totalSteps = 0;
unsigned int c0;

unsigned long comSteps[NUM_STEPPERS];

bool steppersTurning;

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


	/* TIMER */
	noInterrupts();
	//Timer Counter/Controll Register PWM
	TCCR1A = 0;//deactivate PWM
	//Timer Counter/Controll Register Prescaler
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= ((1 << CS11) | (1 << CS10));
	//Timer Count Register
	TCNT1 = 0;
	OCR1A = 1000;
	TIMER1_INTERRUPTS_OFF;
	interrupts();

	steppersTurning = false;
	c0 = 1600;

	resetCommandedSteps();
}


void loop() {
	//do nothing
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
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = comSteps[i];
	}
	resetCommandedSteps();
	digitalWrite(DIR_PIN, LOW);//pump forward
	totalSteps = calcTotalSteps();
	d = c0;
	OCR1A = d;
	stepCount = 0;
	steppersTurning = true;
	TIMER1_INTERRUPTS_ON;
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
* Date: 19.06.19
* Purpose: activate timer ISR, start running steppers
* State: dev
*/
void stopSteppers(void) {
	stepCount = 0;
	steppersTurning = false;
	TIMER1_INTERRUPTS_OFF
}

/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: reverse run all pumps
* State: dev
*/
void flushTubes(unsigned long duration) {
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = duration;
	}
	resetCommandedSteps();
	steppersTurning = true;
	digitalWrite(DIR_PIN, HIGH);//reverse direction
	TIMER1_INTERRUPTS_ON;
}


/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: handle incoming messages from Serial connection
* State: pre-dev
* Description: commands: s			stop pumps
*						 fX			flush tubes for X ml
*						 pX1,X2,..	pump1 X1 ml, pump2 X2 ml,.. 
*/
void serialEvent(void) {
	if (Serial.available()) {
		if (steppersTurning) {
			if ((char)Serial.read() == 's') {
				//STOP command
				stopSteppers();
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
						comSteps[0] *= 10;
						comSteps[0] += val;
					}
					else {
						break;
					}
				}
				flushTubes(comSteps[0]);
				Serial.print('f');
				return;
			}
			if (Serial.available() && command == 'p') {
				int i = 0;
				while (Serial.available()) {
					char val = (char)Serial.peek();
					if (isDigit(val)) {
						Serial.read();
						comSteps[i] *= 10;
						comSteps[i] += val;
					}
					else if(val=','){
						Serial.read();
						i++;
						if (i >= NUM_STEPPERS) break;
					}
					else {
						break;
					}
				}
				startSteppers();
				Serial.print('p');
				return;
			}
		}
	}
	return;
}



/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: handle timer interrupt
* State: pre-dev
* Description: for timing purposes, no loop is used
*/
ISR(TIMER1_OVF_vect) {
	TCNT1 = INTERVALL_TOPSPEED;//restart counter
	bool steppersFinished = true;
	if (steppers[0].timeRemaining > 0) {
		PORTD |=  0b10000000;
		PORTD &= ~0b10000000;
		steppers[0].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[1].timeRemaining > 0) {
		PORTD |=  0b01000000;
		PORTD &= ~0b01000000;
		steppers[1].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[2].timeRemaining > 0) {
		PORTD |=  0b00100000;
		PORTD &= ~0b00100000;
		steppers[2].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[3].timeRemaining > 0) {
		PORTD |=  0b00010000;
		PORTD &= ~0b00010000;
		steppers[3].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[4].timeRemaining > 0) {
		PORTD |=  0b00001000;
		PORTD &= ~0b00001000;
		steppers[4].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[5].timeRemaining > 0) {
		PORTD |=  0b00000100;
		PORTD &= ~0b00000100;
		steppers[5].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[6].timeRemaining > 0) {
		PORTB |=  0b00100000;
		PORTB &= ~0b00100000;
		steppers[6].timeRemaining--;
		steppersFinished = false;
	}
	if (steppers[7].timeRemaining > 0) {
		PORTB |=  0b00010000;
		PORTB &= ~0b00010000;
		steppers[7].timeRemaining--;
		steppersFinished = false;
	}

	if(steppersFinished) {
		stopSteppers();
	}

	if (rampUpStepCount == 0) { // soft acceleration
		n++;
		d = d - (2 * d) / (4 * n + 1);
		if (d <= maxSpeed) {
			d = maxSpeed;
			rampUpStepCount = stepCount;
		}
		if (stepCount >= totalSteps / 2) {
			rampUpStepCount = stepCount;
		}
		OCR1A = d;
	}
}
