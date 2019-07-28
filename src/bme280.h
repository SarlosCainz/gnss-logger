//
// Created by MATSUNAGA Takuji on 2019-07-27.
//

#ifndef GNSS_LOGGER_BME280_H
#define GNSS_LOGGER_BME280_H

namespace bme280 {
    void setup(void);
    void update(void);
    void createTaskMessage(task::message_t &);
}

#endif //GNSS_LOGGER_BME280_H
