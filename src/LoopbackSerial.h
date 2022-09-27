/*
LoopbackSerial.h
Copyright (C) 2022  RandomOutcome/github.com

Multi-instance loopback (memory based) serial library for Arduino

Based on Arduino SoftwareSerial library published interface.

Many thanks to all.

Original may be found atSoftwareSerial - https://www.arduino.cc

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

The latest version of this library can be found at: https://github.com/RandomOutcome/LoopbackSerial 

Contact email: Github.RandomOutcome@gmail.com
*/

#ifndef LoopbackSerial_h
#define LoopbackSerial_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _LS_MAX_BUFF 64

class LoopbackSerial : public Stream
{
private:
  // Private variables
  uint16_t _bufferOverflow:1;

  volatile char _buffer[_LS_MAX_BUFF];
  volatile uint8_t _bufferTail = 0;
  volatile uint8_t _bufferHead = 0;
  bool _strictMode = false;

  // Static Variables
  inline static LoopbackSerial *_activeObject = 0;

public:
  // public methods
  LoopbackSerial();
  LoopbackSerial(bool strictMode);
  LoopbackSerial(uint8_t receivePin, uint8_t transmitPin, bool inverseLogic); // convienince constructor for SoftwareSerial consistency 
  ~LoopbackSerial();

  void begin();
  void begin(uint32_t baud); // convienince constructor for SoftwareSerial consistency
  bool listen();
  void end();
  bool isListening() { return _strictMode ? _activeObject == this : true; }
  bool overflow() { bool retval = _bufferOverflow; _bufferOverflow = false; return retval; }
  
  int  peek();
  
  // virtual methods that classes must override
  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();

  // public operators 
  operator bool() { return true; }

  using Print::write;
};

#endif

// --- End of Header ---