/*****************************
 * Filename: timer_100us.cpp
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
 *
 * This class creates a timer using Timer0 that counts 100us intervals and sets an interrupt.
 * Automatically adjusts its counter TOP value if the F_CPU frequency of the chip's clock changes.
 * Triggers the TIM0_COMPA_vect interrupt handler.
 *
 * Architectures:
 * - ATtiny24A/44A/84A
 *
 */

# include <defs.h>
# include <avr/io.h>
# include <timers/timers/timer_100us.h>

// Select the correct prescaler divider to keep the count within 256.
# if (F_CPU > 2560000UL)
# define clk_sel_value 0x02
# define prescale_count_div 8UL
# else
# define clk_sel_value 0x01
# define prescale_count_div 1UL
# endif

// Calculate the count to get close to 100us per count cycle.
# define count_tc_value (F_CPU / (prescale_count_div * 10000UL))

namespace timers
{

// Constructor.
timer_100us::timer_100us(){}

// Initialize function.
void timer_100us::initialize()
{
	// Compare Output mode OC0A is set to toggle on timeout. Output OC0B is unused.
	// Set the counter to CTC mode.
	TCCR0A = 0x42;
	TCCR0B = 0x00;

	// Select the correct prescaler divider to keep the count within 256.
	TCCR0B |= clk_sel_value;

	// Program the OCRA with the TOP value where the counter will roll over.
	OCR0A = (ubyte)(count_tc_value);

	// Enable the interrupt-on-match for OCR0A. The interrupt will fire when the counter reaches OCR0A.
	TIMSK0 = 0x02;
}

// Destructor.

timer_100us::~timer_100us()
{
	// Turn off the clock to the counter.
	TCCR0B = 0x00;
}

}
