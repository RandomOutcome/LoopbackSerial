# LoopbackSerial Library
 
This library adds memory based serial communication, between software components, to an Arduino device.
___
Author: RandomOutcome  
___
## Local non-IO based loopback library implementing SoftwareSerial interface

[![GitHub release](https://img.shields.io/github/v/release/RandomOutcome/LoopbackSerial?display_name=tag)](https://github.com/RandomOutcome/LoopbackSerial/releases/latest) [![License](https://img.shields.io/github/license/RandomOutcome/LoopbackSerial.svg?maxAge=3600)](LICENSE) [![Examples](https://github.com/RandomOutcome/LoopbackSerial/actions/workflows/platformio.yml/badge.svg?branch=master)](https://github.com/RandomOutcome/LoopbackSerial/actions/workflows/platformio.yml) [![Issues](https://img.shields.io/github/issues/RandomOutcome/LoopbackSerial.svg?maxage=3600)](ISSUES)

The library is based upon the Arduino SoftwareSerial libraries methods, and can be used interchangably with SoftwareSerial & HardwareSerial libraries to support serial communuication within an Arduino device.
### Examples of use:
- Drop in simplified testing of serial interfaces via local loopback without requiring physical connections - e.g. communication with on device software stubs or test harnesses to simulate hardware serial interfaces.
- Intra-device serial communication, e.g. as an software Midi loop solution to allow Midi libraries to communicate with a  Midi service local to the device.
- As a fifo based communications pipe between software components, especially for those components that may needed to be split between devices at a later date with serial based communications between them. 

**To use this library:**

    #include <LoopbackSerial.h>
 
### LoopbackSerial library has the following known limitations:
- FIFO queue size is fixed, determined by the Arduino SoftwareSerial library currently at 64 bytes 
- Does not seek to mirror limitations of SoftwareSerial library (e.g. single reciever or device impacts)
- Does not simulate baudrate - the data can be written and read from the loopback device at device rather than interface speed
- Does not simulate serial handshaking signals (e.g. DTR/DSR/RTS/CRS) or half/full duplex
- Does not support print(), println() or multibyte write methods
### Features
-   Provides local loopback communications
-   Provides a memory based FIFO stream between software components
-   Accessed using standard Arduino serial library methods
-   Able to both send and receive data at the same time
-   Supports multiple loopback streams to used at the same time blocking/impacting each other - subject only to available device memory
-   Each loopback stream is always active (listening)
-   No hardware IOs or Interrupts are utilised
-   Can be used with libraries that support Software Serial or Hardware Serial libraries
### To-Do
- Test, test and more tests
- Implement platformio library integration
- Implement multibyte write, print and println methods
- Review use 'strict' mode parameter to impose limit to one active receving loopback instance at a time.  *Use case = loopback library to emulate SoftwareSerial behaviour more closely to support stub test scenarios*    
### Getting Started

1. Use the Arduino or Platformio Library Manager to install the LoopbackSerial library from github
- *(for now manually add https://github.com/RandomOutcome/LoopbackSerial.git to library dependencies)* 
2. Start coding
#### Example (abreviated)
Software selectable Midi output (MidiLibray based) supporting USB, IO serial and an on device midi 2 DAC service (snippet):
```c++
#include <LoopbackSerial.h>
#include <MIDI.h>

// Create loopback serial interface 
LoopbackSerial  loopSeria1l();

// Create Midi Instances (note MidiLibrary requires HardwareSerial be used here but supports LoopbackSerial & SoftwareSerial)
MIDI_CREATE_INSTANCE(HardwareSerial, loopSerial1, LOOPBACK_MIDI);  // Loopback midi

... 

void setup()
{
    MIDI_LOOPBACK.begin(MIDI_CHANNEL_OMNI);  // Listen to incoming messages on all midi channels
    ...
}

void loop()
{
    midiOutInterface = readConfig('midiTarget');

    // Send note 42 with velocity 127 on channel 1 to configured Midi target
    note = 42;
    velocity = 127;
    channel = 1;
    switch( miniOutInterface ) {
      case USBMIDI:
        LOOPBACK_MIDI.sendNoteOn( note, velocity, channel );
        break;
      case SERIALMIDI:
        SERIAL_MIDI.sendNoteOn( note, velocity, channel );
        break;
      case USBMIDI:
        USB_MIDI.sendNoteOn( note, velocity, channel );
       break;
       ...
    }
    ...
    midi2OnboardDAC.run();
}

midi2OnboardDAC::run()
{
    // Read incoming messages
    LOOPBACK_MIDI.read();
    ...
}   
```

## Methods
### LoopbackSerial()

Create an instance of a LoopbackSerial object. Multiple LoopbackSerial objects may be created (all active).

**Syntax**

    LoopbackSerial()

**Parameters**

    none
    
**Returns**

    none

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial();
```

### available()

Get the number of bytes (characters) available for reading from a software serial port. This is data that has already arrived and stored in the serial receive buffer.

**Syntax**

    MyLoopSerial.available()

**Parameters**

    None

**Returns**

    The number of bytes available to read.

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial =  myLoopSerial();

void setup()  {
    myLoopSerial.begin();
}

void loop() {
    if (myLoopSerial.available() > 0) {
        myLoopSerial.read();
    }
}
```

### begin()

Initalises the FIFO buffer to start processing a stream of data.

**Syntax**

    myLoopSerial.begin()

**Parameters**

    none

**Returns**

    none

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial =  LoopbackSerial();

void setup()  {
    myLoopSerial.begin();
}


void loop() {
    ...
}
```

### isListening()

Tests to see if requested loopback serial object is actively listening (always true), implemented for backward compatibility with SoftwareSerial.

**Syntax**

    myLoopSerial.isListening()

**Parameters**

    none

**Returns**

    boolean

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial portOne();

void setup() {
    // Set the baud rate for the Serial port
    Serial.begin(9600);

    // Initialise the LoopbackSoftware object
    portOne.begin();
}

void loop() {
    if (portOne.isListening()) { 
        Serial.println("portOne is listening!");
    }

    ...
}
```

### overflow()

Tests to see if a LoopbackSerial buffer overflow has occurred. Calling this function clears the overflow flag, meaning that subsequent calls will return false unless another overflow condition occurs. The LoopbackSerial buffer can hold up to 64 bytes.

**Syntax**

    myLoopSerial.overflow()

**Parameters**

    none

**Returns**

    boolean

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial portOne();

void setup() {
    // Set the baud rate for the Serial port
    Serial.begin(9600);

    // Initialise the LoopbackSoftware object
    portOne.begin();
}

void loop() {
    if (portOne.overflow()) {
        Serial.println("portOne overflow!");
    }

    ...
}
```

### peek()

Return the first character available to be read from the FIFO buffer that was received on the loopback serial port. Unlike read(), however, subsequent calls to this function will return the same character. 

**Syntax**

    myLoopSerial.peek()

**Parameters**

    none

**Returns**

    The character read or -1 if none is available.

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial();

void setup() {
    // Initialise the LoopbackSoftware object
    myLoopSerial.begin();
}

void loop() {
    char c = myLoopSerial.peek();
    ...
}
```

### read()

Return the earliest character that was placed into LoopbackSerial object. Once read the subsequent character placed is then available for the next read.

**Syntax**

    myLoopSerial.read()

**Parameters**

    none

**Returns**

    The character read or -1 if none is available.

**Example*

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial();

void setup() {
    // Initialise the LoopbackSoftware object
    myLoopSerial.begin(9);
}

void loop() {
    char c = myLoopSerial.read();
    ...
}
```

### listen()

Implemented for interface compatability with SoftwareSerial, calling has no effect for LoopbackSerial as all objects created are listening.

**Syntax**

    myLoopSerial.listen()

**Parameters**

    none

**Returns**

    Returns true

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial portOne();


// Set up another LoopbackSerial object 

LoopbackSerial portTwo();


void setup() {

    // Set the baud rate for the Serial object

    Serial.begin(9600);


    // Initialise the LoopbackSerial objects

    portOne.begin();

    portTwo.begin();

}


void loop() {

    // Enable LoopbackSerial object to listen - has no effect as all LoopbackSerial objects listen

    portOne.listen();


    if (portOne.isListening()) {

        Serial.println("portOne is listening!");

    } else {

        Serial.println("portOne is not listening!");

    }


    if (portTwo.isListening()) {

        Serial.println("portTwo is listening!");

    } else {

        Serial.println("portTwo is not listening!");

    }

    // The above is an example of a difference with SoftwareSerial - both portOne and portTwo wil report listening 
    // for LoopbackSerial objects rather than only portOne for SoftwareSerial. This difference is by design in order
    // to support multiple loopback serial ports in parallel operation (unlike hardware/software serial ports which 
    // are device resource bound to limit to one at a time.

    ...

}
```

### write()

Appends the parameter data into the LoopbackSerial object's buffer.

**Syntax*

    myLoopSerial.write(val)

**Parameters**

    val: the byte to be writen.

**Returns**

    0 if error (e.g. buffer full)
    1 if byte written sucessfully 

**Example**

```c++
#include <LoopbackSerial.h>

// Set up a new LoopbackSerial object

LoopbackSerial myLoopSerial();

void setup() {
    // Initialise the LoopbackSoftware object
    myLoopSerial.begin();
}

void loop() {
    // Send a byte with the value 45
    myLoopSerial.write(45);
}
```

### Also Implemented:
- end() - deinitialises the FIFO buffer
- flush() - resets the FIFO buffer
