//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <Arduino.h>
#include <M5Stack.h>
#include <Logger.h>
#include <TinyGPS++.h>
#include "gnss_logger.h"
#include "task.h"
#include "gnss.h"
#include "lcd.h"
#include "bme280.h"

static logger::Status status;
static logger::Status prevStatus;
static task::message_t msg;

void setup() {
    M5.begin();

    Serial.begin(115200);
    Serial2.begin(115200);

    Logger::setLogLevel(Logger::VERBOSE);
    Logger::verbose(__func__, "start.");

    task::setup();
    lcd::setup();
    gnss::setup();
    bme280::setup();

    status = prevStatus = logger::IDLE;

    msg.displayType = task::GUIDE;
    msg.status = status;
    task::sendDisplayMessage(msg);
}

void loop() {
    static unsigned long lastProcess = millis();

    M5.update();

    gnss::update(status);
    bme280::update();

    if (M5.BtnA.wasPressed()) {
        if (status == logger::IDLE) {
            status = logger::SAVE_TIME_READY;
        } else if (status == logger::SAVE_TIME_READY) {
            status = logger::SAVE_TIME;
        } else if (status == logger::SAVE_DISTANCE_READY) {
            status = logger::SAVE_DISTANCE;
        } else if (status == logger::SAVE_TIME_CONFIRM || status == logger::SAVE_DISTANCE_CONFIRM) {
            status = logger::IDLE;
        }
    } else if (M5.BtnB.wasPressed()) {
        if (status == logger::IDLE) {
            status = logger::SAVE_DISTANCE_READY;
        } else if (status == logger::SAVE_TIME_READY || status == logger::SAVE_DISTANCE_READY) {
            status = logger::IDLE;
        } else if (status == logger::SAVE_TIME_CONFIRM) {
            status = logger::SAVE_TIME;
        } else if (status == logger::SAVE_DISTANCE_CONFIRM) {
            status = logger::SAVE_DISTANCE;
        }
    } else if (M5.BtnC.wasPressed()) {
        if (status == logger::SAVE_TIME) {
            status = logger::SAVE_TIME_CONFIRM;
        } else if (status == logger::SAVE_DISTANCE) {
            status = logger::SAVE_DISTANCE_CONFIRM;
        }
    }

    if (prevStatus != status) {
        msg.displayType = task::GUIDE;
        msg.status = status;
        task::sendDisplayMessage(msg);
        prevStatus = status;
    }

    unsigned long diff = millis() - lastProcess;
    if (diff > 1000) {
        msg.displayType = task::MARK;
        msg.status = status;
        task::sendDisplayMessage(msg);
        lastProcess = millis();
    }
}


