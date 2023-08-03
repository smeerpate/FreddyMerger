# FreddyMerger
MIDI merger using two hardware serial ports on the Arduino MKRZero.
Based on lathoub's DualMerger.ino in the FortySevenEffects arduino_midi_library examples folder which uses one hardware serial port and the USB port.

This example uses two hardware serial ports Serial1 and Serial2.
In order to be able to use Serial2 check the readme.txt in the variantFreddyMerger folder.

## Hardware
The hardware MIDI ins and outs might look something like this:
![MIDI ins and outs, a snippet from FreddyMerger.sch](frontEnd.png)
Using 3.3V for the MIDI outs isn't compliant with the standard (it should be 5V I think...) but it works in my setup.
I might add 2 transistors to have the outs on 5V.
