/*
 Name:		Motorsteuerung.ino
 Created:	10.06.2019 12:58:23
 Author:	Johannes Langner
 Description:
			1) uC recieves stepper run command including duration for each stepper via SPI
			2) requested Steppers start up simultaneously
			   ->soft acceleration results in higher speeds due to more torque in the beginning
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


	c0 = 1600;
}


void loop() {
	//do nothing
}

/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: activate timer ISR, start running steppers
* State: dev
*/
void startSteppers(unsigned long s1, unsigned long s2, unsigned long s3, unsigned long s4, unsigned long s5, unsigned long s6, unsigned long s7, unsigned long s8) {
	steppers[0].timeRemaining = s1;
	steppers[1].timeRemaining = s2;
	steppers[2].timeRemaining = s3;
	steppers[3].timeRemaining = s4;
	steppers[4].timeRemaining = s5;
	steppers[5].timeRemaining = s6;
	steppers[6].timeRemaining = s7;
	steppers[7].timeRemaining = s8;
	digitalWrite(DIR_PIN, LOW);//pump forward
	totalSteps = max(s1, s2, s3, s4, s5, s6, s7, s8);
	d = c0;
	OCR1A = d;
	stepCount = 0;
	TIMER1_INTERRUPTS_ON;
}

/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: activate timer ISR, start running steppers
* State: dev
*/
void stopSteppers(void) {
	stepCount = 0;
	TIMER1_INTERRUPTS_OFF
}

/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: reverse pump
* State: dev
*/
void flushTubes(unsigned long duration) {
	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppers[i].timeRemaining = duration;
	}
	digitalWrite(DIR_PIN, HIGH);//reverse direction
	TIMER1_INTERRUPTS_ON;
}


/*
* Author: Johannes Langner
* Date: 19.06.19
* Purpose: handle incoming messages from Serial connection
* State: pre-dev
*/
void serialEvent() {
	while (Serial.available()) {
		// get byte:
		char in = (char)Serial.read();
		delay(2);  //delay to allow byte to arrive in input buffer
		SerialBufferString += in;
		if (SerialBufferString.length() >= MESSAGELENGTH) {
			if (SerialBufferString.startsWith("pump")) {
				Serial.println("Pumpe:");
				//pumpselector = strtol(SerialBufferString.substring(SerialBufferString.indexOf(';') + 1),NULL,10);
				//stepCount_pumpe[pumpselector] = strtol(SerialBufferString.substring(SerialBufferString.lastIndexOf(';') + 1),NULL,10);
				//Serial.println(pumpselector);
				Serial.println("Menge:");
				//Serial.println(stepCount_pumpe[pumpselector]);
			} else if(SerialBufferString.startsWith("conn")){
				Serial.println("connected");
			}
		}
	}
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
