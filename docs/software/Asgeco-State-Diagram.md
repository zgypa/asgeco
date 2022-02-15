# Asgeto State Tables

A list of various states. See also [automatic cycles](asgeco%20states/auto_cycle.csv), [manual cycles](asgeco%20states/manu_cycle.csv).

### State Diagram

![](/download/attachments/3145797/ASGC%20States.png?version=2&modificationDate=1380460586437&api=v2)

### State Description

| #   | HEX | description |
| --- | --- | --- |
| #   | HEX | description |
| --- | --- | --- |
| 10  |     |     |
| 11  |     |     |
| 12  |     |     |
| 13  |     | Starter is starting engine, engine not running alone yet. |
| 14  |     | Engine running alone, but not at full speed yet. Time between starter turned off and engine at full regimen. |
| 15  |     |     |
| 16  | 0x5043 |     |
| 17  |     |     |
| 18  |     |     |
| 19  |     |     |
| 220 | 0x1041 | Engine running, warmed up, Mains OFF, Manual request on, AUTO off |
| 20  | 0x3041 | Engine running, warmed up, Mains ON, Manual request on, AUTO off |
| 21  |     |     |
| 22  |     |     |
| 23  |     |     |
| 24  |     |     |
| 25  |     |     |
| 26  |     |     |
| 27  |     |     |
| 28  |     |     |
| 29  |     |     |
| 52  |     |     |
| 90  |     |     |
| 120 |     |     |
| 120 |     |     |
| 130 |     |     |
| 140 |     |     |
| 150 |     |     |
| 160 |     |     |
| 170 |     |     |
| 230 |     |     |
| 240 |     |     |
| 241 |     |     |
| 242 |     |     |
| 243 |     |     |
|     | 0x1001 | Engine running, Valve on |
|     | 0x3005 | Engine running, cooling down, mains ON, valve open |
|     | 0x1005 | Engine running, cooling down, mains OFF, valve open |
|     | 0x5005 | Engine running, cooling down, mains OFF, valve open, Wating |
|     | 0x4005 | Engine running, cooling down, mains OFF, valve closed , Wating |
|     | 0x4004 | Engine stopped, cooling down, mains OFF, valve closed, Wating |
|     | 0x0004 | Engine stopped, cooling down, mains OFF, valve closed |
|     | 0x0000 | Engine stopped, mains OFF, valve closed |
