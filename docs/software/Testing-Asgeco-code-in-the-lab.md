## Overview

The page describes how to setup and run tests with an Arduino board.

## Steps

### Setting up.

The required wiring is the followin:

| Port | Usage | connect |
| --- | --- | --- |
| Port | Usage | connect |
| --- | --- | --- |
| A0  | CURRENT SENSOR | connect to POT1 |
| A2  | 24V BATT SENSE | connect to POT2 |
| A3  | GND = Engine NOT running | to GND |
| A4  | OIL | to GND |
| D2  | GND>HI = toggle manual request | connect to switch button |
| D3  | STARTER | connect to LED |
| D7  | GND = Valve open |     |

### Procedure for manual startup

|     |     |     |
| --- | --- | --- |
| 1   | A3,A4:GND,D7:HI,POT1:MAX | Resting state |
| 2   |     | Push button |
| 3   | D7:GND | OPen valve |
| 4   | lower POT1 | drop current |
| 5   | A3:HI | set engine running |

### Procedure for manual shutdown

|     |     |     |
| --- | --- | --- |
| 1   |     | Push button |
| 2   |     | wait for cooldown timeout |
| 3   | D7:HI | Close valve |
| 4   | A3:LO | set engine not running |

See Also
--------