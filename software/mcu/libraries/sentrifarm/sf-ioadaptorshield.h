/*
  Copyright (c) 2015 Andrew McDonnell <bugs@andrewmcdonnell.net>

  This file is part of SentriFarm Radio Relay.

  SentriFarm Radio Relay is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SentriFarm Radio Relay is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SentriFarm Radio Relay.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SENTRIFARM_IOADAPTORSHIELD_H__
#define SENTRIFARM_IOADAPTORSHIELD_H__

// Supports the following configurations:
//
// (1) ESP8266, using sentrifarm-shield-adaptor
//     GPIO12 = MISO (HSPI)
//     GPIO13 = MOSI (HSPI)
//     GPIO14 = SCK (HSPI)
//     GPIO15 = CS
//     GPIO0  = RST
//     GPIO2  = LED4 (inverted)
//     GPIO5  = SDA (note, it is supposed to be 4 but swapped on v0 hardware)
//     GPIO4  = SCL (note, it is supposed to be 5 but swapped on v0 hardware)
//
// (2) TEENSY LC, using sentrifarm-shield-adaptor
//     MOSI,11
//     MISO,12
//     SCK,14
//     CS,10
//     RST,21
//     LED4,5
//     I2C 18, 19

#if defined(ESP8266)

// Pivot - we dont have a spare I/O for the DHT on the ESP
//       - (we should learn how to retask serial RX whilst keeping TX for debug)
//       - but we haven't had the odd hangs like the teensy to debug either
#define DISABLE_LED4
//#define PIN_LED4         2

#define PIN_DHT          2

#define PIN_SX1276_RST   0
#define PIN_SX1276_CS   15
#define PIN_SX1276_MISO 12
#define PIN_SX1276_MOSI 13
#define PIN_SX1276_SCK  14

// Note this is backwards to the default (doh)
#define PIN_SDA          5
#define PIN_SCL          4

#elif defined(TEENSYDUINO)

#define PIN_SDA          18
#define PIN_SCL          19

#define PIN_DHT          17

// Note: the LED on the teensy board is inverted relative to us (doh)
#define PIN_LED4         5
#define PIN_LED4T        13

#define PIN_SX1276_RST  21
#define PIN_SX1276_CS   10
#define PIN_SX1276_MISO 12
#define PIN_SX1276_MOSI 11
#define PIN_SX1276_SCK  14

// I2C is using the teensy default for I2c (18,19) on the adaptor shield
// So we dont need to specifically setup the pins

#endif

/// Notional reference voltage on PCF8591 Vref pin - millivolts
/// One option could be to _sample_ this with the built in ADC on the ESP-201 or teensy-lc
#define PCF8591_VREF 2490

#define PCF8591_I2C_ADDR (0x90 >> 1)

#define D1307_I2C_ADDR 0x68

#endif // SENTRIFARM_IOADAPTORSHIELD_H__
