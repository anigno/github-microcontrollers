#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>
#include "espnow_helper.h"
#include "keypad_def.h"

#define LED_PIN 2
int blinks_to_send = 4;
int randomSeedCounter = 0;
int lastRandomNumber = -1;
//receivers esp32 mac addresses
uint8_t receiverAddress[][6] = {
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 },
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 },
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 },
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 },
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 },
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF4 }
};

int getNewRandomNumber() {
  int newNumber = esp_random() % 6;
  do {
    newNumber = esp_random() % 6;
  } while (newNumber == lastRandomNumber);
  lastRandomNumber = newNumber;
  return newNumber;
}

/////////////////////////////// setup ///////////////////////////////////////////////////
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  initEspNow();
  esp_now_register_send_cb(OnDataSent);
  for (int i = 0; i < 6; i++) {
    registerPear(receiverAddress[i]);
  }
  unsigned int seed = millis();  // Example: Use millis() as the seed
  esp_random();
}

////////////////////////// loop /////////////////////////////////////////////////////////
void loop() {
  char keyChar = keypad.getKey();
  if (keyChar != NO_KEY) {
    Serial.println(keyChar);
    int intDigit = keyChar - '0';
    uint8_t *selectedReceiverAddress;
    if (intDigit >= 1 && intDigit <= 6) {
      int receiver_index = intDigit - 1;
      Serial.println("send to receiver: " + String(receiver_index));
      selectedReceiverAddress = receiverAddress[receiver_index];
    }
    if (keyChar == '*') {
      int randomValue = getNewRandomNumber();
      Serial.println("send to receiver: " + String(randomValue));
      selectedReceiverAddress = receiverAddress[randomValue];
    }
    String message = "blink_" + String(blinks_to_send);
    sendStringMessage(selectedReceiverAddress, message);
  }
}