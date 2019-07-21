//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <Arduino.h>
#include <M5Stack.h>
#include <Logger.h>

namespace lcd {
#define FONT 1
#define LCD_BRIGHTNESS 50
#define OFFSET_X 120

    static const char *MenuItems[] PROGMEM = {
            "Latitude:",
            "Longitude:",
            "Speed:",
            "Course:",
            "Altitude:",
            ""
    };

    void setup() {

        M5.Lcd.setBrightness(LCD_BRIGHTNESS);
        M5.Lcd.fillScreen(TFT_WHITE);
        M5.Lcd.fillRect(0, 0, 320, 22, TFT_NAVY);

        M5.Lcd.setFreeFont(&FreeMono9pt7b);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        int x = 2;
        int y = 32;

        for (int i = 0; *MenuItems[i] != '\0'; i++) {
            M5.Lcd.drawString(MenuItems[i], x, y);
            y += 23;
        }
    }

    void showDatetime(time_t datetime) {
        char buf[21];

        struct tm *jst = localtime(&datetime);
        sprintf(buf, "%4d/%02d/%02d %02d:%02d:%02d ",
                jst->tm_year + 1900, jst->tm_mon + 1, jst->tm_mday, jst->tm_hour, jst->tm_min, jst->tm_sec);

        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
        M5.Lcd.drawString(buf, 2, 2, FONT);
    }

    void showMode(uint8_t mode) {
        static const PROGMEM char *mode_str[] = {"--", "NO", "2D", "3D"};

        if (mode > 3) {
            mode = 0;
        }

        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
        M5.Lcd.drawRightString(mode_str[mode], 315, 2, FONT);
    }

    void showLatitude(double latitude) {
        char buf[16];

        sprintf(buf, "%9.5f ", latitude);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawString(buf, OFFSET_X, 32, FONT);
    }

    void showLongitude(double longitude) {
        char buf[16];

        sprintf(buf, "%9.5f ", longitude);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawString(buf, OFFSET_X, 55, FONT);
    }

    void showSpeed(double speed) {
        char buf[16];

        sprintf(buf, "%5.1f Km/h", speed);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawString(buf, OFFSET_X, 78, FONT);
    }

    void showCardinal(const char *cardinal) {
        char buf[5];

        sprintf(buf, "%3s ", cardinal);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawString(buf, OFFSET_X, 101, FONT);
    }

    void showAltitude(double altitude) {
        char buf[16];

        sprintf(buf, "%5.1f m ", altitude);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawString(buf, OFFSET_X, 124, FONT);

        Logger::verbose(__func__, buf);
    }
}