#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

// Home WiFi credentials
const char* ssid = "anigno-tenda";
const char* password = "136161271";
// Create a web server object
WebServer server(80);

// Status variables
bool activityActive = false;
bool alertActive = false;

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Control</title>
  <style>
    body { font-family: Arial, sans-serif; }
    .button { padding: 15px 25px; font-size: 24px; margin: 10px; cursor: pointer; }
    .status { font-size: 18px; margin: 10px; }
  </style>
</head>
<body>
  <h1>ESP32 Web Server</h1>
  <table>
        <tr>
          <td><button class="button" onclick="sendRequest('/toggleActivity')">Toggle Activity</button></td>
        </tr>
        <tr>
          <td><button class="button" onclick="sendRequest('/toggleAlert')">Toggle Alert</button></td>
        </tr>
        <tr>
          <td><button class="button" onclick="sendRequest('/action1')">Action 1</button></td>
        </tr>
        <tr>
          <td><button class="button" onclick="sendRequest('/action2')">Action 2</button></td>
        </tr>
        <tr>
          <td><div class="status" id="activityStatus">Activity Status: Unknown</div></td>
        </tr>
        <tr>
          <td><div class="status" id="alertStatus">Alert Status: Unknown</div></td>
        </tr>
      </table>
 
  <script>
    function sendRequest(path) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", path, true);
      xhr.send();
    }

    function updateStatus() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/status", true);
      xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
          var status = JSON.parse(xhr.responseText);
          document.getElementById("activityStatus").innerHTML = "Activity Status: " + (status.activityActive ? "Active" : "Inactive");
          document.getElementById("alertStatus").innerHTML = "Alert Status: " + (status.alertActive ? "Active" : "Inactive");
        }
      };
      xhr.send();
    }

    setInterval(updateStatus, 1000); // Update status every second
    window.onload = updateStatus; // Update status when page loads
  </script>
</body>
</html>
)rawliteral";


void sendTelegramMessage(const char* message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("https://api.telegram.org/bot7101736617:AAG_SgYUf_kvl_B01ExOz6pWRBOQdaca4dw/sendMessage?chat_id=1807248297&text=") + message;
    http.begin(url);
    int httpResponseCode = http.GET();
    http.end();
    Serial.printf("Telegram Message sent, code: %d\n", httpResponseCode);
  } else {
    Serial.println("WiFi not connected");
  }
}


void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleToggleActivity() {
  activityActive = !activityActive;
  server.send(200, "text/plain", "Activity Toggled");
}

void handleToggleAlert() {
  alertActive = !alertActive;
  server.send(200, "text/plain", "Alert Toggled");
}

void handleAction1() {
  // Your action 1 code here
  sendTelegramMessage("Hello%20World_alert_notification");
  server.send(200, "text/plain", "Action 1 Executed");
}

void handleAction2() {
  // Your action 2 code here
  server.send(200, "text/plain", "Action 2 Executed");
}

void handleStatus() {
  String json = "{\"activityActive\":" + String(activityActive ? "true" : "false") + ",\"alertActive\":" + String(alertActive ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setup() {
  Serial.begin(115200);

  connectToWiFi();

  server.on("/", handleRoot);
  server.on("/toggleActivity", handleToggleActivity);
  server.on("/toggleAlert", handleToggleAlert);
  server.on("/action1", handleAction1);
  server.on("/action2", handleAction2);
  server.on("/status", handleStatus);
  server.begin();

  Serial.println("Server started");

  if (!MDNS.begin("esp32")) {
    Serial.println("Error starting mDNS");
  }
  // Print the IP address
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    connectToWiFi();
  }
  server.handleClient();
}
