#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define LED_PIN 2 //pin config to connect LED/Relay

ESP8266WebServer server(80);

String ssid, password;

// Replace with your Sinric Pro credentials
#define APP_KEY     "YourAPIKey"
#define APP_SECRET  "YourAppSecret"
#define SWITCH_ID   "YourDeviceID"

bool isConnectedToWiFi = false;

void saveCredentials(const String &ssid, const String &password) {
  File f = SPIFFS.open("/wifi.json", "w");
  if (!f) return;
  StaticJsonDocument<256> doc;
  doc["ssid"] = ssid;
  doc["password"] = password;
  serializeJson(doc, f);
  f.close();
}

bool loadCredentials(String &ssid, String &password) {
  if (!SPIFFS.exists("/wifi.json")) return false;
  File f = SPIFFS.open("/wifi.json", "r");
  if (!f) return false;
  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, f) != DeserializationError::Ok) return false;
  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();
  return true;
}

void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP_Config"); //Wi-Fi name during AP mode(it will be open network)
  Serial.print("[WiFi] AP Mode IP: ");
  Serial.println(WiFi.softAPIP());
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("[WiFi] Connecting");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WiFi] Connected to " + ssid);
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    isConnectedToWiFi = true;
  } else {
    Serial.println("[WiFi] Failed to connect. Starting AP mode.");
    startAPMode();
  }
}

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleConnect() {
  ssid = server.arg("ssid");
  password = server.arg("password");
  if (ssid.length() == 0 || password.length() == 0) {
    String body = server.arg("plain");
    StaticJsonDocument<256> doc;
    deserializeJson(doc, body);
    ssid = doc["ssid"].as<String>();
    password = doc["password"].as<String>();
  }

  if (ssid.length() && password.length()) {
    saveCredentials(ssid, password);
    server.send(200, "text/plain", "Saved. Restarting...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "SSID or Password missing");
  }
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);

    // Serve static files from SPIFFS (like .js, .css, .html, etc.)
  server.serveStatic("/", SPIFFS, "/");
  
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });
  server.begin();
  Serial.println("[Web] Web server started");
}

// SinricPro Callback
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("[SinricPro] Device %s turned %s\n", deviceId.c_str(), state ? "ON" : "OFF");
  digitalWrite(LED_PIN, state ? LOW : HIGH); // LED logic is inverted
  return true;
}

void setupSinricPro() {
  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);

  SinricPro.onConnected([]() { Serial.println("[SinricPro] Connected"); });
  SinricPro.onDisconnected([]() { Serial.println("[SinricPro] Disconnected"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // off by default (inverted logic)

  if (!SPIFFS.begin()) {
    Serial.println("[FS] SPIFFS Mount Failed");
    return;
  }

  if (loadCredentials(ssid, password)) {
    connectToWiFi();
  } else {
    startAPMode();
  }

  setupServer();

  if (isConnectedToWiFi) {
    setupSinricPro();
  }
}

void loop() {
  server.handleClient();
  if (isConnectedToWiFi) {
    SinricPro.handle();
  }
}
