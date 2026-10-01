#include "logger.h"
#include "config.h"
#include "globals.h"
#include <LittleFS.h>
#include <time.h>

void initStorage() {
  if (!LittleFS.begin(true)) {
    Serial.println("[FS] LittleFS mount failed!");
  } else {
    Serial.println("[FS] LittleFS mounted successfully");
  }
}

String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "unknown";
  char buf[20];
  sprintf(buf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buf);
}

void logEvent(String medicine, String status) {
  File file = LittleFS.open(LOG_FILE, "a");
  if (!file) {
    Serial.println("[FS] Could not open log file for appending");
    return;
  }
  file.println(getTimestamp() + "," + medicine + "," + status);
  file.close();
}

void handleHistory() {
  String page = "<html><head><title>Adherence History</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>table{border-collapse:collapse;width:100%;font-family:Arial;}";
  page += "td,th{border:1px solid #ccc;padding:8px;text-align:left;}";
  page += ".taken{background:#d4f7d4;} .missed{background:#f7d4d4;}</style>";
  page += "</head><body style='font-family:Arial;padding:20px;'>";
  page += "<h1>Adherence History</h1>";
  page += "<a href='/'>&larr; Back to dashboard</a><br><br>";
  page += "<table><tr><th>Time</th><th>Medicine</th><th>Status</th></tr>";

  File file = LittleFS.open(LOG_FILE, "r");
  if (file) {
    String lines[200];
    int count = 0;
    while (file.available() && count < 200) {
      lines[count] = file.readStringUntil('\n');
      lines[count].trim();
      if (lines[count].length() > 0) count++;
    }
    file.close();

    // Render in reverse chronological order
    for (int i = count - 1; i >= 0; i--) {
      int c1 = lines[i].indexOf(',');
      int c2 = lines[i].indexOf(',', c1 + 1);
      if (c1 == -1 || c2 == -1) continue;
      String t = lines[i].substring(0, c1);
      String med = lines[i].substring(c1 + 1, c2);
      String stat = lines[i].substring(c2 + 1);
      String cls = (stat == "TAKEN") ? "taken" : "missed";
      page += "<tr class='" + cls + "'><td>" + t + "</td><td>" + med + "</td><td>" + stat + "</td></tr>";
    }
  } else {
    page += "<tr><td colspan='3'>No log entries found.</td></tr>";
  }

  page += "</table></body></html>";
  server.send(200, "text/html", page);
}

void handleClearLog() {
  LittleFS.remove(LOG_FILE);
  server.sendHeader("Location", "/history");
  server.send(303);
}
