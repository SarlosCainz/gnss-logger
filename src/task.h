//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#ifndef GNSS_LOGGER_TASK_H
#define GNSS_LOGGER_TASK_H

#include <vector>
#include <map>
#include "gnss_logger.h"

namespace task {
    enum MessageType {BOTH, GNSS, ENV, GUIDE, MARK};

    typedef struct {
        time_t datetime;
        uint8_t mode;
        double lat;
        double lng;
        double speed;
        const char *cardinal;
        double altitude;
        uint32_t satellites;
        double hdop;
        std::map<std::string, std::vector<uint8_t>> usedSat;
        //std::vector<uint16_t> usedSat;
    } gnssMessage_t;

    typedef struct {
        float temperature;
        float humidity;
        float pressure;
    } envMessage_t;

    typedef struct {
        MessageType displayType;
        gnssMessage_t gnss;
        envMessage_t env;
        logger::Status status;
    } message_t;

    void setup();

    void sendDisplayMessage(message_t&);
    void sendSaveMessage(message_t&);

    void displayTask(void *);
    void saveTask(void *);
}

#endif //GNSS_LOGGER_TASK_H
