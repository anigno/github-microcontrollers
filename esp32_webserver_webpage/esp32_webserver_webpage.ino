#include <WiFi.h>
#include <WebServer.h>

//ADC1_CH0
#define BATTERY_PIN 32

const char* ssid = "anigno-tenda";
const char* password = "136161271";

WebServer server(80);

IPAddress staticIP(192, 168, 1, 96);  // Set your desired static IP address
IPAddress gateway(192, 168, 1, 1);    // Set your gateway IP address
IPAddress subnet(255, 255, 255, 0);   // Set your subnet mask

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectToWiFi();

  // Define routes
  server.on("/", HTTP_GET, []() {
    float batteryVoltage = analogRead(BATTERY_PIN); //(0 - 4095) 
    float batteryPercentage = (batteryVoltage / 4095.0 * 3.3) * (4.2 / 3.3)*100;
    // Serve the HTML page
    server.send(200, "text/html", generateHTML(batteryPercentage));
  });

  server.on("/button1", HTTP_GET, []() {
    // Handle button 1 press
    String textValue = server.arg("textValue");
    Serial.print("Button 1 pressed with text: ");
    Serial.println(textValue);
    server.send(200, "text/plain", "Button 1 pressed with text: " + textValue);
  });

  server.on("/button2", HTTP_GET, []() {
    // Handle button 2 press
    String textValue = server.arg("textValue");
    Serial.print("Button 2 pressed with text: ");
    Serial.println(textValue);
    server.send(200, "text/plain", "Button 2 pressed with text: " + textValue);
  });

  server.on("/button3", HTTP_GET, []() {
    // Handle button 3 press
    String textValue = server.arg("textValue");
    Serial.print("Button 3 pressed with text: ");
    Serial.println(textValue);
    server.send(200, "text/plain", "Button 3 pressed with text: " + textValue);
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");

  // Register WiFi events handler
  WiFi.onEvent(WiFiEvent);
  pinMode(BATTERY_PIN, INPUT);
}

void loop() {
  server.handleClient();
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet);
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

String generateHTML(float batteryPrecentage) {
  String html = "<!DOCTYPE html><html><head><title>ESP32 Web Server</title>";
  html += "<style>";
  html += "body { font-size: 20px; }"; // Enlarge text size
  html += "button { font-size: 20px; padding: 10px 20px; }"; // Enlarge button size and add padding
  html += "</style>";
  html += "</head><body>";
  html += "<h1>ESP32 Web Server</h1>";
  html += "<p>Battery voltage: " + String(batteryPrecentage) + "%</p>";
  html += "<input type=\"text\" id=\"textbox\"><br>";
  html += "<button onclick=\"button1()\">Button 1</button>";
  html += "<button onclick=\"button2()\">Button 2</button>";
  html += "<button onclick=\"button3()\">Button 3</button><br>";
  html += "<script>";
  html += "function button1() {";
  html += "  var textValue = document.getElementById('textbox').value;";
  html += "  fetch('/button1?textValue=' + textValue);";
  html += "}";
  html += "function button2() {";
  html += "  var textValue = document.getElementById('textbox').value;";
  html += "  fetch('/button2?textValue=' + textValue);";
  html += "}";
  html += "function button3() {";
  html += "  var textValue = document.getElementById('textbox').value;";
  html += "  fetch('/button3?textValue=' + textValue);";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  return html;
}

