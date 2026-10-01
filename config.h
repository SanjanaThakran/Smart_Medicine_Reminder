#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi Settings
extern const char* ssid;
extern const char* password;

// Gmail SMTP Credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "YOUR_GMAIL_APP_PASSWORD"
#define RECIPIENT_EMAIL "caregiver_email@gmail.com"

// Twilio WhatsApp API Config
#define TWILIO_ACCOUNT_SID "YOUR_TWILIO_ACCOUNT_SID"
#define TWILIO_AUTH_TOKEN  "YOUR_TWILIO_AUTH_TOKEN"
#define TWILIO_WHATSAPP_FROM "whatsapp:+14155238886"   
#define TWILIO_WHATSAPP_TO "whatsapp:+910000000000"    

// File path for history logs
#define LOG_FILE "/log.csv"

// ESP32 Pin Connections
const int IR1 = 21;        // Sensor 1 (Slot 1)
const int IR2 = 4;         // Sensor 2 (Slot 2)
const int BLUE_LED = 5;    // Alert LED 1
const int RED_LED = 15;    // Alert LED 2
const int BUZZER = 18;     // Shared Alarm Buzzer

#endif
