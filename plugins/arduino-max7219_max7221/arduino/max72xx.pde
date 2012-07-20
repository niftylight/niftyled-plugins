/*
 * libniftyled - Interface library for LED interfaces
 * Copyright (C) 2010 Daniel Hiepler <daniel@niftylight.de>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


/** 
 * Arduino project to interface MAX7219/7221 with niftyled 
 * arduino-max72xx plugin 
 *
 * This is is based on the LedControl library from Eberhard Fahle 
 */

#include <stdarg.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif



/** pin definitions */
#define PIN_DATA	10
#define PIN_CLOCK	12
#define PIN_CS		11

#define DEBUG 

/******************************************************************************/



/** opcodes for the MAX7221 and MAX7219 */
enum
{
  OP_NOOP = 0,
  OP_DIGIT0,
  OP_DIGIT1,
  OP_DIGIT2,
  OP_DIGIT3,
  OP_DIGIT4,
  OP_DIGIT5,
  OP_DIGIT6,
  OP_DIGIT7,
  OP_DECODEMODE,
  OP_INTENSITY,
  OP_SCANLIMIT,
  OP_SHUTDOWN,
  OP_RESERVED1,
  OP_RESERVED2,
  OP_DISPLAYTEST,
};



/** some private data */
struct priv
{
  /* amount of devices configured (0-8) */
  char n_devices;
  /* Data is shifted out of this pin */
  int SPI_MOSI;
  /* The clock is signaled on this pin */
  int SPI_CLK;
  /* This one is driven LOW for chip selectzion */
  int SPI_CS;
  /* The array for shifting the data to the devices */
  char spidata[16];
  /* We keep track of the led-status for all 8 devices in this array */
  char status[64];
}
priv;



#ifdef DEBUG
void LOG(char *fmt, ... )
{
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.println(tmp);
}
#else
void LOG(char *fmt, ...) {
}
#endif


/** send data+opcode to MAX72xx chain */
void spiTransfer(char addr, volatile char opcode, volatile char data) 
{
  if(addr < 0 || addr >= priv.n_devices)
    return;

  /* send data last-byte first, start at offset */
  int offset = (int) addr*2;

  /* clear buffer */
  for(char i=0; i < priv.n_devices*2; i++)
    priv.spidata[i]=0;

  /* put our device data into the array */
  priv.spidata[offset+1] = opcode;
  priv.spidata[offset] = data;

  /* enable the line */
  digitalWrite(priv.SPI_CS, LOW);

  /* Now shift out the data */
  for(char i = priv.n_devices*2; i>0; i--)
  {
        //LOG("0x%x", priv.spidata[i-1]);
        shiftOut(priv.SPI_MOSI, priv.SPI_CLK, MSBFIRST, priv.spidata[i-1]);
  }

  /* latch the data onto the display */
  digitalWrite(priv.SPI_CS,HIGH);
}   


/** switch chip into shutdown mode */
void shutdown(char addr, bool b) 
{
  b ? LOG("Waking up chip %d", addr) : LOG("Shutting down chip %d", addr);

  if(addr < 0 || addr >= priv.n_devices)
  {
    LOG("Illegal address: %d", addr);
    return;
  }

  spiTransfer(addr, OP_SHUTDOWN, b ? 0 : 1);
}


/** set chip to this scan limit (0-8) */
void setScanLimit(char addr, char limit) 
{
  if(addr < 0 || addr >= priv.n_devices)
  {
    LOG("Illegal address: %d", addr);
    return;
  }

  LOG("Setting scanLimit to %d", limit);

  if(limit>=0 || limit<8)
    spiTransfer(addr, OP_SCANLIMIT,limit);
}


/** set intensity/brightness/gain of this chip (0-16) */
void setIntensity(char addr, char intensity) 
{
  if(addr < 0 || addr >= priv.n_devices)
  {
    LOG("Illegal address: %d",addr);
    return;
  }

  LOG("Setting intensity to %d",intensity);

  if(intensity>=0 || intensity<16)	
    spiTransfer(addr, OP_INTENSITY,intensity);
}


/** upload buffer to display */
void showBuffer()
{
  LOG("Showing buffer");

  char chip, row;
  for(chip=0; chip < priv.n_devices; chip++)
  {
    for(row=0; row < 8; row++)
    {
      spiTransfer(chip, row+1, priv.status[chip*8+row]);
    }
  }
}


