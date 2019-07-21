//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <Arduino.h>
#include <TinyGPS++.h>
#include <Logger.h>
#include "gnss.h"
#include "task.h"

#define TIME_OFFSET 9 * 3600

namespace gnss {
    static TinyGPSPlus GNSS;
    static TinyGPSCustom PositioningMode;
    static TinyGPSCustom UserSatellite[12];
    static double distance;

    void datetimeProcess();
    void modeProcess();
    void locationProcess();
    void speedProcess();
    void courceProcess();
    void altitudeProcess();

    void setup() {
        PositioningMode.begin(GNSS, "GPGSA", 2);
        for (int i = 0; i < 12; i++) {
            UserSatellite[i].begin(GNSS, "GPGSA", i + 3);
        }

        distance = 0;
    }

    void setDistace(double d) {
        distance = d;
    }

    void update() {
        if (Serial2.available()) {
            int ch = Serial2.read();
            bool isValid = GNSS.encode(ch);

            if (isValid) {
                datetimeProcess();
                modeProcess();
                locationProcess();
                speedProcess();
                courceProcess();
                altitudeProcess();
            }
        }
    }

    void datetimeProcess() {
        struct tm utc;

        if (GNSS.date.isUpdated() && GNSS.date.isValid()) {
            utc.tm_year = GNSS.date.year() - 1900;
            utc.tm_mon = GNSS.date.month() - 1;
            utc.tm_mday = GNSS.date.day();
        }
        if (GNSS.time.isUpdated() && GNSS.time.isValid()) {
            utc.tm_hour = GNSS.time.hour();
            utc.tm_min = GNSS.time.minute();
            utc.tm_sec = GNSS.time.second();

            if (utc.tm_year != 0) {
                time_t datetime = mktime(&utc) + TIME_OFFSET;

                task::displayMessage_t msg;
                msg.displayType = task::Datetime;
                msg.data.datetime = datetime;
                task::sendDisplayMessage(msg);
            }
        }
    }

    void modeProcess() {
        if (PositioningMode.isUpdated()) {
            task::displayMessage_t msg;
            msg.displayType = task::Mode;
            msg.data.mode = atoi(PositioningMode.value());

            task::sendDisplayMessage(msg);
        }
    }

    void locationProcess() {
        static double lastLat = 0;
        static double lastLng = 0;

        if (GNSS.location.isUpdated()) {
            double lat = GNSS.location.lat();
            double lng = GNSS.location.lng();

            task::displayMessage_t msg;
            msg.displayType = task::Location;
            msg.data.location.lat = lat;
            msg.data.location.lng = lng;

            task::sendDisplayMessage(msg);

            if (distance != 0) {
                double current = GNSS.distanceBetween(lat, lng, lastLat, lastLng);
                if (current >= distance) {
                    lastLat = lat;
                    lastLng = lng;
                }
            }
        }
    }

    void speedProcess() {
        if (GNSS.speed.isUpdated()) {
            task::displayMessage_t msg;
            msg.displayType = task::Speed;
            msg.data.speed = GNSS.speed.kmph();

            task::sendDisplayMessage(msg);
        }
    }

    void courceProcess() {
        if (GNSS.course.isUpdated()) {
            task::displayMessage_t msg;
            msg.displayType = task::Cardinal;
            msg.data.cardinal = GNSS.cardinal(GNSS.course.deg());

            task::sendDisplayMessage(msg);
        }
    }

    void altitudeProcess() {
        if (GNSS.altitude.isUpdated()) {
            task::displayMessage_t msg;
            msg.displayType = task::Altitude;
            msg.data.altitude = GNSS.altitude.meters();

            task::sendDisplayMessage(msg);
        }
    }
}
