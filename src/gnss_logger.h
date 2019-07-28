//
// Created by MATSUNAGA Takuji on 2019-07-27.
//

#ifndef GNSS_LOGGER_GNSS_LOGGER_H
#define GNSS_LOGGER_GNSS_LOGGER_H


namespace logger {
    enum Status {
        IDLE, SAVE_TIME_READY, SAVE_TIME, SAVE_TIME_CONFIRM, SAVE_DISTANCE_READY, SAVE_DISTANCE, SAVE_DISTANCE_CONFIRM
    };

    Status getStatus();
}

#endif //GNSS_LOGGER_GNSS_LOGGER_H
