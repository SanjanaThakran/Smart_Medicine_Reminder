#include <WiFi.h>
#include <time.h>

#include "config.h"
#include "globals.h"
#include "notifications.h"
#include "logger.h"
#include "alarm_manager.h"
#include "web_server.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n--- ESP32 Smart Medicine Reminder ---");

  // Initialize LittleFS storage
  initStorage();

  // Load saved dosage schedules from NVS
  prefs.begin("medicine", false);
  med1Hour = prefs.getInt("m1h", 9);
  med1Minute = prefs.getInt("m1m", 0);
  med2Hour = prefs.getInt("m2h", 14);
  med2Minute = prefs.getInt("m2m", 0);

  // Connect to local WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("[WiFi] IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure NTP time (19800 = GMT+5:30 for IST)
  configTime(19800, 0, "pool.ntp.org");

  // Setup Web Server routes
  initWebServer();

  // Configure hardware pins
  initHardware();
}

void loop() {
  server.handleClient();
  checkAlarm();
  checkScheduledAlarms();
  delay(500);
}