/** initialize one MAX72xx */
void chipInit(int i)
{
  spiTransfer(i,OP_DISPLAYTEST,0);
  /* scanlimit is set to max on startup */
  setScanLimit(i,7);
  /* set intensity to lowest on startup */
  setIntensity(i, 8);
  /* decode is done in source */
  spiTransfer(i,OP_DECODEMODE,0);
  /* wake up chip */
  shutdown(i,false); 
}


/** OPCODES to control our arduino via USB */
enum
{
  /** set intensity of LEDs */
  LED_SET_GAIN = 0,
  /** set amount of connected chips */
  LED_SET_CHIPCOUNT,
  /** set scanlimit */
  LED_SET_SCANLIMIT,
  /** send previously received data to LEDs */
  LED_LATCH,
  /** receive pixel data */
  LED_UPLOAD,
};


/*****************************************************************************/

/** receive a packet from USB host */
void rxPacket(char opcode, char ssize)
{

  LOG("Got packet, opcode: %d size: %d", opcode, ssize);

  char tmp[64];	
  if(ssize > 0)
    Serial.readBytes(tmp, ssize < 64 ? ssize : 64);

  /* handle various packet types */
  switch(opcode)
  {
  case LED_SET_GAIN:
    {			
      char addr = tmp[0];
      char intensity = tmp[1];

      LOG("LED_SET_GAIN(%d,%d)", addr, intensity);

      setIntensity(addr, intensity);
      break;
    }

  case LED_SET_CHIPCOUNT:
    {
      char chipcount = tmp[0];
      if(chipcount < 0 || chipcount >= 8)
        return;

      LOG("LED_SET_CHIPCOUNT(%d)", chipcount);

      priv.n_devices = chipcount;

      /* re-initialize new amount of chips */
      for(int i=0; i < priv.n_devices; i++)
        chipInit(i);
      break;
    }

  case LED_SET_SCANLIMIT:
    {			
      char addr = tmp[0];
      char limit = tmp[1];

      LOG("LED_SET_SCANLIMIT(%d,%d)", addr, limit);

      setScanLimit(addr, limit);
      break;
    }

  case LED_LATCH:
    {
      LOG("LED_LATCH");

      showBuffer();
      break;
    }

  case LED_UPLOAD:
    {		
      LOG("LED_UPLOAD");

      char i;
      for(i=0; i < ssize; i++)
      {
        priv.status[i] = tmp[i];
      }

      break;
    }

    /* huh? */
  default:
    {
      return;
    }
  }

  /* throw away remaining data if we received too much */
  if(ssize > 64)
  {
    int i;
    for(i=0; i < ssize-64; i++)
      Serial.read();
  }
}


/*****************************************************************************/

/** arduino setup function (called once after reset) */
void setup() 
{
  /* we start with 8 chips */
  priv.n_devices = 8;

  /* initialize SPI */
  priv.SPI_MOSI = PIN_DATA;
  priv.SPI_CLK  = PIN_CLOCK;
  priv.SPI_CS   = PIN_CS;
  pinMode(priv.SPI_MOSI, OUTPUT);
  pinMode(priv.SPI_CLK,  OUTPUT);
  pinMode(priv.SPI_CS,   OUTPUT);
  digitalWrite(priv.SPI_CS,HIGH);

  /* clear our buffer */
  int i;
  for(i=0; i < sizeof(priv.status); i++) 
    priv.status[i] = 0x00;

  for(i=0; i < priv.n_devices; i++) 
  {
    chipInit(i);
  }

  /* initialize serial communication */
  Serial.begin(115200);

  LOG("Initialized...");
}

/**
 * Test packets:
 * 0x4 0x8 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 
 * 0x3 0x0
 * 
 * 0x1 0x1 0x1 0x4 0x8 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0x3 0x0
 * 
 * Gives:
 * 0xffff 0x2 0x0 0x3 0xffff 0x4 0x0 0x5 0xffff 0x6 0x0 0x7 0xffff 0x8 0x0
 */

/** arduino loop function (called repeatedly)*/
void loop()
{
  char opcode, size;

  /* wait for 2 bytes at least */
  if(Serial.available() < 2)
    return;

  opcode = Serial.read();
  size = Serial.read();

  rxPacket(opcode, size);
}

