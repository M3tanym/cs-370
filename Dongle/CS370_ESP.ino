// Generic ESP8266, Default (lowercase Serial port)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define PORT 2342
#define BAUD 57600

WiFiUDP UDP;
WiFiClient client;

void connectWifi()
{
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    delay(500);
  }
}

void setup()
{
  Serial.begin(BAUD);
  Serial.println("Starting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "Password");

  connectWifi();

  UDP.begin(PORT);
  
  Serial.println("Running!");
}

void loop()
{
  if(millis() % 1000 == 0) // send a heartbeat packet every second
  {
    IPAddress r(192, 168, 1, 8); // IP adress of the controller-computer
    UDP.beginPacket(r, PORT);
    UDP.write('B'); // A random character to tell the controller that we're connected
    UDP.endPacket();
  }
  
  int packetSize = UDP.parsePacket();
  if(packetSize == 8) // ignore if not correct size
  {
    // Read the UDP packet and send it over the Serial lines
    unsigned char b[8];
    UDP.read(b, sizeof(b));
    
    Serial.print("!#(<"); // Header
    for(int i = 0; i < 8; i++)
      Serial.write(b[i]); // write = a byte, print = a string
    Serial.print(">)#!"); // Footer
  }
}
