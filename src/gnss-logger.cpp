//
// Created by MATSUNAGA Takuji on 2019-07-21.
//
#include <Arduino.h>
#include <M5Stack.h>
#include <Logger.h>
#include <TinyGPS++.h>
#include "task.h"
#include "gnss.h"
#include "lcd.h"


void setup() {
    M5.begin();

    Serial.begin(115200);
    Serial2.begin(115200);

    Logger::setLogLevel(Logger::VERBOSE);
    Logger::verbose(__func__, "start.");

    task::setup();
    lcd::setup();
    gnss::setup();
}

void loop() {
    M5.update();

    gnss::update();
}


