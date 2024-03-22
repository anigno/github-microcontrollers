esp32 wroom 32 led pin=2
mac01: 08:D1:F9:3B:38:24
mac02: E4:65:B8:83:EC:F4

//get mac address
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  sleep(1000);
}
