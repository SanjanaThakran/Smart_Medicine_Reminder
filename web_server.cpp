#include "web_server.h"
#include "config.h"
#include "globals.h"
#include "logger.h"
#include "alarm_manager.h"
#include <time.h>

void saveSettings() {
  if (server.hasArg("m1h")) med1Hour = server.arg("m1h").toInt();
  if (server.hasArg("m1m")) med1Minute = server.arg("m1m").toInt();
  if (server.hasArg("m2h")) med2Hour = server.arg("m2h").toInt();
  if (server.hasArg("m2m")) med2Minute = server.arg("m2m").toInt();

  prefs.putInt("m1h", med1Hour);
  prefs.putInt("m1m", med1Minute);
  prefs.putInt("m2h", med2Hour);
  prefs.putInt("m2m", med2Minute);

  med1Triggered = false;
  med2Triggered = false;
  med1Status = "PENDING";
  med2Status = "PENDING";

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStatus() {
  String json = "{";
  json += "\"med1\":\"" + med1Status + "\",";
  json += "\"med2\":\"" + med2Status + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleRoot() {
  struct tm timeinfo;
  String currentTime = "Not Synced";

  if (getLocalTime(&timeinfo)) {
    char buf[20];
    sprintf(buf, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    currentTime = buf;
  }

  String page;
  page += "<html><head><title>Medicine Reminder</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<script>";
  page += "function updateStatus(){";
  page += "fetch('/status').then(r=>r.json()).then(d=>{";
  page += "document.getElementById('m1status').innerHTML=d.med1;";
  page += "document.getElementById('m2status').innerHTML=d.med2;";
  page += "});}";
  page += "setInterval(updateStatus,1000);";
  page += "</script></head>";

  page += "<body style='font-family:Arial;padding:20px;'>";
  page += "<h1>SMART MEDICINE REMINDER</h1>";
  page += "<h2>Current Time: " + currentTime + "</h2><hr>";

  page += "<form action='/save'>";
  page += "<h3>Medicine 1</h3>";
  page += "Hour: <input type='number' name='m1h' min='0' max='23' value='" + String(med1Hour) + "'><br><br>";
  page += "Minute: <input type='number' name='m1m' min='0' max='59' value='" + String(med1Minute) + "'><br><br>";

  page += "<h3>Medicine 2</h3>";
  page += "Hour: <input type='number' name='m2h' min='0' max='23' value='" + String(med2Hour) + "'><br><br>";
  page += "Minute: <input type='number' name='m2m' min='0' max='59' value='" + String(med2Minute) + "'><br><br>";

  page += "<input type='submit' value='Save Times'>";
  page += "</form><br><br>";

  page += "<form action='/reset'>";
  page += "<input type='submit' value='Reset Alarms'>";
  page += "</form><hr>";

  page += "<h3>Saved Schedule</h3>";
  page += "Medicine 1: " + String(med1Hour) + ":" + (med1Minute < 10 ? "0" : "") + String(med1Minute) + "<br><br>";
  page += "Medicine 2: " + String(med2Hour) + ":" + (med2Minute < 10 ? "0" : "") + String(med2Minute);

  page += "<hr><h3>Status</h3>";
  page += "Medicine 1: <span id='m1status'>" + med1Status + "</span><br><br>";
  page += "Medicine 2: <span id='m2status'>" + med2Status + "</span>";

  page += "<hr><a href='/history'>View Adherence History</a>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void initWebServer() {
  server.on("/", handleRoot);
  server.on("/save", saveSettings);
  server.on("/reset", resetAlarms);
  server.on("/status", handleStatus);
  server.on("/history", handleHistory);
  server.on("/clearlog", handleClearLog);

  server.begin();
  Serial.println("[Web] HTTP Server listening on port 80");
}
