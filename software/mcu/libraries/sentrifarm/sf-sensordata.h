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

#ifndef SENTRIFARM_SENSOR_DATA_H__
#define SENTRIFARM_SENSOR_DATA_H__

#include "sf-util.h"

namespace Sentrifarm {

  /// Container for all local measurements.
  /// Note, this is NOT used for MQTT transmission as we will need to deal with float formats and other issues
  /// Not to mention sensor configuration and ontologies and other complications
  /// To keep it simple, we have support one of each sensor for now.
  /// At this stage for similar reasons no point making it to generic either
  struct SensorData
  {
    char sw_version[42]; ///< Git hash

    bool have_radio;
    int radio_version;   ///< SX1276 radio version

    bool have_bmp180;
    float ambient_hpa;   ///< BPM180 barometric air pressure, HPa
    float ambient_degc;  ///< BMP180 temperature, degC
    float altitude_m;    ///< BMP180 altitude. Recorded for interest rather than usefulness, as we dont expect a lot of accuracy

    bool have_pcf8591;
    byte adc_data0;      ///< PCF 8591 channel #0. For now, deciding _what_ this is is left up to the node functional logic
    byte adc_data1;
    byte adc_data2;
    byte adc_data3;

    void reset() {
      // Requires -DSF_GIT_VERSION to be set...
      strncpy(sw_version, STRINGIFYDEF(SF_GIT_VERSION), sizeof(sw_version));
      have_radio = false;
      have_bmp180 = false;
      have_pcf8591 = false;
    }

    void debug_dump() const {
      char buf[256];
      Serial.println(LINE_DOUBLE);

      Serial.println("Software tag %s", sw_version);

      if (have_radio) {
        snprintf((char*)buf, sizeof(buf), "SX1276 Version  %02x\n", radio_version);
        Serial.print(buf);
      } else { Serial.println(("Radio           NOT FOUND")); }

      if (have_bmp180) {
        snprintf((char*)buf, sizeof(buf), "Barometer       %3d.%d hPa\nAir temperature %3d.%d degC\nAltitude        %3d.%d\n",
                 (int)floorf(ambient_hpa), fraction(ambient_hpa),
                 (int)floorf(ambient_degc), fraction(ambient_degc),
                 (int)floorf(altitude_m), fraction(altitude_m));
        Serial.print(buf);
      } else { Serial.println(("Barometer       NOT FOUND")); }

      if (have_pcf8591) {
        int v0 = float(adc_data0) * PCF8591_VREF / 256.F;
        int v1 = float(adc_data1) * PCF8591_VREF / 256.F;
        int v2 = float(adc_data2) * PCF8591_VREF / 256.F;
        int v3 = float(adc_data3) * PCF8591_VREF / 256.F;
        snprintf((char*)buf, sizeof(buf), "PCF8591 Ch#0    %4dmV\n", v0); Serial.print(buf);
        snprintf((char*)buf, sizeof(buf), "PCF8591 Ch#1    %4dmV\n", v1); Serial.print(buf);
        snprintf((char*)buf, sizeof(buf), "PCF8591 Ch#2    %4dmV\n", v2); Serial.print(buf);
        snprintf((char*)buf, sizeof(buf), "PCF8591 Ch#3    %4dmV\n", v3); Serial.print(buf);
      } else { Serial.println(("PCF8591       NOT FOUND")); }

      Serial.println(LINE_DOUBLE);
    }

    // Make a catch all MQTT-SN message.
    // At this stage its not bery refined.
    void make_mqtt_0(char *buf, int len)
    {
      int v0 = float(adc_data0) * PCF8591_VREF / 256.F;
      int v1 = float(adc_data1) * PCF8591_VREF / 256.F;
      int v2 = float(adc_data2) * PCF8591_VREF / 256.F;
      int v3 = float(adc_data3) * PCF8591_VREF / 256.F;
      snprintf(buf, len, "%d%d%d,%d,%d.%d,%d.%d,%d,%d,%d,%d", have_radio, have_bmp180, have_pcf8591, radio_version,
               (int)floorf(ambient_hpa), fraction(ambient_hpa),
               (int)floorf(ambient_degc), fraction(ambient_degc),
               v0, v1, v2, v3);
    }

  };
}

#endif // SENTRIFARM_SENSOR_DATA_H__
