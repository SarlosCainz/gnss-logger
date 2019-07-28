//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <string>
#include <sstream>

#include <Arduino.h>
#include <M5Stack.h>
#include <Logger.h>
#include "task.h"

namespace lcd {
#define FONT 1
#define LCD_BRIGHTNESS 50
#define OFFSET_X (240)
#define ITEM_TOP (25)
#define ITEM_HEIGHT (22)

    static const char *MenuItems[] PROGMEM = {
            "Latitude",
            "Longitude",
            "Speed(Km/h)",
            "Course",
            "Altitude(m)",
            "HDOP",
            "Temperature(C)",
            "Humidity(%)",
            "Pressure(HPa)",
            ""
    };

    static const char* GNSS[] PROGMEM = {"G", "R", "B", ""};

    static uint16_t color;

    uint8_t getItemY(uint8_t index) {
        return ITEM_TOP + index * ITEM_HEIGHT;
    }

    void setup() {
        M5.Lcd.setBrightness(LCD_BRIGHTNESS);
        M5.Lcd.fillScreen(TFT_WHITE);
        M5.Lcd.fillRect(0, 0, 320, 20, TFT_NAVY);
        M5.Lcd.fillRect(0, 220, 320, 20, TFT_NAVY);

        M5.Lcd.setFreeFont(&FreeMono9pt7b);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        int x = 5;
        int y = ITEM_TOP;

        for (int i = 0; *MenuItems[i] != '\0'; i++) {
            M5.Lcd.drawString(MenuItems[i], x, y);
            if (i != 0) {
                M5.Lcd.drawFastHLine(0, y - 5, 320, TFT_NAVY);
            }
            y += ITEM_HEIGHT;
        }

        M5.Lcd.drawFastVLine(160, 20, 200, TFT_NAVY);
        M5.Lcd.drawFastVLine(274, 20, 200, TFT_NAVY);

        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
        M5.Lcd.drawString("[", 214, 2, FONT);
        M5.Lcd.drawString("]", 266, 2, FONT);

        color = M5.Lcd.color565(255, 255, 180);
        M5.Lcd.fillRect(275, 20, 45, 200, color);
    }

    void showDatetime(time_t datetime) {
        char buf[21];

        struct tm *jst = localtime(&datetime);
        sprintf(buf, "%4d/%02d/%02d %02d:%02d ",
                jst->tm_year + 1900, jst->tm_mon + 1, jst->tm_mday, jst->tm_hour, jst->tm_min);

        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
        M5.Lcd.drawString(buf, 8, 2, FONT);
    }

    void showMode(uint8_t satellites, uint8_t mode) {
        char buf[32];
        static const PROGMEM char *mode_str[] = {"--", "NO", "2D", "3D"};

        if (mode > 3) {
            mode = 0;
        }
        sprintf(buf, " %2s", mode_str[mode]);

        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
        M5.Lcd.drawRightString(buf, 310, 2, FONT);
    }

    void showLocation(double latitude, double longitude) {
        char buf[16];
        int x = OFFSET_X + 25;
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);

        sprintf(buf, "%9.5f", latitude);
        M5.Lcd.drawRightString(buf, x, getItemY(0), FONT);

        sprintf(buf, "%9.5f", longitude);
        M5.Lcd.drawRightString(buf, x, getItemY(1), FONT);

    }

    void showSpeed(double speed) {
        char buf[16];

        sprintf(buf, " %5.1f", speed);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(2), FONT);
    }

    void showCardinal(const char *cardinal) {
        if (cardinal == NULL) {
            cardinal = "--";
        }
        char buf[5];

        sprintf(buf, " %3s", cardinal);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(3), FONT);
    }

    void showAltitude(double altitude) {
        char buf[16];

        sprintf(buf, " %6.1f", altitude);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(4), FONT);
    }

    void showHDop(double hdop) {
        char buf[16];

        sprintf(buf, " %6.2f", hdop);
        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(5), FONT);
    }

    void showSat(const char* name, uint8_t &y, std::vector<uint8_t> &sat) {
        char buf[5];

        // Sort
        std::sort(sat.begin(), sat.end());

        for (auto itr = sat.begin(); itr != sat.end(); ++itr) {
            if (*itr != 0) {
                sprintf(buf, "%s%d", name, *itr);
                M5.Lcd.drawRightString(buf, 313, y, FONT);
                y += 16;
            }
        }
    }

    void showUsedSatellites(std::map<std::string, std::vector<uint8_t>> usedSat) {
        uint8_t y = 25;

        M5.Lcd.fillRect(275, 20, 45, 200, color);
        M5.Lcd.setTextColor(TFT_NAVY);

        for (int i = 0; *GNSS[i] != '\0'; i++) {
            const char *key = GNSS[i];
            showSat(key, y, usedSat[key]);
        }
    }

    void showEnvironment(float temperature, float humidity, float pressure) {
        char buf[16];

        M5.Lcd.setTextColor(TFT_NAVY, TFT_WHITE);

        sprintf(buf, " %5.1f", temperature);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(6), FONT);

        sprintf(buf, " %5.1f", humidity);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(7), FONT);

        sprintf(buf, " %6.1f", pressure);
        M5.Lcd.drawRightString(buf, OFFSET_X, getItemY(8), FONT);
    }

    void showGuide(logger::Status status) {
        const char *msg = "";

        if (status == logger::IDLE) {
            msg = PSTR("Push: A=Time, B=Distance");
        } else if (status == logger::SAVE_TIME || status == logger::SAVE_DISTANCE) {
            msg = PSTR("Push: C=Stop");
        } else {
            msg = PSTR("Push: A=OK, B=Cancel");
        }

        M5.Lcd.fillRect(0, 220, 320, 20, TFT_NAVY);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);

        M5.Lcd.drawCentreString(msg, 160, 223, FONT);
    }

    void showMark(logger::Status status) {
        static bool flick = true;

        M5.Lcd.fillRect(224, 0, 45, 20, TFT_NAVY);
        if (flick) {
            const char *mark = PSTR("----");

            if (status == logger::SAVE_TIME || status == logger::SAVE_TIME_CONFIRM) {
                mark = PSTR("Time");
            } else if (status == logger::SAVE_DISTANCE || status == logger::SAVE_DISTANCE_CONFIRM) {
                mark = PSTR("Dist");
            }

            M5.Lcd.setTextColor(TFT_WHITE, TFT_NAVY);
            M5.Lcd.drawCentreString(mark, 245, 2, FONT);
        }

        flick = !flick;
    }
}
