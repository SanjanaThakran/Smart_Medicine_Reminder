#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

void initStorage();
void logEvent(String medicine, String status);
String getTimestamp();
void handleHistory();
void handleClearLog();

#endif // LOGGER_H
