#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>
/////////////////////////////// generic /////////////////////////////
#define LED_PIN 2
int blinks = 9;

void blinkLed(int on_time) {
  digitalWrite(LED_PIN, HIGH);
  delay(on_time);
  digitalWrite(LED_PIN, LOW);
}

//////////////////////keypad/////////////////////////////////////////
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 27, 26, 25, 33 };  // Replace GPIO_X, GPIO_Y, GPIO_Z, GPIO_W with your chosen GPIO pins
byte colPins[COLS] = { 14, 12, 13 };      // Replace GPIO_A, GPIO_B, GPIO_C with your chosen GPIO pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//////////////////////////espnow/////////////////////////////////////
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

esp_err_t sendStringMessage(uint8_t macAddress[], String message) {
  esp_err_t result = esp_now_send(macAddress, (uint8_t *)message.c_str(), message.length());
  Serial.print("sent: [" + message + "] ");
  if (result == ESP_OK) {
    Serial.println("with success");
  } else {
    Serial.println("with Error");
  }
  return result;
}

/////////////////////////////// setup ///////////////////////////////////////////////////
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  initEspNow();
  esp_now_register_send_cb(OnDataSent);
  registerPear(receiverAddress_1);
}

////////////////////////// loop /////////////////////////////////////////////////////////
void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
    String message = "blink_" + String(blinks);
    sendStringMessage(receiverAddress_1, message);
  }
}