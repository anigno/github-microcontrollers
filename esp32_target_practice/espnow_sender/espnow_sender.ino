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
  { 0xA0, 0xA3, 0xB3, 0xFE, 0xED, 0x70 },  //A0:A3:B3:FE:ED:70
  { 0xE4, 0x65, 0xB8, 0x77, 0x08, 0x64 },  //E4:65:B8:77:08:64
  { 0xd8, 0x13, 0x2A, 0x43, 0x27, 0x50 },  //D8:13:2A:43:27:50
  { 0x08, 0xD1, 0xF9, 0x3B, 0x38, 0x24 },  //08:D1:F9:3B:38:24
  { 0xE4, 0x65, 0xB8, 0x83, 0xEC, 0xF6 }
};

void blinkLED_Quick(int numBlinks) {
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(25);  // Adjust blink duration as needed
    digitalWrite(LED_PIN, LOW);
    delay(75);  // Adjust blink duration as needed
  }
}


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
  Serial.begin(115200);
  initEspNow();
  esp_now_register_send_cb(OnDataSent);
  for (int i = 0; i < 6; i++) {
    registerPear(receiverAddress[i]);
  }
  unsigned int seed = millis();  // Example: Use millis() as the seed
  esp_random();
  pinMode(LED_PIN, OUTPUT);
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
  blinkLED_Quick(1);
}