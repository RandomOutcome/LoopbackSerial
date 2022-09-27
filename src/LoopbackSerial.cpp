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
#include <SoftwareSerial.h>


//
// Constructors
//

LoopbackSerial::LoopbackSerial()  // default
{
  // does nothing explicit
}


LoopbackSerial::LoopbackSerial() : 
  _buffer_overflow(false)
{
  // does nothing explicit
}


//
// Destructor
//
LoopbackSerial::~LoopbackSerial()
{
  end();
}


//
// Public methods
//

void LoopbackSerial::begin()
{
  // initialise buffer offsets
  _receive_buffer_head = _receive_buffer_tail = 0;
}


void LoopbackSerial::end()
{
  // initialise buffer offsets
  _receive_buffer_head = _receive_buffer_tail = 0;
}


bool LoopbackSerial::listen()
{
  return true;
}


int LoopbackSerial::available()
{
  // check LoopbackSerial instance is active
  if (!isListening())
    return 0;

  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;  
}


int LoopbackSerial::read()
{
  // check LoopbackSerial instance is active
  if ( !isListening() )
    return -1;

  // check data is present
  if ( _receive_buffer_head == _receive_buffer_tail )
    return -1;

  // read data from the buffer (ordered as read from the head & write to the tail)
  uint8_t data = _receive_buffer[_receive_buffer_head]; 

  // increament buffer head offset
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;  

  return data;
}


int LoopbackSerial::peek()
{
  // check LoopbackSerial instance is active
  if (!isListening())
    return -1;

  // check if buffer is empty
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}


size_t LoopbackSerial::write(uint8_t data)
{
  // check LoopbackSerial instance is active
  if ( !isListening() )
    return 0;

  // if buffer full, set the overflow flag and return
  if ( (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF == _receive_buffer_head ) 
  {
    _buffer_overflow = true;
    return 0;
  }

  // add data to buffer (ordered as read from the head & write to the tail)
  _receive_buffer[_receive_buffer_tail] = data; 

  // increament buffer tail offset
  _receive_buffer_tail = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
  
  return 1; // bytes written
}


void LoopbackSerial::flush()
{
  if (!isListening())
    return;

  _receive_buffer_head = _receive_buffer_tail = 0;
}


// --- End of Code ---
