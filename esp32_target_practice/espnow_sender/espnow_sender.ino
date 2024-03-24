#include <esp_now.h>
#include <WiFi.h>

uint8_t receiverAddress_1[] = { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 };
// uint8_t broadcastAddress2[] = {0x08, 0xD1, 0xF9, 0x3B, 0x38, 0x25};
esp_now_peer_info_t peerInfo;
int cnt = 0;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void initEspNow() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
}

void registerPear(uint8_t macAddress[]) {
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, macAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void setup() {
  Serial.begin(115200);
  initEspNow();
  esp_now_register_send_cb(OnDataSent);
  registerPear(receiverAddress_1);
}

esp_err_t sendStringMessage(uint8_t macAddress[], String message) {
  esp_err_t result = esp_now_send(macAddress, (uint8_t *)message.c_str(), message.length());
  Serial.print("sent: ["+message+"] ");
  if (result == ESP_OK) {
    Serial.println("with success");
  } else {
    Serial.println("Error");
  }
  return result;
}

void loop() {

  String message = "hello world";
  sendStringMessage(receiverAddress_1, message);

  delay(4000);
}