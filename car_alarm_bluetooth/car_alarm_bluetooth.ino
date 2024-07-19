#include <WiFi.h>
#include <BluetoothSerial.h>
#include <HTTPClient.h>

// Replace these with your network credentials
const char* ssid = "anigno-tenda";
const char* password = "136161271";


// Pins
const int pirPin = 13;
const int buzzerPin = 12;

// Create Bluetooth Serial object
BluetoothSerial SerialBT;

bool motionDetected = false;
bool alarmEnabled = true;

// Define the pairing code
const char* pairingCode = "1271";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Bluetooth
  SerialBT.begin("ESP32_Car_Alarm"); // Bluetooth device name
  SerialBT.enableSSP(); // Enable Secure Simple Pairing
  SerialBT.setPin(pairingCode); // Set the pairing code
  Serial.println("Bluetooth started with pairing code");

  // Connect to WiFi
  connectToWiFi();

  // Initialize PIR sensor
  pinMode(pirPin, INPUT);

  // Initialize Buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Initially off

  // Attach interrupt to PIR sensor
  attachInterrupt(digitalPinToInterrupt(pirPin), motionDetectedISR, RISING);
}

void loop() {
  // Check for Bluetooth commands
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    Serial.println("Received via BT: " + command);

    if (command == "11") {
      disableAlarm();
    } else if (command == "12") {
      enableAlarm();
    }else if (command == "13") {
      triggerAlarm();
    }
  }

  // If motion is detected and alarm is enabled, trigger the alarm
  if (motionDetected && alarmEnabled) {
    triggerAlarm();
    motionDetected = false; // Reset motion detected flag
  }

  // Check WiFi connection status and reconnect if necessary
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    connectToWiFi();
  }

  delay(100);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void motionDetectedISR() {
  motionDetected = true;
}

void triggerAlarm() {
  Serial.println("Motion detected! Triggering alarm.");

  // Activate buzzer
  digitalWrite(buzzerPin, HIGH);

  // Send HTTP request to Telegram bot
  sendTelegramMessage("Motion detected! Alarm triggered.");

  // Keep the buzzer on for a while
  delay(5000);
  digitalWrite(buzzerPin, LOW);
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("https://api.telegram.org/bot7101736617:AAG_SgYUf_kvl_B01ExOz6pWRBOQdaca4dw/sendMessage?chat_id=1807248297&text=Hello%20World_alert_notification");
    
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Telegram message sent successfully.");
      Serial.println(response);
    } else {
      Serial.print("Error sending message: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send message.");
  }
}

void disableAlarm() {
  detachInterrupt(digitalPinToInterrupt(pirPin));
  digitalWrite(buzzerPin, LOW);
  alarmEnabled = false;
  Serial.println("Alarm disabled.");
}

void enableAlarm() {
  attachInterrupt(digitalPinToInterrupt(pirPin), motionDetectedISR, RISING);
  alarmEnabled = true;
  Serial.println("Alarm enabled.");
}
