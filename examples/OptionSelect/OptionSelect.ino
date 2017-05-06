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
 * This example simulates a rotating menu selection process
 * using the yatuli function dir()
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

// lib instantiation as "yt"
Yatuli yt;

// options to rotate upon
String optionNames[] = {"  IF  ",
                        " FREQ ",
                        " XTAL ",
                        " BFO  ",
                        " PPM "};
#define OptCount 4

int opt = 0; 


// function to show the value via serial
void show(void) {
    Serial.print("Option: ");
    Serial.println(optionNames[opt]);
}


void setup() {
    // initialize serial communications at 19200 bps:
    Serial.begin(19200);
    Serial.println("Example init...");

    // Init the lib, at this point this doesn't matter in this example
    // as we will use just dir()
    yt.init(APin, 6900000LL, 7500000LL, 1000, 10000L);

    // Set 7.100 Mhz as the start freq
    yt.set(7100000L);
}

void loop() {
    // check
    if (yt.check()) {
        // we are using just the dir() function
        int direction = yt.dir();

        // check to move if needed
        if (direction != 0) {
            // move
            opt += direction;

            // limit checks
            if (opt > OptCount) opt = 0;
            if (opt < 0) opt = OptCount;

            // show it
            show();
        }
    }

    delay(50);
}
