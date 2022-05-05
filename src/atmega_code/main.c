/*
 * trashe_main.c
 *
 * Created: 3/26/2022 10:13:11 AM
 * Author : John
 */ 

#define F_CPU 1000000UL // Define F_CPU to 1MHz since the atmega internal clock runs at 1MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//PORTB 
#define FL_LED 2
#define FR_LED 1
#define BL_LED 4
#define BR_LED 3

//PORTC

#define FL_Trig 4
#define FR_Trig 5
#define BL_Trig 3
#define BR_Trig 2

//PORTD
#define detectionPin 2
#define loadDetPin 3
#define loadPin 4
#define directionPin 6

uint8_t loadDetBool = 0;
uint8_t timerBool = 0;
uint16_t overflowCount = 0;
uint8_t detThreshold = 25;
uint16_t overflowThreshold = 4;

ISR (TIMER0_OVF_vect) {
	/*if (overflowCount) {
		PORTD &= ~(1<<5);
	}
	else {
		PORTD |= (1<<5);
	}*/
	//PORTD |= (1<<loadDetPin);
	overflowCount++;
}

uint8_t readPin(volatile uint8_t reg, uint8_t pin) {
	return reg & (1<<pin);
}

float runUltra(uint8_t trigPin) {
	/* Runs Ultrasonics
	 * Trigger the ultrasonics with 10 us signal then start timer1 with 1024 prescale on rising edge capture
	 * Then save timer and then set timer1 to falling edge capture and save time
	 * Then perform distance calculation
	 * Distance calculation seem to be off by a factor of 10 with normal conversion (1.098) so we divide by 10 to get a more accurate reading
	 */
	float distance = 0;
	unsigned int elapsedTime = 0, timeStart = 0, timeEnd = 0;
	
	//PORTC &= ~(1<<trigPin);
	PORTC = (0<<trigPin);
	_delay_us(2);
	//PORTC |= (1<<trigPin);
	PORTC = (1<<trigPin);
	_delay_us(10);
	PORTC = (0<<trigPin);
	//PORTC &= ~(1<<trigPin);
	
	TCNT1 = 0;
	//TCCR1B = 0x45; //For 16MHz
	TCCR1B = 0x43;
	//PORTD |= (1<<loadDetPin);
	while(!(TIFR1 & (1<<ICF1)));
	//PORTD &= ~(1<<loadDetPin);
	timeStart = ICR1;
	TIFR1 = (1<<ICF1);
	
	//TCCR1B = 0x05; // for 16MHz
	TCCR1B = 0x03;
	
	while (!(TIFR1 & (1<<ICF1)));
	
	timeEnd = ICR1;
	TIFR1 = (1<<ICF1);
	
	elapsedTime = timeEnd - timeStart;
	
	distance = elapsedTime * 1.098;
	//distance = elapsedTime * 17.562;
	
	_delay_us(60);
	return distance;
}

float ultraAverage(uint8_t times, uint8_t trigPin) {
	/*
	Multisample ultrasonic times number of times
	*/
	float sum = 0;
	for (int i = 0; i < times; i++) {
		sum += runUltra(trigPin);
	}
	return sum / times;
}

void loadDetector() {
	if (!readPin(PIND, loadPin)) {
		if (!loadDetBool) {
			if (!timerBool) {
				TCNT0 = 0;
				overflowCount = 0;
				timerBool = 1;
			}
			else if (overflowCount >= overflowThreshold){
				loadDetBool = 1;
				timerBool = 0;
				PORTD |= (1<<loadDetPin);
			}
		}
		else {
			TCNT0 = 0;
			overflowCount = 0;
			timerBool = 0;
		}
		//PORTD |= (1<<5);
		
	}
	else {
		if (loadDetBool) {
			if (!timerBool) {
				TCNT0 = 0;
				overflowCount = 0;
				timerBool = 1;
			}
			else if (overflowCount >= overflowThreshold) {
				loadDetBool = 0;
				timerBool = 0;
				PORTD &= ~(1<<loadDetPin);
			}
		}
		else {
			TCNT0 = 0;
			overflowCount = 0;
			timerBool = 0;
		}
		//PORTD &= ~(1<<5);
	}
}

void setup() {
	TCCR0A = 0;
	TCCR0B = 5;
	TIMSK0 = 1;
	
	DDRC = (1<<FL_Trig) | (1<<FR_Trig) | (1<<BL_Trig) | (1<<BR_Trig);
	DDRD = (1<<detectionPin) | (1<<loadDetPin) | (0<<loadPin) | (0<<directionPin);
	DDRB = (0<<0) | (1<<FL_LED) | (1<<FR_LED) | (1<<BL_LED) | (1<<BR_LED);
	
	PORTC = 0;
	PORTD = (1<<loadPin) | (1<<directionPin);
	PORTB = (0<<0);
	sei();
}

int main(void)
{
	setup();
    while (1) 
    {
		
		loadDetector();
		
		if (readPin(PIND, directionPin)) { 
			if (ultraAverage(10, FR_Trig) < detThreshold) {
				PORTB |= (1<<FR_LED);
				PORTD |= (1<<detectionPin);
				//PORTD |= (1<<loadDetPin);
			}
			else if (ultraAverage(10, FL_Trig) < detThreshold) {
				PORTB |= (1<<FL_LED);
				PORTD |= (1<<detectionPin);
				//PORTD |= (1<<loadDetPin);
			}
			else {
				PORTB &= ~((1<<FR_LED) | (1<<FL_LED) | (1<<BR_LED) | (1<<BL_LED));
				PORTD &= ~(1<<detectionPin);
				//PORTD |= (1<<loadDetPin);
			}
		}
		else {
			if (ultraAverage(10, BR_Trig) < detThreshold) {
				PORTB |= (1<<BR_LED);
				PORTD |= (1<<detectionPin);
			}
			else if (ultraAverage(10, BL_Trig) < detThreshold) {
				PORTB |= (1<<BL_LED);
				PORTD |= (1<<detectionPin);
			}
			else {
				PORTB &= ~((1<<FR_LED) | (1<<FL_LED) | (1<<BR_LED) | (1<<BL_LED));
				PORTD &= ~(1<<detectionPin);
				//PORTD |= (1<<loadDetPin);
			}
		}
    }
	return 0;
}

