#include "alarm_manager.h"
#include "config.h"
#include "globals.h"
#include "logger.h"
#include "notifications.h"
#include <time.h>

void initHardware() {
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void startMedicine1Alarm() {
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(BUZZER, HIGH);

  med1Status = "ALERT ACTIVE";
  alarmActive = true;
  activeMedicine = 1;
  alarmStart = millis();

  Serial.println("[Alarm] Medicine 1 Alert Active");
}

void startMedicine2Alarm() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, HIGH);

  med2Status = "ALERT ACTIVE";
  alarmActive = true;
  activeMedicine = 2;
  alarmStart = millis();

  Serial.println("[Alarm] Medicine 2 Alert Active");
}

void checkAlarm() {
  if (!alarmActive) return;

  // Check IR 1 sensor (Active LOW when container is lifted)
  if (activeMedicine == 1 && digitalRead(IR1) == LOW) {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(BUZZER, LOW);
    med1Status = "TAKEN";
    alarmActive = false;

    logEvent("Medicine 1", "TAKEN");
    Serial.println("[Alarm] Medicine 1 confirmed TAKEN");
  }

  // Check IR 2 sensor
  if (activeMedicine == 2 && digitalRead(IR2) == LOW) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    med2Status = "TAKEN";
    alarmActive = false;

    logEvent("Medicine 2", "TAKEN");
    Serial.println("[Alarm] Medicine 2 confirmed TAKEN");
  }

  // 30-second timeout check
  if (alarmActive && (millis() - alarmStart > 30000)) {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    if (activeMedicine == 1) {
      med1Status = "MISSED";
      logEvent("Medicine 1", "MISSED");
      sendEmail("Medicine 1 Missed", "Medicine 1 was not taken within 30 seconds. Please check on the patient.");
      sendWhatsApp("Medicine 1 was NOT taken within 30 seconds. Please check on the patient.");
    }

    if (activeMedicine == 2) {
      med2Status = "MISSED";
      logEvent("Medicine 2", "MISSED");
      sendEmail("Medicine 2 Missed", "Medicine 2 was not taken within 30 seconds. Please check on the patient.");
      sendWhatsApp("Medicine 2 was NOT taken within 30 seconds. Please check on the patient.");
    }

    alarmActive = false;
    Serial.println("[Alarm] Medicine MISSED - Notifications sent!");
  }
}

void checkScheduledAlarms() {
  struct tm timeinfo;

  if (getLocalTime(&timeinfo)) {
    if (!med1Triggered && timeinfo.tm_hour == med1Hour && timeinfo.tm_min == med1Minute) {
      med1Triggered = true;
      startMedicine1Alarm();
    }

    if (!med2Triggered && timeinfo.tm_hour == med2Hour && timeinfo.tm_min == med2Minute) {
      med2Triggered = true;
      startMedicine2Alarm();
    }
  }
}

void resetAlarms() {
  activeMedicine = 0;
  med1Triggered = false;
  med2Triggered = false;

  med1Status = "PENDING";
  med2Status = "PENDING";
  alarmActive = false;

  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  server.sendHeader("Location", "/");
  server.send(303);
}
