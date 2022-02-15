Generator controller
--------------------

The generator controller hardware must be powerful enough to be able to control the following:

|     | HW required | I/O type | notes |
| --- | --- | --- | --- |
|     | HW required | I/O type | notes |
| --- | --- | --- | --- |
| Engine Starter Solenoid | 20A relay | Digital Output |     |
| Engine Starter Current Sensor | hall effect sensor | Analog Input |     |
| Fuel Valve Open Solenoid | 10A relay | Digital Output |     |
| Fuel Valve Close Solenoid | 10A relay | Digital Output |     |
| Fuel Valve Sensor | microswitch | Digital Input | to sense if it's open or closed |
| 3 Mains Solid State Relays |     | Digital Output |     |
| Oil sensor |     | Digital Input |     |
| Generator Battery voltage | resistors | Analog Input |     |
| Arduino voltage | resistors | Analog Input |     |
| Generator running sensor | relay | Digital Input | to know if the generator is running and producing power |
| Network | Ethernet card SPI BUS | Digital I/O 4x |     |
| Ethernet Link |     | Digital input | to know if network is connected or not |
| Soft Switches | switches or selector | Digital Input 2x | to manually control system. Turn engine on or off, or other things gen purpose things |
| Hard Switches | 6 or 7 3 way rotary switches | –   | to manually set an output to high or low, overriding whatever the software says. Position 0: auto, 1: always OFF, 2: always ON.<br><br>You can force pins down or up.  It will simply cause current draw through the arduino so you have to be careful to stay within spec.  That can be done by adding resistors.  If the resistor that you use on the cutoff switch is smaller than the one on the output pin, you should be able to change the output voltage enough to switch the value.  <br><br>You might also try looking up "wired-or" or "wired-and" circuits.  Depending on which way you want the switch to force (on or off). |

|     |     |
| --- | --- |
| Digital I/O | 14  |
| Analog I/O | 3   |