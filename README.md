# grinduino

Nuova Simonelli MDX coffee grinder with Arduino control.

Note - this is for self-educational purposes only.  I am not an electrical engineer!

## Overview

A lack of consistency of grinding the correct amount of coffee led to the birth of this project.  This project is intended to provide Arduino control for a doser style grinder with an on-off switch.  With automation of the grinding process, less coffee waste is expected which helps reduce coffee bean costs.

An interface will allow the use of three timings; single dose, double dose and a purge function.  The purge function allows the stale grounds that have accumulated in the chute of the grinder to be flushed out.  Each of these functions will be able to be modified through an LCD display and five button inputs.

For testing purposes, an Arduino will be used to handle the logic and set the solid state relay state.  Menu items can be easily added as features are requested / developed.

## Electronics

Bill of materials:

- 2x 20A solid state relays (double isolation)
- 1x Mini-Din connectors and sockets for control signal
- 1x Arduino for prototyping
- 1x LCD / keypad shield for the Arduino
- Casing

## License

This project was initially forked from https://github.com/jamebsVS/grinduino.
