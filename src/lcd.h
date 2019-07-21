//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#ifndef GNSS_LOGGER_LCD_H
#define GNSS_LOGGER_LCD_H

namespace lcd {
    void setup();

    void showDatetime(time_t datetime);
    void showMode(uint8_t mode);
    void showLatitude(double latitude);
    void showLongitude(double longitude);
    void showSpeed(double speed);
    void showCardinal(const char *cardinal);
    void showAltitude(double altitude);
}

#endif //GNSS_LOGGER_LCD_H
