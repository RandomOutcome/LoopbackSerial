/*
LoopbackSerial.cpp
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

#include <Arduino.h>
#include <LoopbackSerial.h>

// Constructor

LoopbackSerial::LoopbackSerial() : 
  _bufferOverflow(false),
  _strictMode(false)
{
}

LoopbackSerial::LoopbackSerial(bool strictMode) : 
  _bufferOverflow(false),
  _strictMode(strictMode)
{
}

LoopbackSerial::LoopbackSerial(uint8_t receivePin, uint8_t transmitPin, bool inverseLogic) :
  _bufferOverflow(false),
  _strictMode(true)
{
  // receivePin, transmitPin and inverseLogic are dummy parameters
  // implemented for convience for consistency with SoftwareSerial
  // constructor interface

  // this method asserts strict mode for handling listen/isListenting methods
  // to mirror the behaviour of SoftwareSerial
}


// Destructor

LoopbackSerial::~LoopbackSerial()
{
    end();
}


// Public methods

void LoopbackSerial::begin()
{
  // initialise buffer offsets and overflow status
  _bufferHead = _bufferTail = 0;
  _bufferOverflow = false;
}


void LoopbackSerial::begin(uint32_t baud)
{
  // baud is a dummy parameter - implemented for convience for consistency with SoftwareSerial
  begin();
}

void LoopbackSerial::end()
{
  // initialise buffer offsets and overflow status
  _bufferHead = _bufferTail = 0;
  if (_activeObject == this ) _activeObject = 0;
}


bool LoopbackSerial::listen()
{
  if (_activeObject != this) {

    if ( _strictMode ) {
      // reset buffer upon making active - mirros SoftwareSerial behaviour
      _activeObject = this;
      _bufferOverflow = false;
      _bufferHead = _bufferTail = 0;

      return true;
    }
  }

  return false;
}


int LoopbackSerial::available()
{
  // check LoopbackSerial instance is active
  if (!isListening())
    return 0;

  return (_bufferTail + _LS_MAX_BUFF - _bufferHead) % _LS_MAX_BUFF;  
}


int LoopbackSerial::read()
{
  // check LoopbackSerial instance is active
  if ( !isListening() )
    return -1;

  // check data is present
  if ( _bufferHead == _bufferTail )
    return -1;

  // read data from the buffer (ordered as read from the head & write to the tail)
  uint8_t data = _buffer[_bufferHead]; 

  // increament buffer head offset
  _bufferHead = (_bufferHead + 1) % _LS_MAX_BUFF;  

  return data;
}


int LoopbackSerial::peek()
{
  // check LoopbackSerial instance is active
  if (!isListening())
    return -1;

  // check if buffer is empty
  if (_bufferHead == _bufferTail)
    return -1;

  // Read from "head"
  return _buffer[_bufferHead];
}


size_t LoopbackSerial::write(uint8_t data)
{
  // check LoopbackSerial instance is active
  if ( !isListening() )
    return 0;

  // if buffer full, set the overflow flag and return
  if ( (_bufferTail + 1) % _LS_MAX_BUFF == _bufferHead ) 
  {
    _bufferOverflow = true;
    return 0;
  }

  // add data to buffer (ordered as read from the head & write to the tail)
  _buffer[_bufferTail] = data; 

  // increament buffer tail offset
  _bufferTail = (_bufferTail + 1) % _LS_MAX_BUFF;
  
  return 1; // one byte written
}


void LoopbackSerial::flush()
{
  if (!isListening())
    return;

  _bufferHead = _bufferTail = 0;
  _bufferOverflow = false;
}

// --- End of Code ---
