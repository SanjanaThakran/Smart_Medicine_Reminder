#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>

void initWebServer();
void handleRoot();
void saveSettings();
void handleStatus();

#endif // WEB_SERVER_H
