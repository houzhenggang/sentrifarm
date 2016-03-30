/*
  Copyright (c) 2016 Andrew McDonnell <bugs@andrewmcdonnell.net>

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

#ifndef SENTRIFARM_WEB_H__
#define SENTRIFARM_WEB_H__

#include <memory>

class ESP8266WebServer;

namespace sentrifarm {

// GET http://gateway        <-- status page
// GET http://gateway/reset  <-- API call: drop wifi configuration, reset back into AP + captive portal

class WebServer
{
public:
  void begin();
  void handle();

  void setStatus(const String& statusMsg) { statusMsg_ = statusMsg; }

private:
  std::unique_ptr<ESP8266WebServer> webServer_;

  void handleRoot();
  void handleReset();
  void handleNotFound();

  String statusMsg_;
};

}

#endif
