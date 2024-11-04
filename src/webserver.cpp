#include "webserver.h"

String readFile(const char* path) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        Serial.println("Failed to open file: " + String(path));
        return String();
    }
    String content = file.readString();
    file.close();
    return content;
}

void setupWiFi() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Initialize LED to OFF state
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) 
    {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted successfully");
    Serial.println("Listing files:");
    listDir(SPIFFS, "/");
    
    // Start the Access Point with the correct SSID
    WiFi.softAP(ssid, password);
    Serial.println("Access Point Started");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("PWD: ");
    Serial.println(password);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    
    // Start the server
    server.begin();
    Serial.println("Server started");
}

void listDir(fs::FS &fs, const char *dirname) 
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root || !root.isDirectory()) {
    Serial.println("Failed to open directory or not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("DIR : ");
      Serial.println(file.name());
    } else {
      Serial.print("FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void handleClientRequests() {
    WiFiClient client = server.available();
    
    if (client) {
        Serial.println("New Client Connected");
        String currentLine = "";
        String requestLine = "";
        bool requestLineComplete = false;
        
        unsigned long currentTime = millis();
        unsigned long previousTime = currentTime;
        const long timeoutTime = 2000;
        
        while (client.connected() && currentTime - previousTime <= timeoutTime) {
            currentTime = millis();
            
            if (client.available()) {
                char c = client.read();
                
                // Store the first line of the request
                if (!requestLineComplete) {
                    if (c != '\n' && c != '\r') {
                        requestLine += c;
                    } else if (c == '\n') {
                        requestLineComplete = true;
                    }
                }
                
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        // Process the stored request line
                        Serial.println("Request: " + requestLine);
                        
                        if (requestLine.indexOf("GET /style.css") >= 0) {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/css");
                            client.println("Connection: close");
                            client.println();
                            client.print(readFile("/style.css"));
                        }
                        else if (requestLine.indexOf("GET /script.js") >= 0) {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:application/javascript");
                            client.println("Connection: close");
                            client.println();
                            client.print(readFile("/script.js"));
                        }
                        else if (requestLine.indexOf("GET /LED_on") >= 0) {
                            digitalWrite(LED_PIN, HIGH);
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:application/json");
                            client.println("Connection: close");
                            client.println();
                            client.println("{\"status\":\"on\"}");
                        }
                        else if (requestLine.indexOf("GET /LED_off") >= 0) {
                            digitalWrite(LED_PIN, LOW);
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:application/json");
                            client.println("Connection: close");
                            client.println();
                            client.println("{\"status\":\"off\"}");
                        }
                        else {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();
                            client.print(readFile("/index.html"));
                        }
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        
        client.stop();
        Serial.println("Client disconnected");
    }
}