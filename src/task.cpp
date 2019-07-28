//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <Arduino.h>
#include <M5Stack.h>
#include <Logger.h>
#include "task.h"
#include "lcd.h"

namespace task {
    static QueueHandle_t DisplayQueue, SaveQueue;

    void setup() {
        DisplayQueue = xQueueCreate(6, sizeof(message_t));
        if (DisplayQueue != 0) {
            TaskHandle_t handle;
            xTaskCreatePinnedToCore(displayTask, "display", 8192, NULL, 2, &handle, 1);
        }

        SaveQueue = xQueueCreate(2, sizeof(message_t));
        if (SaveQueue != 0) {
            TaskHandle_t handle;
            xTaskCreatePinnedToCore(saveTask, "save", 8192, NULL, 3, &handle, 1);
        }
    }

    void sendDisplayMessage(message_t &message) {
        xQueueSend(DisplayQueue, &message, portMAX_DELAY);
    }

    void sendSaveMessage(message_t &message) {
        xQueueSend(SaveQueue, &message, portMAX_DELAY);
    }

    void displayTask(void *param) {
        message_t msg;
        while (xQueueReceive(DisplayQueue, (void*)&msg, portMAX_DELAY)) {
            if (msg.displayType == task::GNSS) {
                lcd::showDatetime(msg.gnss.datetime);
                lcd::showMode(msg.gnss.satellites, msg.gnss.mode);
                lcd::showLocation(msg.gnss.lat, msg.gnss.lng);
                lcd::showSpeed(msg.gnss.speed);
                lcd::showCardinal(msg.gnss.cardinal);
                lcd::showAltitude(msg.gnss.altitude);
                lcd::showHDop(msg.gnss.hdop);
                lcd::showUsedSatellites(msg.gnss.usedSat);
            } else if (msg.displayType == ENV) {
                lcd::showEnvironment(msg.env.temperature, msg.env.humidity, msg.env.pressure);
            } else if (msg.displayType == GUIDE) {
                lcd::showGuide(msg.status);
            } else if (msg.displayType == MARK) {
                lcd::showMark(msg.status);
            }
        }
    }

    void saveTask(void *param) {
        message_t message;
        while (xQueueReceive(SaveQueue, (void*)&message, portMAX_DELAY)) {
            char fileName[20];

            gnssMessage_t &gnss = message.gnss;
            envMessage_t &env = message.env;

            time_t datetime = gnss.datetime;
            struct tm *jst = localtime(&datetime);
            sprintf(fileName, "/save_%4d%02d%02d.csv",
                    jst->tm_year + 1900, jst->tm_mon + 1, jst->tm_mday);

            String str = "filename = ";
            str += fileName;
            Logger::verbose(__func__, str.c_str());

            File file = SD.open(fileName, FILE_APPEND);
            if (file) {
                if (gnss.cardinal == NULL) {
                    gnss.cardinal = "";
                }

                file.printf("%ld,%f,%f,%f,%s,%f,%f,%f,%f,%f\r\n",
                        datetime, gnss.lat, gnss.lng, gnss.speed, gnss.cardinal,
                        gnss.altitude, gnss.hdop, env.temperature, env.humidity, env.pressure);
                file.close();

                Logger::verbose(__func__, "Save done.");
            } else {
                Logger::error(__func__, PSTR("Coud not open file."));
            }
        }
    }
}