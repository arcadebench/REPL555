/*
 * clk_meas_period.cpp
 *
/*****************************
 * Filename: clk_measure_period.cpp
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
 *
 * Change log:
 * 6/13/14 Initial release
 *
 *****************************
 * This class uses the Input Capture Unit of Timer 1 to help measure the period of a clock signal.
 * The sample rate comes from the clock input on the T1 input pin.
 *
 * Architectures:
 * - ATtiny24A/44A/84A
 *
 */

# include <defs.h>
# include <avr/io.h>
# include <avr/interrupt.h>
# include <util/delay.h>
# include <limits.h>
# include <avr/common.h>
# include <timers/timers/clk_measure_period.h>

namespace timers
{

// Constructor.

clk_measure_period::clk_measure_period(){}

void clk_measure_period::initialize()
{
	// Put counter in Normal mode. The Compare Match Outputs are unused.
	TCCR1A = 0x00;
	// Enable the Input Capture Noise Canceller, rising edge capture, Force compares not activated.
	// Clock comes from the rising edge on the CLKI input pin.
	TCCR1B = 0xC7;
	// Enable the interrupts for both the Input Capture and Overflow Detect.
	// Input Capture interrupt is handled by TIM1_CAPT_vect.
	// Overflow interrupt handled by TIM1_OVF_vect.
	TIMSK1 = 0x21;
}

// Destructor.

clk_measure_period::~clk_measure_period()
{
	// Turn off the clock to the counter.
	TCCR1B = 0x00;
}

Trigger_Edge clk_measure_period::trigger_edge() const
{
	return _trigger_edge;
}

void clk_measure_period::trigger_edge(const Trigger_Edge trigEdge)
{
	_trigger_edge = trigEdge;
	if (_trigger_edge == rising)
	{
		TCCR1B |= 0x40;
	}
	else
	{
		TCCR1B &= ~(0x40);
	}
}

uint16 clk_measure_period::clk_meas_period() const
{
	return _clk_meas_period;
}


// The clk_meas_period should be set here by the TIM1_CAPT_vect ISR routine.
void clk_measure_period::clk_meas_period(const uint16 clk_meas_period)
{
	_clk_meas_period = clk_meas_period;
}

void clk_measure_period::clk_meas_overflow_incr()
{
	_clk_meas_out_of_range = true;
	if ((_clk_meas_overflow_count + 2) > 16)
	{
		_clk_meas_overflow_count = 16;
	}
	else
	{
		_clk_meas_overflow_count += 2;
	}
}

void clk_measure_period::clk_meas_overflow_decr()
{
	if ((_clk_meas_overflow_count - 1) < 0)
	{
		_clk_meas_overflow_count = 0;
	}
	else
	{
		_clk_meas_overflow_count -= 1;
	}

	if (_clk_meas_overflow_count == 0)
	{
		_clk_meas_out_of_range = false;
	}
	else
	{
		_clk_meas_out_of_range = true;
	}
}

bool clk_measure_period::clk_meas_out_of_range() const
{
	return _clk_meas_out_of_range;
}

float clk_measure_period::clk_meas_freq_kHz() const
{
	return _clk_meas_freq_kHz;
}

// The clk_meas_freq_kHz should be set here by the TIM1_CAPT_vect ISR routine.
void clk_measure_period::clk_meas_freq_kHz(float freq)
{
	_clk_meas_freq_kHz = freq;
}

}

