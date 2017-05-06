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

#include "Arduino.h"
#include "yatuli.h"


/*****************************************************************************
 * Main initializing procedure
 ****************************************************************************/
void Yatuli::init(uint8_t _pin, uint32_t _start, uint32_t _end, uint16_t _step, uint16_t _edgeStep) {
    pin = _pin;
    start = _start;
    end = _end;
    edgeStep = _edgeStep;
    
    // step must be greater than 10 always
    if (_step < 10) _step = 10;
    step = _step/10;

    // set the analog put to input
    pinMode(pin, INPUT);

    // update adc values
    adc = analogRead(pin);
    lastAdcDir = 0;

    // calc current position
    base = start;
}


/*****************************************************************************
 * Use it when you need to start in a specific spot on the valid range
 ****************************************************************************/
void Yatuli::set(uint32_t init_value) {
    // update adc values
    adc = analogRead(pin);

    // limit check, if out of range: no-go
    if (init_value > end or init_value < start) return;
    
    // put the internal variables in the correct place for the pot being on
    // the passed position
    base = init_value - (step * (-512L + adc));
}


/*****************************************************************************
 * This is the one you NEED to run in every loop cycle, it will return true
 * if the status has changed
 ****************************************************************************/
bool Yatuli::check(void) {
    // update adc values
    adc = analogRead(pin);

    // if adc is on beyond edges always return changed
    // but in a predefined pace
    if (adc <= LIMITLOW or adc >= LIMITHIGH) {
        
        // ok check if the elapsed time passed away
        if (millis() > newTime) {
            // it's safe
            newTime = millis() + PACE;
            return 1;
        } 
    }

    // flutter fix, from bitx amunters raduino, code author Jerry KE7ER
    // first some direction detectors
    bool up   = (adc > lastAdc) && (adcDir == 1 || (adc - lastAdc) > 3);
    bool down = (adc < lastAdc) && (adcDir == 0 || (lastAdc - adc) > 3);
    
    // check it now
    if (up || down) {
        // flag about the direction of the movement
        if (adc > lastAdc) {
            adcDir = 1;
        } else {
            adcDir = 0;
        }

        // save the last adc
        lastAdc = adc;

        // return true
        return 1;
    }

    // if you get here that's because nothing changed
    return 0;
}


/*****************************************************************************
 * Use this to get the value in the range (including edge jumps)
 *
 * It's made optional (not forced), as you may use dir or value at your own
 * way and tricks
 *
 * See ShuttleTune example
 ****************************************************************************/
uint32_t Yatuli::value(void) {
    // calc the values depending on the ADC value
    uint32_t out;
    int32_t delta = (-512L + adc) * step;

    // check for movemnts
    if (adc <= LIMITLOW) {
        // low edge
        out = base - edgeStep + delta;
    } else if (adc >= LIMITHIGH) {
        // high edge
        out = base + edgeStep + delta;
    } else {
        // normal move
        out = base + delta;
    }
    
    if (adc <= LIMITLOW or adc >= LIMITHIGH) base = out - delta;

    return out;
}


/*****************************************************************************
 * This will emit a -1/0/+1 vector corresponding to the movement on the dial,
 * very useful for option selection, it's supposed to emit about 50 steps per
 * dial rotation
 *
 * See OptionSelect example
 ****************************************************************************/
int16_t Yatuli::dir(void) {
    // calc the difference
    int16_t result = (int16_t)(adc) - (int16_t)(lastAdcDir);

    // limited to ~50 steps per rotation of the dial
    if (abs(result) < 20) return 0;

    // 
    if (result > 0) result =  1;
    if (result < 0) result = -1;

    // set last state
    lastAdcDir = adc;

    // return
    return result;
}
