#include "globals.h"

const char* ssid = "Sanjana";
const char* password = "12345678";

WebServer server(80);
Preferences prefs;
SMTPSession smtp;
Session_Config config;
SMTP_Message message;

// Initial default schedule: Med 1 at 09:00, Med 2 at 14:00
int med1Hour = 9;
int med1Minute = 0;
int med2Hour = 14;
int med2Minute = 0;

String med1Status = "PENDING";
String med2Status = "PENDING";

bool med1Triggered = false;
bool med2Triggered = false;

bool alarmActive = false;
int activeMedicine = 0;
unsigned long alarmStart = 0;
