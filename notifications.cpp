#include "notifications.h"
#include "config.h"
#include "globals.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

String base64Encode(String input) {
  static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String out = "";
  int val = 0, bits = -6;
  for (unsigned char c : input) {
    val = (val << 8) + c;
    bits += 8;
    while (bits >= 0) {
      out += chars[(val >> bits) & 0x3F];
      bits -= 6;
    }
  }
  if (bits > -6) out += chars[((val << 8) >> (bits + 8)) & 0x3F];
  while (out.length() % 4) out += '=';
  return out;
}

void sendWhatsApp(String body) {
  WiFiClientSecure client;
  client.setInsecure(); // Skip SSL cert verification for Twilio API

  HTTPClient http;
  String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(TWILIO_ACCOUNT_SID) + "/Messages.json";

  if (!http.begin(client, url)) {
    Serial.println("[Twilio] HTTP connection init failed");
    return;
  }

  String auth = base64Encode(String(TWILIO_ACCOUNT_SID) + ":" + String(TWILIO_AUTH_TOKEN));
  http.addHeader("Authorization", "Basic " + auth);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String toEncoded = String(TWILIO_WHATSAPP_TO);
  toEncoded.replace("+", "%2B");
  String fromEncoded = String(TWILIO_WHATSAPP_FROM);
  fromEncoded.replace("+", "%2B");

  String postData = "To=" + toEncoded + "&From=" + fromEncoded + "&Body=" + body;

  int httpCode = http.POST(postData);
  Serial.print("[Twilio] WhatsApp HTTP response: ");
  Serial.println(httpCode);

  if (httpCode != 200 && httpCode != 201) {
    Serial.println("[Twilio] Error response: " + http.getString());
  }

  http.end();
}

void sendEmail(String subject, String body) {
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;

  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  message.sender.name = "Medicine Reminder System";
  message.sender.email = AUTHOR_EMAIL;

  message.subject = subject;
  message.addRecipient("Caregiver", RECIPIENT_EMAIL);
  message.text.content = body.c_str();

  smtp.connect(&config);

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("[SMTP] Email dispatch failed");
  } else {
    Serial.println("[SMTP] Email sent successfully");
  }
}
