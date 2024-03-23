#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2
uint8_t receiverMacAddress1[] = { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 };  // Replace with your receiver's MAC address
int cnt = 0;
//E4:65:B8:83:EC:F4
void registerPeer(uint8_t *macAddress) {
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, macAddress, 6);
  peerInfo.channel = 0;  // Use channel 0
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
}

void sendMessage(uint8_t *macAddress, String message) {
  esp_now_send(macAddress, (uint8_t *)message.c_str(), message.length());
  Serial.print("sent ");
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  registerPeer(receiverMacAddress1);
}

void loop() {
  cnt = (cnt + 1) % 2;
  digitalWrite(LED_PIN, cnt);
  Serial.println(cnt);
  sendMessage(receiverMacAddress1, "blink_3");  // Change the blink count as needed
  delay(2000);
}
