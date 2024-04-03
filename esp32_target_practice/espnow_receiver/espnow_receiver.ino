#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2  // Pin connected to LED
int cnt = 0;

void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

  String receivedString = "";
  for (int i = 0; i < data_len; i++) {
    receivedString += (char)data[i];
  }
  if (receivedString.startsWith("blink_")) {
    int x = receivedString.substring(6).toInt(); // Start from index 6 to skip "blink_"
    Serial.print("Received blink command. Value of x: ");
    Serial.println(x);
    blinkLED(x);
  } else {
    Serial.println("Invalid command received");
  }
}

void blinkLED(int numBlinks) {
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100); // Adjust blink duration as needed
    digitalWrite(LED_PIN, LOW);
    delay(200); // Adjust blink duration as needed
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function to handle received data
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
}
