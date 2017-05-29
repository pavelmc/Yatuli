/*
 * Yatuli - (Yet Another Tune Lib)
 * ===============================
 *
 * This lib is inspired in the work of the bitx40 raduino code with a
 * linear volume resistor as tune element instead a quadrature encoder
 *
 * Se here for more details: https://github.com/afarhan/bitx40.git
 *
 * We use a linear volume resistor with extremes connected to GND and +Vcc
 * of the arduino, for stability you must put a 1uF polarized capacitor
 * across GND and +Vcc, then a 10nF (103) capacitor across GND and the wiper
 *
 * The wiper is connected to an Analog input of the Arduino, see the
 * examples with this lib
 *
 * You can get the latest code in this Github repository:
 *
 * https://github.com/pavelmc/yatuli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef YATULI_H
#define YATULI_H

// rigor includes
#include "Arduino.h"

// some defines
#define PACE 500            // how many msecs between edge jumps & dir emits
#define LIMITLOW -5000L     // limits to jump (< 10%)
#define LIMITHIGH 5000L     // limits to jump (> 90%)
#define DIRTICKS 500L       // about 20 ticks per rotation
                            // ~ 1000 / (desired_steps) ... 500

/**** RANGE of one turn is 1023 steps, but internally is handled as 10230 ****/

class Yatuli {
    public:
        // Custom init procedure, we need:
        // * ADC pin
        // * Limits, start/end (up to 4.2 Ghz)
        // * Minimum step (up to 65 K)
        // * Steps on the edges (up to 65 K)
        void init(uint8_t, int32_t, int32_t, uint16_t, uint16_t);

        // pass a starting point, if not called it will be on the lower end
        void set(int32_t);

        // Update, this is what you put in your loop and call it on every
        // cycle to update all values and see if it moved
        void check(void);

        // Return a relative vector from the last position: -1/0/+1
        // it will emit about ~50 steps in one rotation
        // WATCH OUT!: int8_t is char in arduino
        int8_t dir(void);

        // public value
        int16_t adc;            // oversampled ADC in the range -5115/+5115
        int32_t value;          // real value in the range

        // lock feature, when lock is true, we refuse to update the value/dir
        // as in a real TX high currents or RF can disturb the ADC and add FMing
        // to the real freq.
        bool lock;

    private:
        int32_t start;          // start of the range
        int32_t end;            // stop of the range
        uint16_t step;          // minimum step
        uint16_t edgeStep;      // steps at the edges
        int32_t base;           // base value of the slot, lower end by default
        uint8_t  pin;           // the analog pin on which we will operate

        // oversampling the ADC
        void _osadc(void);
};

#endif
