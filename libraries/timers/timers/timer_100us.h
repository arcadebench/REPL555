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

namespace timers
{
	class timer_100us
	{
		public:

		// Constructor.
		timer_100us();

		// Destructor.
		~timer_100us();

		void initialize();

	};
}
