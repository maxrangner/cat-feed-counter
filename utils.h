#pragma once

#include <Arduino.h>
#include "time.h"
#include "definitions.h"

time_t getTime();
String convertTime(time_t timestamp);
int getConvertedDay(time_t t);
