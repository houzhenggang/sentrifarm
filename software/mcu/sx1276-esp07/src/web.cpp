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
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "web.h"
#include "globals.h"

inline void resetWifiSettingsAndReboot(bool justReboot = false)
{
  if (!justReboot) { WiFi.disconnect(); delay(500); }
  ESP.restart(); delay(500);
}

namespace sentrifarm {

const char HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

void WebServer::handleRoot()
{
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Gateway Status");
  page += FPSTR(HTTP_HEAD_END);
  page += "<h1>";
  page += "Gateway Status.";
  page += "</h1>";
  if (statusMsg_.length() > 0) {
    page += statusMsg_;
  } else {
    page += "OK";
  }
  page += "<br/>";
  page += FPSTR(HTTP_END);
  webServer_->send(200, "text/html", page);
}

void WebServer::handleReset()
{
  String page = "Reset OK. Search for AP SSID: ";
  page += FALLBACK_SSID;
  webServer_->send(200, "text/html", page);
  delay(500);

  Serial.println(F("USER RESET of WIFI DATA. Reboot..."));
  resetWifiSettingsAndReboot();
}

void WebServer::handleNotFound()
{
  webServer_->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer_->sendHeader("Pragma", "no-cache");
  webServer_->sendHeader("Expires", "-1");
  webServer_->send ( 404, "text/plain", "sorry");
}

void WebServer::begin()
{
  // Launch web server.
  // Note our IP address will be pseudo-random (by DHCP) unless
  // we somehow do something like mDNS or UPnP or something
  webServer_.reset(new ESP8266WebServer(80));
  webServer_->on("/", std::bind(&WebServer::handleRoot, this));
  webServer_->on("/reset", std::bind(&WebServer::handleReset, this));
  webServer_->onNotFound(std::bind(&WebServer::handleNotFound, this));
  webServer_->begin();
}

void WebServer::handle()
{
  webServer_->handleClient();
}

}
