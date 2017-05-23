# Yatuli Changelog File #

==================================================
## Release v0.4 (May 22, 2017) (First Public Release) ##

### Improvements ###

* Bug fixes with value calculations, working as expected now.
* Bug fixes with dir() calculations, working as expected now.
* Big code refactory for size/speed.
* Docs update.

===========================================================
## Release v0.3 (May 7, 2017) (private, never published) ##

### Improvements ###

* Range and value can now handle negative integers, needed on setup functions
* Soft set of the 10Hz the hard way, some calcs can lead to jumps on the lower 10 Hz.
* Bug Fix: range edged was not honored on certain conditions

===========================================================
## Release v0.2 (May 6, 2017) (private, never published) ##

### Improvements ###

* Flutter fix from amunters code, thanks to Jerry KE7ER.
* Refactory of the code for size and speed, to match flutter fix fix.
* Update all the docs about this release.

====================================================================
## Initial Release, v0.1 (May 1, 2017) (private, never published) ##

### Basic functionality ###

* Range set on initialization.
* Step selection on initialization.
* Edge jumps on initialization.
* Start point in the passed range.
* All the above is changeable in hot state via public variables.
* Direction finding for Option selection.
