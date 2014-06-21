/*****************************
 * Filename: repl555.cpp
 * Release: 1.0
 * Copyright 2014 Craig Yarbrough
 *
 * www.arcadebench.com
 *
 * This program and all related files are distributed under the
 * terms of the GNU General Public License (GPLv3). Please refer to
 * the COPYING.txt file for a complete version of the copyright
 * notice. ADditional information may be obtained
 * from http://www.gnu.org/licenses.
 *
 * Project Date: April 2014
 * Author: The Arcadebench
 * Note that BOD level was set to 4V3.
 *
 * Change log:
 * 6/13/14 Initial release
 *
 *****************************/

# include <defs.h>
# define F_CPU 1000000UL
# include <stdlib.h>
# include <avr/io.h>
# include <avr/interrupt.h>
# include <util/delay.h>
# include <timers/timers/pwm_meas_100us.h>

// Define pin assignments for ATtiny24A device.
volatile ubyte *PAN_EN_N = &PORTA;
ubyte PAN_EN_N_pin = 0;
volatile ubyte *VIN = &PORTA;
ubyte VIN_pin = 1;
volatile ubyte *OUTPUT = &PORTA;
ubyte OUTPUT_pin = 2;
volatile ubyte *DIP_SW3 = &PORTB;
ubyte DIP_SW3_pin = 0;
volatile ubyte *DIP_SW2 = &PORTB;
ubyte DIP_SW2_pin = 1;
volatile ubyte *DIP_SW1 = &PORTB;
ubyte DIP_SW1_pin = 2;
volatile ubyte *DIP_SW0 = &PORTA;
ubyte DIP_SW0_pin = 7;

// Globals.
volatile ubyte *ADCPntr = &ADCH;
ubyte potADCValue, inputADCValue;
volatile ubyte *inputs_portb = &PINB;
volatile ubyte currentState, nextState = 0x00;

// Create the PWM measurement timer.
timers::pwm_meas_100us pwm_timer(OUTPUT, OUTPUT_pin, timers::pwm_meas_100us::rising);

// Variables for test only.
uint16 pwm_period_value;
bool pwm_meas_valid_value;
bool pwm_out_of_range_value;
float pwm_freq_kHz_value;

enum State {
	run = 0x00,
	offset = 0x01
	};
State devState;

byte SwitchState()
{
	volatile ubyte switches = 0x00;
	//if (_AVR_IOM328P_H_)
	//{
	switches = 0x0F & PINA;
	//}
	return switches;
}

int main(void)
{
	// MISC
	OSCCAL = 0x70; // Sets the calibration for the internal RC oscillator clock.
	MCUCR  = 0x00; // For safety, reset the WDRF bit to prevent an eternal WDR loop. Also keep the interrupt vectors at the beginning of flash memory.
	PRR    = 0x00; // Not using the Power Reduction Register.

    // ADC
    ADMUX = 0x65; // ADC set ref to AVCC.  Result will be left-justified in the ADCH register.  Single-ended input from pin ADC5.
    ADCSRA = 0x0E; // Don't enable the ADC until the code is ready to do so. The interrupts will be triggered 'manually'.  Set the conversion clock to 8MHz / 64 = 125kHz.
    ADCSRB = 0x00; // Set the conversion trigger mode to Free Running.
	DIDR0 = 0x30; // Disable the digital input buffers on pins ADC5 and ADC4, since they will not be used as digital inputs.


	// Enable interrupts.
	sei();

	// Enable the ADC.
	ADCSRA|=(1<<ADEN);

	// Kick off the ADC conversions.
	ADCSRA|=(1<<ADSC);

	while(1)
	{
		nextState = SwitchState();
		switch (currentState)
		{
			case run:
			{
				currentState = nextState;
				break;
			}
			case offset:
			{
				currentState = nextState;
				break;
			}

		}
	}
}

ISR(ADC_vect)
{
	// Test to see which channel we're reading from.
	if ((ADMUX & 0x0F) == 0x05) // On ADC5.
	{
		potADCValue = *ADCPntr;
		ADMUX &= 0xF0;
		ADMUX |= 0x04;
	}
	else if ((ADMUX & 0x0F) == 0x04) // On ADC4.
	{
		inputADCValue = *ADCPntr;
		ADMUX &= 0xF0;
		ADMUX |= 0x05;
	}
	else // Fault case, so ignore value and set the mux to good value.
	{
		ADMUX &= 0xF0;
		ADMUX |= 0x04;
	}

	// Kick off the next ADC conversion.
	ADCSRA|=(1<<ADSC);
}

ISR(TIM0_COMPA_vect)
{
	pwm_timer.advance();
	pwm_period_value = pwm_timer.pwm_period();
	pwm_meas_valid_value = pwm_timer.pwm_meas_valid();
	pwm_out_of_range_value = pwm_timer.pwm_out_of_range();
	pwm_freq_kHz_value = pwm_timer.pwm_freq_kHz();

	//	if (countDown > 0x00) {countDown-=1;}
}


