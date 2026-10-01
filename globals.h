#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ESP_Mail_Client.h>

// Global Objects
extern WebServer server;
extern Preferences prefs;
extern SMTPSession smtp;
extern Session_Config config;
extern SMTP_Message message;

// Schedule Timings
extern int med1Hour;
extern int med1Minute;
extern int med2Hour;
extern int med2Minute;

// Alarm & Medicine Statuses
extern String med1Status;
extern String med2Status;

extern bool med1Triggered;
extern bool med2Triggered;

extern bool alarmActive;
extern int activeMedicine;
extern unsigned long alarmStart;

#endif // GLOBALS_H
