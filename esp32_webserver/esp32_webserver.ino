#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectToWiFi();

  // Define routes
  server.on("/do_a", HTTP_GET, []() {
    handleAPIRequest("A");
  });

  server.on("/do_b", HTTP_GET, []() {
    handleAPIRequest("B");
  });

  server.on("/do_c", HTTP_GET, []() {
    handleAPIRequest("C");
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");

  // Register WiFi events handler
  WiFi.onEvent(WiFiEvent);
}

void loop() {
  server.handleClient();
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Disconnected from WiFi");
      // Attempt to reconnect
      connectToWiFi();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Got IP address");
      Serial.println(WiFi.localIP());
      break;
    default:
      break;
  }
}

void handleAPIRequest(String apiName) {
  // Parse string and integer parameters
  String message = server.arg("message");
  int number = server.arg("number").toInt();

  // Construct response
  String response = "API " + apiName + " response: ";
  response += "Message: " + message + ", Number: " + String(number);

  // Send response
  server.send(200, "text/plain", response);
}
