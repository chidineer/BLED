#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <SPIFFS.h>

// Function declarations
void setupWiFi();
void handleClientRequests();
String readFile(const char* path);
void listDir(fs::FS &fs, const char *dirname);

// Constants
extern const char* ssid;
extern const char* password;
extern const int LED_PIN;
extern WiFiServer server;

#endif