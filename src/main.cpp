#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

#include "SystemConfig.h"

#include "page.html.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PSK;

const char *www_username = HTTP_USERNAME;
const char *www_password = HTTP_PASSWORD;

#include <WebServer.h>
WebServer server(80);

#include <ESP32Servo.h>
Servo servoRelease;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoReleasePin = 18;

void handleRoot()
{
  server.send(200, "text/html", mainPage);
}

void handleRelease()
{
  // if (!server.authenticate(www_username, www_password))
  // {
  //   return server.requestAuthentication();
  // }

  Serial.println("Release.");
  int sliderPos = constrain(server.arg("slider").toInt(), 0, 101);

  if (sliderPos == 100)
  {
    servoRelease.write(SERVO_OPEN);
  }

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleLock()
{
  Serial.println("Lock.");
  servoRelease.write(SERVO_LOCKED);

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Startup...");

  Serial.println("Startup: servo...");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servoRelease.setPeriodHertz(50);
  servoRelease.attach(servoReleasePin, 500, 2500);

  Serial.println("Startup: access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println("Startup: webserver...");
  server.on("/", handleRoot);
  server.on("/release", handleRelease);
  server.on("/lock", handleLock);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  Serial.println("Startup: OTA...");
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });
  ArduinoOTA.begin();

  Serial.println("Startup: done.");
}

void loop()
{
  server.handleClient();
  ArduinoOTA.handle();
}