#include "webserver.h"

const char* ssid = "BLED";
const char* password = "bled12345";
const int LED_PIN = 2;
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    setupWiFi();
}

void loop() 
{
    handleClientRequests();
}