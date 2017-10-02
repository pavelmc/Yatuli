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
 * across GND and +Vcc, then a 1nF (102) capacitor across GND and the wiper
 * to avoid RF in noisy environments, but to allow some LF noise that is
 * good to the ADC oversampling that is the base of this lib
 *
 * The wiper is connected to an Analog input of the Arduino, see the
 * examples with this lib
 *
 * You can get the latest code in this Github repository:
 *
 * https://github.com/pavelmc/yatuli
 *
 * ===========================================================================
 *
 * This example simulates a radio dial via serial port, you can see the
 * frequency, ADC value and relative value to ADC center.
 *
 * It's intended to be tested on a Raduino hardware.
 *
 * ===========================================================================
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


#include "yatuli.h"

// Define the Analog Pin to Use
#define APin  A7

// lib instantiation as "St" for Shuttle tune function
Yatuli St;

// function to show the value via serial
void show(long val, int adc) {
    Serial.print("Freq: ");
    Serial.print(val);
    Serial.print(" ADC: ");
    Serial.println(adc);
}


void setup() {
    // initialize serial communications at 19200 bps:
    Serial.begin(19200);
    Serial.println("Example init...");

    // Init the lib
    St.init(APin, 6900000, 7500000, 100, 10000);

    // 6.9 to 7.5 Mhz
    // 100 Hz steps
    // 10 kHz steps on edges

    // Set 7.100 Mhz as the start freq
    St.set(7100000);
}


void loop() {
    // the assignation happens only at the first cycle
    static long last = St.value;

    // check
    St.check();

    // show only if it has changed
    if (last != St.value) {
        // has moved, show
        show(St.value, St.adc);

        // set the last value
        last = St.value;
    }
}
