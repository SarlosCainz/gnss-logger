//
// Created by MATSUNAGA Takuji on 2019-07-15.
//

#ifndef GPS_TFT_TERMINAL_H
#define GPS_TFT_TERMINAL_H

#include <Arduino.h>
#include <M5Stack.h>

void termInit();
void termPutchar(unsigned char data);
void printString(unsigned char *str);
int scroll_line();
void setupScrollArea(uint16_t tfa, uint16_t bfa);
void scrollAddress(uint16_t vsp);

#endif //GPS_TFT_TERMINAL_H
