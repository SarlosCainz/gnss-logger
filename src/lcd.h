//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#ifndef GNSS_LOGGER_LCD_H
#define GNSS_LOGGER_LCD_H

#include <map>
#include <vector>

namespace lcd {
    void setup();

    void showDatetime(time_t datetime);
    void showMode(uint8_t satellites, uint8_t mode);
    void showLocation(double latitude, double longitude);
    void showSpeed(double speed);
    void showCardinal(const char *cardinal);
    void showAltitude(double altitude);
    void showHDop(double hdop);
    void showUsedSatellites(std::map<std::string, std::vector<uint8_t>> usedSat);
    void showEnvironment(float temperature, float humidity, float pressure);
    void showGuide(logger::Status status);
    void showMark(logger::Status status);
}

#endif //GNSS_LOGGER_LCD_H
