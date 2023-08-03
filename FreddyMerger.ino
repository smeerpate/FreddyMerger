#include <MIDI.h>

/*
  In order to be able to use Serial2 on Arduino SAMD21 variants 
  1) 
    a) PC: go to folder C:\Users\<User Name>\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.8.13\variants\mkrzero
    b) MAC: go to folder /Users/<User Name>/Library/Arduino15/packages/arduino/hardware/samd/1.8.13/variants/mkrzero
  2) Overwrite the files below with the equally named files in the variantFreddyMerger folder
    a) variant.h
    b) variant.cpp

  Only implemented for Arduino MKR Zero.
  Might work for other Arduino SAMD12 variants as well, provided the variant.h and variant.cpp files are altered for the respective variant.
*/

#define BUILDINFO         22091011L // jjmmddhh
#define APP_LOOP_US       1000UL

#define LED_MIDIA         0 // arduino pin
#define LED_MIDIB         1 // arduino pin
#define LED_MIDIOUT       2 // arduino pin


uint32_t midiMergeTask();
void ledMidiATask(uint32_t uiMidiStatusFlags);
void ledMidiBTask(uint32_t uiMidiStatusFlags);
void ledMidiOutTask(uint32_t uiMidiStatusFlags);
void ledRcFilter(double* xn, double* yn, double* xn1, double* yn1);

// Midi flags:
//  bit0: Got input from MIDI A
//  bit1: Got input from MIDI B
//  bit2: Outputting on MIDI A
//  bit3: Outputting on MIDI B
uint32_t muiMidiFlags = 0x00000000;
unsigned long mulTime = 0;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiA);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiB);

void setup()
{
    // Initiate MIDI communications, listen to all channels
    midiA.begin(MIDI_CHANNEL_OMNI);
    midiB.begin(MIDI_CHANNEL_OMNI);
    pinMode(LED_MIDIA, OUTPUT);
    pinMode(LED_MIDIB, OUTPUT);
    pinMode(LED_MIDIOUT, OUTPUT);
}

void loop()
{
  if (micros() > mulTime + APP_LOOP_US)
  {
    mulTime = micros();
    muiMidiFlags = midiMergeTask();
    ledMidiATask(&muiMidiFlags);
    ledMidiBTask(&muiMidiFlags);
    ledMidiOutTask(&muiMidiFlags);
  }
}

uint32_t midiMergeTask()
{
  uint32_t uiFlags = 0;

  if (midiA.read())
    {
        uiFlags |= 1 << 0;
        // Thru on A has already pushed the input message to out A.
        // Forward the message to out B as well.
        midiB.send(midiA.getType(),
                   midiA.getData1(),
                   midiA.getData2(),
                   midiA.getChannel());
        uiFlags |= 1 << 2;
    }

    if (midiB.read())
    {
        uiFlags |= 1 << 1;
        // Thru on B has already pushed the input message to out B.
        // Forward the message to out A as well.
        midiA.send(midiB.getType(),
                   midiB.getData1(),
                   midiB.getData2(),
                   midiB.getChannel());
        uiFlags |= 1 << 3;
    }
    return uiFlags;
}

void ledMidiATask(uint32_t* uiMidiStatusFlags)
{
  double xn = 0.0;
  double yn = 0.0;
  static double xn1 = 0.0;
  static double yn1 = 0.0;

  if(*uiMidiStatusFlags & (0x00000001 << 0))
  {
    xn = 255.0;
    yn1 = 255.0;
  }
  else
  {
    xn = 0;
  }

  ledRcFilter(&xn, &yn, &xn1, &yn1);
  analogWrite(LED_MIDIA, (int)yn);
  xn1 = xn;
  yn1 = yn;
}

void ledMidiBTask(uint32_t* uiMidiStatusFlags)
{
  double xn = 0.0;
  double yn = 0.0;
  static double xn1 = 0.0;
  static double yn1 = 0.0;

  if(*uiMidiStatusFlags & (0x00000001 << 1))
  {
    xn = 255.0;
    yn1 = 255.0;
  }
  else
  {
    xn = 0;
  }

  ledRcFilter(&xn, &yn, &xn1, &yn1);
  analogWrite(LED_MIDIB, (int)yn);
  xn1 = xn;
  yn1 = yn;
}

void ledMidiOutTask(uint32_t* uiMidiStatusFlags)
{
  double xn = 0.0;
  double yn = 0.0;
  static double xn1 = 0.0;
  static double yn1 = 0.0;

  if(*uiMidiStatusFlags & (0x00000001 << 2))
  {
    xn = 255.0;
    yn1 = 255.0;
  }
  else
  {
    xn = 0;
  }

  ledRcFilter(&xn, &yn, &xn1, &yn1);
  analogWrite(LED_MIDIOUT, (int)yn);
  xn1 = xn;
  yn1 = yn;
}

void ledRcFilter(double* xn, double* yn, double* xn1, double* yn1)
{
  /*
  H(s)=W0/(s+W0)
  Eg. W0=1rad/s = 0.16Hz -> H(s)=(1)/(s+1)
  T sample = APP_LOOP_US = 1000ms
  dicrete: H(z)=0.001/(z-0.999)
  yn = 0.999*yn1 + 0.001/2*xn + 0.001/2*xn1
  yn = 0.999*yn1 + 0.0005*xn + 0.0005*xn1
  */
  *yn = 0.999*(*yn1) + 0.0005*(*xn) + 0.0005*(*xn1);
}
