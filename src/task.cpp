//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#include <Arduino.h>
#include <M5Stack.h>
#include "task.h"
#include "lcd.h"

namespace task {
    static QueueHandle_t DisplayQueue, SaveQueue;

    void setup() {
        DisplayQueue = xQueueCreate(5, sizeof(displayMessage_t));
        if (DisplayQueue != 0) {
            TaskHandle_t handle;
            xTaskCreatePinnedToCore(displayTask, "display", 8192, NULL, 2, &handle, 1);
        }

        SaveQueue = xQueueCreate(5, sizeof(saveMessage_t));
        if (SaveQueue != 0) {
            TaskHandle_t handle;
            xTaskCreatePinnedToCore(saveTask, "save", 8192, NULL, 3, &handle, 1);
        }
    }

    void sendDisplayMessage(displayMessage_t &message) {
        xQueueSend(DisplayQueue, &message, portMAX_DELAY);
    }

    void sendSaveMessage(saveMessage_t &message) {
        xQueueSend(SaveQueue, &message, portMAX_DELAY);
    }

    void displayTask(void *param) {
        displayMessage_t msg;
        while (xQueueReceive(DisplayQueue, (void*)&msg, portMAX_DELAY)) {
            switch (msg.displayType) {
                case Datetime:
                    lcd::showDatetime(msg.data.datetime);
                    break;
                case Mode:
                    lcd::showMode(msg.data.mode);
                    break;
                case Location:
                    lcd::showLatitude(msg.data.location.lat);
                    lcd::showLongitude(msg.data.location.lng);
                    break;
                case Speed:
                    lcd::showSpeed(msg.data.speed);
                    break;
                case Cardinal:
                    lcd::showCardinal(msg.data.cardinal);
                    break;
                case Altitude:
                    lcd::showAltitude(msg.data.altitude);
                    break;
            }
        }
    }

    void saveTask(void *param) {
        saveMessage_t message;
        while (xQueueReceive(SaveQueue, (void*)&message, portMAX_DELAY)) {

        }
    }
}