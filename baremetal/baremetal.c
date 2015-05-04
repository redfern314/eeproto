#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define redLED          5
#define greenLED        6

void blinkLED(int LED) {
    PORTB ^= (1 << LED);
}

// ISR(INT3_vect) {
//     PORTD ^= _BV(PD6);
// }

ISR(TIMER1_CAPT_vect) {
    uint16_t icr = ICR1;
    if(icr > 18) {          // 18 ticks = about 13.9kHz
        blinkLED(redLED);
        PORTD |= _BV(PD6);
    } else {
        PORTD &= ~_BV(PD6);
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

    // EICRA |= _BV(ISC30) | _BV(ISC31);           // trigger on rising edge
    // EIMSK |= _BV(INT3);                         // enable interrupt 3

    sei();

    // blinkLED(redLED);

    while (1) {

    }
}