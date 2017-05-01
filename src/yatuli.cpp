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
    // analog pin
    pin = _pin;
    start = _start;
    end = _end;
    step = _step;
    edgeStep = _edgeStep;

    // set the analog put to input
    pinMode(pin, INPUT);

    // update adc values
    _update();
    lastAdcDir = adc;

    // calc current position
    base = start;

    // last value update
    lastValue = start;
}


/*****************************************************************************
 * Use it when you need to start in a specific spot on the valid range
 ****************************************************************************/
void Yatuli::set(uint32_t init_value) {
    // update adc values
    _update();

    // limit check, if out no-go
    if (init_value > end or init_value < start) return;
    
    // put the internal variables in the correct place for the pot being on
    // the passed position
    base = init_value - (uint16_t)(step * relative);

    // last value update
    lastValue = init_value;
}


/*****************************************************************************
 * Private procedure, it will update the ADC and calc the relative delta
 ****************************************************************************/
void Yatuli::_update(void) {
    int16_t sum = 0;

    // aggregate the ADC samples
    for (byte i = 0; i < ST_SAMPLES; i++) {
        sum += analogRead(pin);
    }

    // average them
    adc = sum/ST_SAMPLES;
    
    relative = map(adc, (LIMITLOW + 1), (LIMITHIGH -1), -500L, 500L);
}


/*****************************************************************************
 * This is the one you NEED to run in every loop cycle, it will return true
 * if the status has changed
 ****************************************************************************/
boolean Yatuli::check(void) {
    // update adc values
    _update();

    // if adc is on beyond edges always return changed
    //  but in a predefined pace
    if (adc <= LIMITLOW or adc >= LIMITHIGH) {
        
        // ok check if the elapsed time passed away
        if (millis() > newTime) {
            // it's safe
            newTime = millis() + PACE;
            return 1;
        } else {
            // not yet
            return 0;
        }
    }

    // it has moved?
    if (lastAdc != adc) {
        lastAdc = adc;
        //output
        return 1;
    } else {
        // nothing has changed
        return 0;
    }
}


/*****************************************************************************
 * Use this to get the value in the range (including edge jumps)
 *
 * It's made optional (not forced), as you may use dir or move at your own
 * way and tricks
 *
 * Se ShuttleTune example
 ****************************************************************************/
uint32_t Yatuli::value(void) {
    // calc the values depending on the ADC value
    // out value
    uint32_t out;
    int32_t delta = (int32_t)(relative) * step;

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
    
    // then if it's possible to move there
     if (out > end or out < start) {
        // can't be applied
        return lastValue;
    } else if (adc <= LIMITLOW or adc >= LIMITHIGH) {
        // re-set the base
        base = out - delta;
    }

    // return it
    lastValue = out;
    return out;
}


/*****************************************************************************
 * This will emit a -1/0/+1 vector corresponding to the movement on the dial,
 * very useful for option selecting, it's emitted a PACE intervals.
 *
 * Se OptionSelect example
 ****************************************************************************/
int16_t Yatuli::dir(void) {
    // update adc values
    _update();

    // ok check if the elapsed time passed away
    if (millis() < newTime) {
        // not yet
        return 0;
    } else {
        // set new interval
        newTime = millis() + PACE;

        // now we calculate
        int16_t result = adc;
        result -= lastAdcDir;

        //detect direction
        if (result > 0) result =  1;
        if (result < 0) result = -1;

        // set the next last and return
        lastAdcDir = adc;
        return result;
    }

}
