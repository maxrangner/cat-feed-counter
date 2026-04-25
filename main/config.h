#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

constexpr uint8_t kBrightnessLow = 15;
constexpr uint8_t kBrightnessMedium = 20;
constexpr uint8_t kBrightnessHigh = 30;
constexpr uint64_t kOneDayUs = 86400000000ULL;
constexpr char kAppTimezone[] = "CET-1CEST,M3.5.0/2,M10.5.0/3";

constexpr uint8_t kNumScreens = 3;
constexpr uint8_t kNumOptions = 4;

constexpr uint8_t kBtnMainPin = 3;
constexpr uint8_t kBtnSidePin = 9;

constexpr uint8_t kPinNumMosi = 6;
constexpr uint8_t kPinNumClk = 7;
constexpr uint8_t kPinNumCs = 14;
constexpr uint8_t kPinNumDc = 15;
constexpr uint8_t kPinNumRst = 21;
constexpr uint8_t kPinNumBl = 22;

constexpr uint16_t kLcdHRes = 172;
constexpr uint16_t kLcdVRes = 320;
constexpr uint32_t kLcdBufSize = kLcdHRes * 80U;
constexpr uint32_t kDisplaySpiPixelClockHz = 40U * 1000U * 1000U;
constexpr uint32_t kBacklightMaxDuty = 1023U;

#endif
