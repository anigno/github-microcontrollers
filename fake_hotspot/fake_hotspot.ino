#include <WiFi.h>

const char* ssid1 = "FakeHotspot1";  // SSID for the first fake hotspot
const char* ssid2 = "FakeHotspot2";  // SSID for the second fake hotspot
const char* password = "password";   // Password for the fake hotspots

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Set up the first fake Wi-Fi hotspot
  WiFi.softAP(ssid1, password);
  Serial.println("Fake Wi-Fi hotspot 1 created");

  // Set up the second fake Wi-Fi hotspot
  WiFi.softAP(ssid2, password);
  Serial.println("Fake Wi-Fi hotspot 2 created");
}

void loop() {
  // Your code here
}
