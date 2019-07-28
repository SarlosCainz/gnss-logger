//
// Created by MATSUNAGA Takuji on 2019-07-27.
//

#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Logger.h>
#include "task.h"
#include "bme280.h"

namespace bme280 {

    static Adafruit_BME280 bme;
    static bool status;

    void setup() {
        status = bme.begin();
        if (status) {
            String str = "BME280: ID = " + bme.sensorID();
            Logger::notice(str.c_str());
        }
    }

    void update() {
        static unsigned long lastProcess = 0;

        if (status) {
            unsigned long diff = millis() - lastProcess;
            if (diff > 4500) {
                lastProcess = millis();

                task::message_t msg;
                memset(&msg, 0, sizeof(task::message_t));
                msg.displayType = task::ENV;

                createTaskMessage(msg);
                task::sendDisplayMessage(msg);
            }
        }
    }

    void createTaskMessage(task::message_t &msg) {
        msg.env.temperature = bme.readTemperature();
        msg.env.humidity = bme.readHumidity();
        msg.env.pressure = bme.readPressure() / 100.0F;
    }
}
