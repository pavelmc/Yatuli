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
 * ===========================================================================
 *
 * This example simulates a radio dial via serial port, you can see the
 * frequency, ADC value and relative value to center.
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

// lib instantiation as "St" for shuttle tune
Yatuli St;

// function to show the value via serial
void show(long val, word adc, int rel) {
    Serial.print("Freq: ");
    Serial.print(val);
    Serial.print(" ADC: ");
    Serial.print(adc);
    Serial.print(" REL: ");
    Serial.println(rel);
}


void setup() {
    // initialize serial communications at 115200 bps:
    Serial.begin(115200);
    Serial.println("Example init...");

    // Init the lib
    St.init(APin, 6900000LL, 7500000LL, 1000, 10000L);

    // Set 7.100 Mhz as the start freq
    St.set(7150000L);
}

void loop() {
    // check
    if (St.check()) {
        // value has changed
        show(St.value(), St.adc, -512L + St.adc);
    }

    delay(50);
}
