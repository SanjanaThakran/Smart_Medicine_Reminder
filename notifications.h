#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <Arduino.h>

void sendEmail(String subject, String body);
void sendWhatsApp(String body);
String base64Encode(String input);

#endif // NOTIFICATIONS_H
