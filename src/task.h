//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#ifndef GNSS_LOGGER_TASK_H
#define GNSS_LOGGER_TASK_H

namespace task {
    enum DisplayType {Datetime, Location, Mode, Speed, Cardinal, Altitude};

    typedef struct {
        DisplayType displayType;
        union {
            time_t datetime;
            uint8_t mode;
            struct {
                double lat;
                double lng;
            } location;
            double speed;
            const char *cardinal;
            double altitude;
        } data;
    } displayMessage_t;

    typedef struct {

    } saveMessage_t;

    void setup();

    void sendDisplayMessage(displayMessage_t&);
    void sendSaveMessage(saveMessage_t&);

    void displayTask(void *);
    void saveTask(void *);
}

#endif //GNSS_LOGGER_TASK_H
