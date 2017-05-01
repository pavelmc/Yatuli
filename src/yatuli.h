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
#define ST_SAMPLES 5        // not bugger than 63
#define PACE 500            // how many time between edge jumps & dir emits
#define LIMITLOW 11         // limits to jump (low)
#define LIMITHIGH 1012      // limits to jump (high)

/***** Main Class initialization *****/
class Yatuli {
    public:
        // Custom init procedure, we need:
        // * ADC pin
        // * Limits, start/end (up to 4.2 Ghz)
        // * Minimum step (up to 65 K)
        // * Steps on the edges (up to 65 K)
        void init(uint8_t, uint32_t, uint32_t, uint16_t, uint16_t);

        // pass a starting point, if not called it will be on the lower end
        void set(uint32_t);
        
        // Update, this is what you put in your loop and call it on every
        // cycle to update all values and see if it moved
        boolean check(void);
        
        // Return the value in the init range
        // including the edge jumps
        uint32_t value(void);
        
        // Return a relative vector from the last position: -1/0/+1
        // at a pace of twice a second
        int16_t dir(void);

        // public variables
        uint16_t adc;           // real value of the ADC
        uint32_t start;         // start of the range
        uint32_t end;           // stop of the range
        uint16_t step;          // minimum step
        uint16_t edgeStep;      // steps at the edges
        int16_t  relative;      // relative unit of ADC in -500 to +500 range
        
    private:
        // function to update the adc value and return
        // the relative moving vale of it
        void _update(void);
        
        uint32_t base;          // base value of the slot (lower end)
        uint32_t newTime;       // future timestamp
        uint16_t lastAdc;       // real value of the ADC the last time (move)
        uint16_t lastAdcDir;    // real value of the ADC the last time (dir)
        uint32_t lastValue;     // last value in the start-end range
        uint8_t  pin;           // the analog pin on which we will operate
};

#endif
