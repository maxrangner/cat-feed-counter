#include <Arduino.h>

String convertTime(time_t timestamp) {
    struct tm currentTime = *localtime(&timestamp);
    char convertedTime[50];
    strftime(convertedTime, sizeof(convertedTime), "%H:%M:%S", &currentTime); //  "%a%e %b %H:%M:%S"
    return String(convertedTime);
}