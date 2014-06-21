/*****************************
 * Filename: clk_measure_period.h
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
/*
 * This class uses the Input Capture Unit of Timer 1 to help measure the period of a clock signal.
 * The sample rate comes from the clock input on the T1 input pin.
 *
 * Architectures:
 * - ATtiny24A/44A/84A
 *
 */

namespace timers
{
	class clk_measure_period
	{

	public:

	// Constructor.
	clk_measure_period();

	// Destructor.

	~clk_measure_period();

	void initialize();

	//enum Trigger_Edge {rising=1, falling=0};

	Trigger_Edge trigger_edge() const;

	void trigger_edge(const Trigger_Edge trigEdge);

	uint16 clk_meas_period() const;

	void clk_meas_period(const uint16 clk_meas_period);

	void clk_meas_overflow_incr();

	void clk_meas_overflow_decr();

	bool clk_meas_out_of_range() const;

	float clk_meas_freq_kHz() const;

	void clk_meas_freq_kHz(float freq);

	private:

	// Backing variables.
	Trigger_Edge _trigger_edge;
	uint16 _clk_meas_period;
	ubyte _clk_meas_overflow_count;
	bool _clk_meas_out_of_range;
	float _clk_meas_freq_kHz;

	};
}
