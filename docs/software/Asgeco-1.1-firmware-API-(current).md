The Asgeco firmware accepts connections on TCP port 80.

Reading
-------

The status can be read from the string it returns with the following format:

[?](#)

`ASGECOv2 <FIELD2> <FIELD3> ...`

The fields are space separated (for easy parsing with awk, using standard field separator value), and are the following:

| FIELD # | content | r/w | var name | comments |
| --- | --- | --- | --- | --- |
| FIELD # | content | r/w | var name | comments |
| --- | --- | --- | --- | --- |
| 1   | ASGECOv2 | na  |     | protocol name and version number |
| 2   | 1\|0 | rw  | STARTER | starter Pin status |
| 3   | 1\|0 | rw  | ONSOLENOID | on Solenoid Pin status |
| 4   | 1\|0 | rw  | OFFSOLENOID | off Solenoid Pin status |
| 5   | 1\|0 | rw  | MAINS | mains relay Pin status |
| 6   | 1\|0 | rw  | MANU\_REQUEST | A manual request has been generated either from HW switch or from SW web API. |
| 7   | unsigned int | ro  | BATTS | PV battery array mV |
| 8   | int | ro  | VCC | Vcc of Arduino mV |
| 9   | 0-1023 | ro  | ST\_CUR | starter current pin |
| 10  | 1\|0 | ro  | ENGINE | Engine state: 1=engine is on, 0=engine is off |
| 11  | 1\|0 | ro  | WARMINGUP | Engine state: 1=engine is warming up, 0=is not warming up |
| 12  | 1\|0 | ro  | COOLINGDOWN | Engine state: 1=engine is cooling down, 0=is not cooling down |
| 13  | 1\|0 | ro  | WAITING | Waiting state: there is a timer ticking. |
| 14  | 1\|0 | rw  | MODE | 0=Manual Operation, 1=Automatic (PV voltage based) operation |
| 15  | 1\|0 | rw  | FATAL | error occurred when this is 1. Reset necessary. |
| 16  | 1\|0 | ro  | AUTO\_REQUEST | An auto request has been generated because battery voltage < GENON |
| 17  | 1\|0 | ro  | AUX\_STATE | Current state of AUX switch: 0=switch is closed, 1=switch is open |
| 18  | 0-3 | ro  | TIMEOUTS | Number of failed attempts to start the engine |
| 19  | 1\|0 | rw  | SECS\_TOT | Total time the engine has been running in seconds |
| 20  | 1\|0 | ro  | SECS | Time the engine has been running since last startup in seconds |
| 21  | int | rw  | GENON | Voltage at which generator should start in mV. This value is stored in EEPROM |
| 22  | int | rw  | GENOFF | Voltage at which generator should stop in mV. This value is stored in EEPROM |
| 23  | 1\|0 | ro  | VALVE | Fuel Valve State: 1: Open, 0: Closed |
| 24  | 1\|0 | ro  | <UNUSED> | Always returns 0 |
| 25  | int | rw  | VCONV | Conversion factor for battery voltage calculation. When read, reads vConv, which needs to be divided by 1000 (to avoid using floats) and multiplied by the voltage perceived by Arduino. When set, use mV of current battery voltage as reported by TriStar. This value is stored in EEPROM |
| 26  | 1\|0 | ro  | OIL | Oil pressure sensor. When 1, pressure low, when 0 pressure OK |
| 27  | int | rw  | WARMINGUP | specify warm up time, before activating load |
| 28  | int | rw  | COOLINGDOWN | specify cool down time, before deactivating load |
| 29  | 0-255 (byte) | rw  | MINIMUMRUNTIME | specify the minimum amount of minutes the generatore should run when an auto request has been triggered |
| 30  | 1\|0 | ro  | OFF\_LOCK | When 1, AUTO\_REQUEST will not get set, when in AUTO mode |
| 31  | 1\|0 | wo  | RESTART | When 1, system will reboot |
| 32  | 0-255 | ro  | ERROR | System Error Codes |
| 33  | 0-65535 | ro  | STATE | System state 16bit bitfield in int |

Writing
-------

To modify the status, simply send the following codes to port TCP port 80. This can be done in the URL as well such as:

[?](#)

`http:``//asgeco/?ASGECOv2&<var1>=<value1>&<var2>=<value2>...`

Refer to the table above to see which codes can be set.

Limitations
-----------