Overview
--------

This page describes the structure and organization of the Asgeco code.

Asgeco v3 has been entirely rewritten to be "multitasking". What i mean by multitasking is that there is no method that locks up the CPU for more than a few hundred ms. Any method that needs to wait for a longer period of time, makes use of global state flags to set the code in a waiting state. Any other state can be in a waiting state, so then an appropriate method picks up the task, when the code is looped again, sees if it's time to keep moving (in which case it changes the machine state by setting/unsetting a flag), or if we still have to wait, and does nothing.

This method allows for any other function to change state even during a wait. It also allows for logging during a waiting state, i.e. it is always possible to use the HTTP API, even if the code is waiting for something to happen.

Structure
---------

### asgeco3.pde

This is the main file. It contains the setup and loop functions. This file contains minimal code, and invokes the HTTPserver, the state updater and the Generator controller.

### LocalLibrary

Contains functions to be used throughout the project. For now, it only contains the logg(String) function, a wrapper to Syslog.logger.

### GeneratorLibrary

See GeneratorLibrary.cpp code for detailed documentation at the beginning of the file.

### Asgeco2API

This is the HTTP API. The main function here is HTTPServer(). When invoked in loop(), it will so to speak, turn on the HTTP API.

### States

#### How to read states

There are two kinds of states: the "human" readable ones, and the low level states. The state is defined by the status of a bunch of variables, which are stored in the unsigned int **engineState**. Each bit of **engineState** corresponds to a flag of some part of the system. So the entire variable defines precisely the state of the entire system.

To decode the **engineState**, it is necessary to know what each bit of it represents. This is defined in **GeneratorLibrary.h**, and reported here:

|     |     |     |
| --- | --- | --- |
| ENGINE | 0   | 1=engine running |
| WARMINGUP | 1   |     |
| COOLINGDOWN | 2   |     |
| TIMEOUTS | 3, 4 | counter for upto 3 timeouts. uses 2 bits |
| MODE | 5   | 0=Manual Operation, 1=Automatic (PV voltage based) operation |
| MANU\_REQUEST | 6   | 1=we want to start the generator regardless of the situation |
| AUTO\_REQUEST | 7   | 0=PV batteries OK, 1=PV batteries low |
| AUX\_STATE | 8   | 0=AUX switch open, 1=AUX switch closed |
| FREE | 9   |     |
| FREE | 10  |     |
| STARTER | 11  | 1=starter is turning |
| VALVE | 12  | 1=diesel valve is open |
| MAINS | 13  | 1=SSD relays are close ![(lightbulb)](/s/-i0koqh/8703/189cb2l/_/images/icons/emoticons/lightbulb_on.svg) |
| WAITING | 14  | 1=software is waiting for something to timeout |
| FATAL | 15  | 1=system hangs for fatal error |

#### State Macros

In GeneratorLibrary.h i have defined some state macros for some commonly used states. This makes the code much more human readable.

#### State Masks

State masks are bit masks used to filter out unwanted or wanted bits from states. Since the total number of possible states is around 65K, i cannot define them all. So i make use of masks to only test for those bits which interest me in that moment. So for example when i'm testing for a state, i usually don't need to know the state of the control bits, as i can simply check for a request with the function isValidRequest(), which takes care of the individual control situations. So i can AND the mask with the current engine state, effectively zeroing out all of the control bits.

### Open Valve Timeout

There is logic in place to make sure that the system doesn't keep trying to open the valve if the microswitch isn't working properly. The code uses the TIMEOUTS bitfield, the same one used for starting attempts, to count 3 open/closed valve attempts. If they fail, system goes in FATAL state.

### Manual request buffer time

This is used primarily to reduce erroneous manual requests due to noise on the line. When the system detects the button has been pressed, it keeps checking for AUX\_BUFFER\_TIME ms the button state. If during that time it changes, then the press is considered noise and not acted upon.

See Also
--------

*   [Asgeco State Diagram](/pages/createpage.action?spaceKey=CIAP&title=Asgeco+State+Diagram)