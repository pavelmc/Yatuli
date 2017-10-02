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
 * We use oversampling to get a better accuracy on the movements
 * as recommended by the AVR AN121 take a peek at this doc below to know more
 * http://www.atmel.com/dyn/resources/prod_documents/doc8003.pdf
 *
 * We are squeezing it for just two more extra bit as this signal is slow
 * moving and do has some noise on it.
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

#include "Arduino.h"
#include "yatuli.h"


/*****************************************************************************
 * Main initializing procedure
 ****************************************************************************/
void Yatuli::init(uint8_t _pin, int32_t _start, int32_t _end, uint16_t _step, uint16_t _edgeStep) {
    pin = _pin;
    start = _start;
    end = _end;
    edgeStep = _edgeStep;
    if (_step < 10) _step = 10;     // force 10hz minimum step
    step = _step;

    // set the analog pin to input
    pinMode(pin, INPUT);

    // update adc values
    _osadc();

    // some defaults
    base = start;
    value = start;

    // defaults to unlock state
    lock = false;
}


/*****************************************************************************
 * Use it when you need to start in a specific spot on the valid range
 ****************************************************************************/
void Yatuli::set(int32_t init_value) {
    // update adc values
    _osadc();

    // internal var
    int32_t shift = (int32_t)(adc/10) * step;

    // limit checks
    if (init_value > end or init_value < start) {
        // out of range; set to lower end + actual ADC
        base = start;
        value = base + shift;
    } else {
        // in range; set
        base = init_value - shift;
        value = init_value;
    }
}


/*****************************************************************************
 * This is the one you NEED to run in every loop cycle to update value
 ****************************************************************************/
void Yatuli::check(void) {
    // lock flag
    if (lock) return;

    // internal vars, statics as they are used repeatedly in this.
    static int16_t lastAdc = adc;
    static uint32_t newTime = millis();
    static boolean adcDir;

    // update adc values
    _osadc();

    // simplifying the math ahead
    bool up   = (adc > LIMITHIGH) && (value < end - edgeStep);
    bool down = (adc < LIMITLOW) && (value > start + edgeStep);

    // out of operative range?
    if (up || down) {
        // ok, big jumps at a pace
        if (millis() > newTime) {
            // in which direction
            long t = 1;
            if (down) t = -1L;

            // move
            value += (edgeStep * t);
            base += (edgeStep * t);

            // reset pace timer
            newTime = millis() + PACE;
        }
    } else {
        // we are in the operative range
        // flutter fix, from bitx amunters raduino code, author Jerry KE7ER

        // direction detectors... (re-using vars)
        up   = (adc > lastAdc) && (adcDir == 1 || (adc - lastAdc) > 5);
        down = (adc < lastAdc) && (adcDir == 0 || (lastAdc - adc) > 5);

        // check it now
        if (up || down) {
            // flag about the direction of the movement
            if (adc > lastAdc) adcDir = 1;
            else               adcDir = 0;

            // save the last adc & old value in case it falls outside range...
            lastAdc = adc;
            int32_t oldValue = value;

            // force an update
            value  = base + (int32_t)(adc / 10) * step;

            // force a consistent step interval
            value /= step;
            value *= step;

            // check limits
            if (value > end or value < start) value = oldValue;
        }
    }
}


/*****************************************************************************
 * Oversampling of the ADC in 12bits from 0 to 4095 but moved to the more
 * convenient range of -2047 to +2047 to easy some computations.
 * Now as per AVR121 oversampling recommendations.
 ****************************************************************************/
void Yatuli::_osadc(void) {
    // lock flag
    if (lock) return;

    // internal var
    int32_t t = 0;

    // In normal conditions we have an ADC from 0 - 1023 (10 bits)
    // with oversampling we move to 12 bits for better accuracy
    // 12 bits is 0-4095, and we need to sample a few more times:
    // AVR121 says 4^n times for every n new bit, so we need 2 more bits:
    // 4^n, where n = 2, 4^2 = 16 time for each unique reading
    for (int i = 0; i < 16; i++) t += analogRead(pin);

    // now we shift it to wipe the two least significant bits to get in range.
    adc = (int16_t)(t >> 2);

    // then we center that on zero to get a usable margin of -2074 to +2047
    adc -= 2047;
}


/*****************************************************************************
 * This will emit a -1/0/+1 vector corresponding to the movement on the dial,
 * very useful for option selection, it's supposed to emit about 20 steps per
 * dial rotation (less if you mode it very fast)
 *
 * See OptionSelect example
 ****************************************************************************/
int8_t Yatuli::dir(void) {
    // lock flag
    if (lock) return 0;

    // internal var
    static int16_t lastAdcDir = adc;

    // calc the difference and scale to 20 tick per dial rotation
    int16_t result = (adc - lastAdcDir)/DIRTICKS;

    // not moved enough
    if (result == 0) return 0;

    // has moved enough, set last state
    lastAdcDir = adc;

    // right?
    if (result > 0) return 1;

    // if you get here then left
    return -1;
}
