# Yatuli Arduino Library #

Yatuli = Yet another Tune Lib, go cheap: use a linear volume control instead a rotary encoder.

## Inspiration ##

This work is based on the previous work of these people:

* [Bitx40v3 users and Raduino developers:](https://github.com/afarhan/bitx40) The original code.
* [Allard Munters PE1NWL](https://github.com/amunters/bitx40/) The updated version of the original code.
* [Jerry KE7ER:](#) With the oversampling and flitter fix.

## Features ##

This is a kind of linear tuning and in the center with big steps on the edges:

* Selectable Analog pin.
* Range set on initialization (Start-End).
* Step selection on initialization, down to 10 Hz (real minimum).
* Edge jumps on initialization.
* Set a start point in the passed range.
* Flutter fix from Jerry KE7ER.
* Direction finding for Option selection (useful in menu selections).
* You can dynamically reset the range and start value while running (useful in setups).
* Negative values are supported in all the range (start, end & value)
* Range is handled by 32 bit signed values, so it will work from -/+ 2.4G values.

See the examples bundled with the lib for use cases.

## Hardware ##

We use a linear volume resistor with extremes connected to GND and +Vcc of the Arduino, for stability you must put a 1uF polarized capacitor across GND and +Vcc at the variable resistor legs, then a 10nF (103) capacitor across GND and the wiper.

Then the wiper is connected to an Analog input of the Arduino, see the examples bundled with this lib.

## Author & contributors ##

The only author is Pavel Milanes, CO7WT, a cuban amateur radio operator; reachable at pavelmc@gmail.com, Until now I have no contributors or sponsors.

## Where to download the latest version? ##

Always download the latest version from the [official github repository](https://github.com/pavelmc/Yatuli)

See ChangeLog.md & version files on this repository to know what is your version and what are the latest changes to it.

## If you like to give thanks... ##

No payment of whatsoever is required to use this code: this is [Free/Libre Software](https://en.wikipedia.org/wiki/Software_Libre), nevertheless donations are very welcomed.

I live in Cuba island and the Internet/Cell is very expensive here (USD $1.50/hour), you can donate anonymously internet time or cell phone air time to me via [Ding Topups](https://www.ding.com/) to keep me connected and developing for the homebrew community.

If you like to do so, please go to Ding, select Cuba, select Cubacell (for phone top up) or Nauta (for Internet time)

* For phone topup use this number (My cell, feel free to call me if you like): +53 538-478-19
* For internet time use this user: co7wt@nauta.com.cu (that's not an email but an user account name)

Thanks!
