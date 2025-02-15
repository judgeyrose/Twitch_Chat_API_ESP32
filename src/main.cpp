#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h> // Include WiFiManager library
#include "LEDAnimations.h"
#include "config.h"

// Declare websocket client class variable
WebSocketsClient webSocket;

// Allocate the JSON document
StaticJsonDocument<200> doc;

// Parameters for Twitch channel
const char twitch_oauth_token[] = TWITCH_OAUTH_TOKEN;
const char twitch_nick[] = TWITCH_NICK;
const char twitch_channel[] = TWITCH_CHANNEL;

#define LED_PIN 5 // Update to a valid GPIO pin
#define NUM_LEDS 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
LEDAnimations ledAnimations(strip);

unsigned long lastWiFiCheckTime = 0;
const unsigned long wifiCheckInterval = 1000; // 1 minute
const unsigned long wifiReconnectInterval = 60000; // 1 minute

TaskHandle_t ledTaskHandle = NULL;

void setAnimation(String animation) {
  ledAnimations.setAnimation(animation);
}

void setColor(String color) {
  if (color == "red") {
    ledAnimations.setColor(255, 0, 0);
  } else if (color == "green") {
    ledAnimations.setColor(0, 255, 0);
  } else if (color == "blue") {
    ledAnimations.setColor(0, 0, 255);
  } else if (color == "yellow") {
    ledAnimations.setColor(255, 200, 0);
  } else if (color == "orange") {
    ledAnimations.setColor(255, 100, 0);
  } else if (color == "cyan") {
    ledAnimations.setColor(0, 255, 255);
  } else if (color == "purple") {
    ledAnimations.setColor(128, 0, 128);
  } else if (color == "white") {
    ledAnimations.setColor(255, 255, 255);
  } else if (color == "off") {
    ledAnimations.setColor(0, 0, 0);
    setAnimation("");
  }
}

void setBrightness(uint8_t value) {
  ledAnimations.setBrightness(value);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to: %s\n", payload);
      webSocket.sendTXT("PASS " + String(twitch_oauth_token) + "\r\n");
      webSocket.sendTXT("NICK " + String(twitch_nick) + "\r\n");
      webSocket.sendTXT("JOIN " + String(twitch_channel) + "\r\n");
      break;
    case WStype_TEXT: {
      Serial.printf("> %s\n", payload);
      String payload_str = String((char *)payload);
      //Serial.println("Received payload: " + payload_str);
      int first_colon = payload_str.indexOf(":");
      int second_colon = payload_str.indexOf(":", first_colon + 1);
      if (second_colon > 0) {
        String command_str = payload_str.substring(second_colon + 1);
        //Serial.println("Extracted command: " + command_str);
        command_str.trim();
        if (command_str == "red" || command_str == "green" || command_str == "blue" ||
            command_str == "yellow" || command_str == "orange" || command_str == "cyan" ||
            command_str == "purple" || command_str == "white" || command_str == "off") {
          setColor(command_str);
        } else if (command_str == "fade" || command_str == "blink" || command_str == "strobe" ||
                   command_str == "pattern" || command_str == "rainbow") {
          setAnimation(command_str);
        } else if (command_str.startsWith("brightness ")) {
          setBrightness(command_str.substring(11).toInt());
        } else if (command_str == "stop") {
          setAnimation("");
        } else {
          //Serial.println("Command not found in payload");
        }
      }
      break;
    }
    case WStype_DISCONNECTED:
      Serial.print("[WSc] Disconnected!\n");
      webSocket.sendTXT("PART " + String(TWITCH_CHANNEL) + "\r\n");
      break;
  }
}

void ledTask(void *parameter) {
  while (true) {
    ledAnimations.update();
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP32_AP"); // Create an access point with the name "ESP32_AP"
 
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi, resetting ESP32...");
    ESP.restart();
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Server address, port, and URL path
  webSocket.begin("irc-ws.chat.twitch.tv", 80, "/");

  // Event handler
  webSocket.onEvent(webSocketEvent);

  // Try every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  lastWiFiCheckTime = millis();

  // Create the LED task
  xTaskCreatePinnedToCore(
    ledTask,          // Task function
    "LED Task",       // Name of the task
    10000,            // Stack size
    NULL,             // Task input parameter
    1,                // Priority of the task
    &ledTaskHandle,   // Task handle
    0                 // Core where the task should run
  );
}

void loop() {
  webSocket.loop();

  // Check WiFi connection status periodically
  if (millis() - lastWiFiCheckTime >= wifiReconnectInterval) {
    lastWiFiCheckTime = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, starting WiFiManager...");
      WiFiManager wifiManager;
      wifiManager.startConfigPortal("ESP32_AP"); // Start configuration portal
    }
  }
}