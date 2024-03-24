#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2  // Pin connected to LED

void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.println("Message received");

  String receivedString = "";
  for (int i = 0; i < data_len; i++) {
    receivedString += (char)data[i];
  }
  Serial.println(receivedString);
  digitalWrite(LED_PIN, HIGH);  // Turn on LED
  delay(250);                   // Keep LED on for 1 second
  digitalWrite(LED_PIN, LOW);   // Turn off LED
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
int cnt = 0;
void loop() {
  // Do other stuff if needed
  Serial.println(cnt++);
  delay(1000);
}
