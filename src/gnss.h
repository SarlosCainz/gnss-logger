//
// Created by MATSUNAGA Takuji on 2019-07-21.
//

#ifndef GNSS_LOGGER_GNSS_H
#define GNSS_LOGGER_GNSS_H

#include "gnss_logger.h"

namespace gnss {
    const time_t TimeOffset = 9 * 3600;
    const uint8_t MaxSatellite = 12;

    void setup();
    void update(logger::Status);

    void setDistance(double);
}
#endif //GNSS_LOGGER_GNSS_H
