//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <vector>

#include <Arduino.h>
#include <TinyGPS++.h>
#include <Logger.h>
#include "gnss.h"
#include "task.h"
#include "bme280.h"


namespace gnss {
    static TinyGPSPlus GNSS;
    static TinyGPSCustom PositioningMode;
    static TinyGPSCustom UsedGPS[12];
    static TinyGPSCustom UsedGL[12];
    static TinyGPSCustom UsedBD[12];
    static double distance, lastLat, lastLng;

    void createTaskMessage(task::message_t&);

    void setup() {
        PositioningMode.begin(GNSS, "GPGSA", 2);
        for (int i = 0; i < 12; i++) {
            UsedGPS[i].begin(GNSS, "GPGSA", i + 3);
            UsedGL[i].begin(GNSS, "GLGSA", i + 3);
            UsedBD[i].begin(GNSS, "BDGSA", i + 3);
        }

        distance = 20;
        lastLat = 0;
        lastLng = 0;
    }

    void setDistace(double d) {
        distance = d;
    }

    void update(logger::Status status) {
        static unsigned long lastProcess = millis();
        static unsigned long lastSave = millis();

        if (Serial2.available()) {
            int ch = Serial2.read();
            bool isValid = GNSS.encode(ch);

            if (isValid) {
                if (status == logger::SAVE_DISTANCE || status == logger::SAVE_DISTANCE_CONFIRM) {
                    double lat = GNSS.location.lat();
                    double lng = GNSS.location.lng();
                    double current = GNSS.distanceBetween(lat, lng, lastLat, lastLng);
                    if (current >= distance) {
                        lastLat = lat;
                        lastLng = lng;

                        task::message_t msg;
                        createTaskMessage(msg);
                        bme280::createTaskMessage(msg);
                        task::sendSaveMessage(msg);
                    }
                }

                unsigned long now = millis();
                unsigned long diff = now - lastProcess;
                if (diff > 4500) {
                    lastProcess = now;

                    task::message_t msg;
                    createTaskMessage(msg);
                    task::sendDisplayMessage(msg);

                    if (status == logger::SAVE_TIME) {
                        diff = now - lastSave;
                        if (diff > 30000) {
                            lastSave = now;
                            bme280::createTaskMessage(msg);
                            task::sendSaveMessage(msg);
                        }
                    }
                }
            }
        }
    }

    void sat2vector(TinyGPSCustom sat[], std::vector<uint8_t> &v) {
        for (int i = 0; i < MaxSatellite; i++) {
            if (sat[i].isValid()) {
                uint8_t value = atoi(sat[i].value());
                v.push_back(value);
            }
        }
    }

    void createTaskMessage(task::message_t &msg) {
        struct tm utc;

        msg.displayType = task::GNSS;
        msg.gnss.datetime = 0;

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
                time_t datetime = mktime(&utc) + TimeOffset;

                msg.gnss.datetime = datetime;
            }
        }

        if (PositioningMode.isUpdated()) {
            msg.gnss.mode = atoi(PositioningMode.value());
        } else {
            msg.gnss.mode = 0;
        }

        if (GNSS.location.isUpdated()) {
            msg.gnss.lat = GNSS.location.lat();
            msg.gnss.lng = GNSS.location.lng();
        } else {
            msg.gnss.lat = 0;
            msg.gnss.lng = 0;
        }

        if (GNSS.speed.isUpdated()) {
            msg.gnss.speed = GNSS.speed.kmph();
        } else {
            msg.gnss.speed = 0;
        }

        if (GNSS.course.isUpdated()) {
            msg.gnss.cardinal = GNSS.cardinal(GNSS.course.deg());
        } else {
            msg.gnss.cardinal = NULL;
        }

        if (GNSS.altitude.isUpdated()) {
            msg.gnss.altitude = GNSS.altitude.meters();
        } else {
            msg.gnss.altitude = 0;
        }

        if(UsedGPS[0].isUpdated()) {
            std::vector<uint8_t> v;
            sat2vector(UsedGPS, v);
            msg.gnss.usedSat["G"] = v;
        }
        if(UsedGL[0].isUpdated()) {
            std::vector<uint8_t> v;
            sat2vector(UsedGL, v);
            msg.gnss.usedSat["R"] = v;
        }
        if(UsedBD[0].isUpdated()) {
            std::vector<uint8_t> v;
            sat2vector(UsedBD, v);
            msg.gnss.usedSat["B"] = v;
        }

        if (GNSS.satellites.isUpdated()) {
            msg.gnss.satellites = GNSS.satellites.value();
        } else {
            msg.gnss.satellites = 0;
        }

        if (GNSS.hdop.isUpdated()) {
            msg.gnss.hdop = GNSS.hdop.hdop();
        } else {
            msg.gnss.hdop = 0;
        }
    }
}
