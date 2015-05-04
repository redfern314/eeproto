#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define redLED          5
#define greenLED        6
#define zerotol         18
#define matchlen        5

uint8_t bitstring[] = {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}; // Derek
//uint8_t bitstring[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}; // Cameron
uint8_t captured[matchlen];
volatile uint8_t matching = 1;
volatile uint8_t current = 0;
volatile uint8_t zeroes = 0;

void blinkLED(int LED) {
    PORTB ^= (1 << LED);
}

ISR(TIMER1_CAPT_vect) {
    uint16_t icr = ICR1;
    current++;
    if(icr > 18 && current <= matchlen) {          // 18 ticks = about 13.9kHz
        zeroes = 0;
        if (bitstring[current] != 1) {
            matching = 0;
        }
    } else {
        zeroes++;
        if (zeroes > zerotol) {
            current = 0;
            matching = 1;
            blinkLED(greenLED);
        } else if (current <= matchlen) {
            if (bitstring[current] != 0) {
                matching = 0;
            }
        }
    }
    if (current == matchlen && matching) {
        // success!
        blinkLED(redLED);
    }
    TCNT1 = 0;
}

int main() {
    USBCON &= ~_BV(USBE);

    DDRD &= ~_BV(PD1) & ~_BV(PD2) & ~_BV(PD3);  // Inputs: PD1 (EM4095 data), PD2 (EM4095 CLK), PD3 (INT3)
    DDRD |= _BV(PD0) | _BV(PD6);                // Outputs: PD0 (EM4095 enable), PD6 (test pin)
    DDRB |= _BV(PB5) | _BV(PB6) | _BV(PB7);     // Outputs: PB5 (red LED), PB6 (green LED), PB7 (square wave)

    TCCR0A |= _BV(WGM01) | _BV(COM0A0);         // set mode to CTC; toggle output on match
    TCCR0B |= _BV(CS00) | _BV(CS01);            // set clock prescaler to 64
    OCR0A = 0x00;                               // output compare value
                                                // output freq = fclk / (2*prescale*(1+OCR0A)) = 125kHz

    TCCR1B |= _BV(ICNC1) | _BV(ICES1) | _BV(CS11) | _BV(CS10);
    TIMSK1 |= _BV(ICIE1);

    sei();

    while (1) {

    }
}