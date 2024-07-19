#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); // Bluetooth device name
}

void loop() {
  if (SerialBT.available()) {
    String receivedData = SerialBT.readString();
    Serial.println("Received: " + receivedData);
    // Process received data here
    
    // Example: send a response back to the phone
    String response = "Response to: " + receivedData;
    SerialBT.println(response);
    Serial.println("Sent: " + response);
  }
  
  delay(100);
}
