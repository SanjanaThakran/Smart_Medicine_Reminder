#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>

void initHardware();
void startMedicine1Alarm();
void startMedicine2Alarm();
void checkAlarm();
void checkScheduledAlarms();
void resetAlarms();

#endif // ALARM_MANAGER_H
