#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2
int cnt = 0;

void blink_n_times(int n) {
  pinMode(2, OUTPUT);
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);                   // wait for a second
    digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
    delay(200);
  }
}

//**receive espnow message. "blink_n", blink n times*/
void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Received message");
  String receivedString = "";
  for (int i = 0; i < data_len; i++) {
    receivedString += (char)data[i];
  }
  if (receivedString.startsWith("blink_")) {
    int numBlinks = receivedString.substring(6).toInt();
    Serial.print("Received blink command. Number of blinks: ");
    Serial.println(numBlinks);
    blink_n_times(numBlinks);
  }
}

/////////////////////////////////////////////////////////
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
  blink_n_times(3);
}
/////////////////////////////////////////////////////////
void loop() {
  // cnt = (cnt + 1) % 20;
  // digitalWrite(LED_PIN, cnt / 10);
  // delay(100);
}
