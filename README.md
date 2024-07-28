# OpenSerialDebugger:

The reason I implemented this software: recently I'm trying to make a close-looped stepper motor driver for my robot arm project. I want to tune PID and also want to control the motor by just cliking some buttons. But I don't find any serial debugger software which support both scenarios. There are some software which has buttons like "set kp", "set ki" to tune the pid, but can't control motor like "enable motor" or "set position".
Indeed I can write some hardcore raw serial data byte in the code to communicate with microcontroller, but in this way I need manually modify the parameters in code and flash again. So why not do it in a more intuitive and elegant way? Only need a single software for various scenarios.

## Introduction
This is a special serial debugger, it has the basic serial debugger functionalities: serial monitor, serial plotter, similar as most serial debuggers.
Based on it, it provides a flexible and scalable way to interacts with microcontroller:
- A serial based protocol is defined for carring the customized command and the data. User can define desired commands for different scenario, project or hardware, the OpenSerialDebugger will parse the commands and list them in UI, so user can assign command for each button.
- The only cost is to write a switch case block to handle the commands. (you can also copy paste my example:)) 

### Packet structure
![packet_header](<截屏2024-07-25 21.13.31-1.png>)

This is a 8 byte serial based packet, contains the following field:
- Magic number: to verify if packet is valid
- Chunk num:    default 1, this is reserved for later extenstion to carry multiple data.
- CMD:          command number.
- Data:         32 bit raw data payload, sufficient for microcontroller.

### Command

#### Command type:
The interaction between OpenSerialDebugger and microcontroller hardware is abstracted as 3 command type: Set, Get and Switch(Act)
- Set: host -> hardware. Let microcontroller set a parameter or variable. Such as set Pi, set Pd.
- Get: Hardware -> host. Once micerocontroller received this command, it should send current parameter value to host, like get speed.
- Act(Switch): Host -> hardware, microcontroller execute an action, like enable motor, reset.
#### Command name and command number:
Each command should have a name and an unique id, fomr 0x00 - 0xff.
#### Data type
Currenly int, uint and float are supported, all 32 bit length.

The following is an eaxmple how to define commands.
```bash
Set kp, SET, 0x01, int
Set ki, SET, 0x02, float
Set kd, SET, 0x03, uint

Get Position, GET, 0x04, int
Get Velocity, GET, 0x05, float

Stop Motor, SWITCH, 0x06, bool
Enable Motor, SWITCH, 0x07, bool
```
## UI

![UI Introduction](images/ui_introduction.jpeg)
1. Serial port setting.
2. Serial plotter, extract and plot numbers(VOFA+ fire water format).
   support auto mode and save data to CSV.
3. Serial monitor. (Currently timestamp not supported)
4. Serial send monitor, user can send message via serial port.
5. Load customized commands, user can define customized commands in a file and load the commands to the command list in 6&7.
6. & 7. After loading the customized commands, user can click the combowidget and select a command name from the command list. For Set command, give it a value and click send button, the command will be sent to microcontroller.

![select_command](images/protocol.PNG)

## Code exapmle on microcontroller side.
This video shows an simple arduino example. Using just include the packet.h and add packet handling function, 
you can easily control the led.

<video controls src="OpenSerialDebugger_Led_simple_demo.mp4" title="Demo"></video>