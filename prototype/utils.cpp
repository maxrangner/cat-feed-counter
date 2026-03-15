#include "utils.h"

time_t getTime() {
  return time(nullptr);
}

String convertTime(time_t timestamp) {
    struct tm currentTime = *localtime(&timestamp);
    char convertedTime[50];
    strftime(convertedTime, sizeof(convertedTime), "%H:%M:%S", &currentTime); //  "%a%e %b %H:%M:%S"
    return String(convertedTime);
}

int getConvertedDay(time_t t) {
    int returnTime = 0;
    tm timeInfo = *localtime(&t);
    returnTime = (timeInfo.tm_year + 1900) * 10000;
    returnTime += (timeInfo.tm_mon + 1) * 100;
    returnTime += timeInfo.tm_mday;
    return returnTime;
}