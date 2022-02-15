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
| 6   | 1\|0 | ro  | AUX | input AUX pin: 0=switch is closed, 1=switch is open |
| 7   | int | ro  | BATTS | PV battery array mV |
| 8   | int | ro  | VCC | Vcc of Arduino mV |
| 9   | 0-1023 | ro  | ST\_CUR | starter current pin |
| 10  | 1\|0 | ro  | ES  | Engine state: 1=engine is on, 0=engine is off |
| 11  | 1\|0 | ro  | WARMINGUP | Engine state: 1=engine is warming up, 0=is not warming up |
| 12  | 1\|0 | ro  | COOLINGDOWN | Engine state: 1=engine is cooling down, 0=is not cooling down |
| 13  | 1\|0 | ro  | SLEEP | Sleep state |
| 14  | 1\|0 | rw  | AUXDISABLE | AUX disabled state: 1=AUX switch has no effect, 0=normal operation |
| 15  | 1\|0 | rw  | FATAL | error occurred when this is 1. Reset necessary. |
| 16  | 1\|0 | rw  | LASTAUXSTATE | Last AUX state, used to unlock AUX disabled |
| 17  | 1\|0 | rw  | AUTO | Automatic: 1=consider PV battery voltage to decided when to start engine. 0=consider only AUX switch |
| 18  | 0-3 | ro  | TIMEOUTS | Number of failed attempts to start the engine |
| 19  | 1\|0 | rw  | SECS\_TOT | Total time the engine has been running in seconds |
| 20  | 1\|0 | ro  | SECS | Time the engine has been running since last startup in seconds |
| 21  | int | rw  | GENON | Voltage at which generator should start in mV. This value is stored in EEPROM |
| 22  | int | rw  | GENOFF | Voltage at which generator should stop in mV. This value is stored in EEPROM |
| 23  | 1\|0 | rw  | REMOTE\_ENABLE | Remote Enable |
| 24  | 1\|0 | rw  | REMOTE\_CONTROL | Remote Control |
| 25  | int | rw  | VCONV | Conversion factor for battery voltage calculation. When read, reads vConv, which needs to be divided by 1000 (to avoid using floats) and multiplied by the voltage perceived by Arduino. When set, use mV of current battery voltage as reported by TriStar. This value is stored in EEPROM |

Writing
-------

To modify the status, simply send the following codes to port TCP port 80. This can be done in the URL as well such as:

[?](#)

`http:``//10.1.11.81/?ASGECOv2;<var1>=<value1>;<var2>=<value2>...`

Refer to the table above to see which codes can be set.

Limitations
-----------