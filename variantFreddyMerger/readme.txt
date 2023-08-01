These two files (variant.cpp and variant.h) enable the use of a second hardware serial port on Arduino MKRZero "Serial2".
Rx Pin is MKR Board pin D7, Tx Pin is MKR Board pin D6.
To disable the second serial port, simply comment out #define ENABLE_SERIAL2 on line 160 in variant.h.

Arduino IDE on PC: Copy and replace the files in location below with the files in this folder:
C:\Users\<User Name>\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.8.13\variants\mkrzero

Arduino IDE on MAC: Copy and replace the files in location below with the files in this folder:
/Users/<User Name>/Library/Arduino15/packages/arduino/hardware/samd/1.8.13/variants/mkrzero