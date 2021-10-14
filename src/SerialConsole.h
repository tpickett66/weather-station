//
// Created by Tyler Pickett on 10/9/21.
//

#ifndef WEATHER_STATION_SERIALCONSOLE_H
#define WEATHER_STATION_SERIALCONSOLE_H

#include <Stream.h>

#include "WSPreferences.h"

#define NEWLINE '\n'

const __FlashStringHelper *const banner = F("              .\n\n"
                      "              |\n"
                      "     .               /\n"
                      "      \\       I\n"
                      "                  /\n"
                      "        \\  ,g88R_\n"
                      "          d888(`  ).\n"
                      " -  --==  888(     ).=--\n"
                      "          Y8P(       '`.\n"
                      "        .+(`(      .   )\n"
                      "       ((    (..__.:'-'\n"
                      "       `(       ) )\n"
                      "         ` __.:'   )\n"
                      "                --'\n"
                      "=============================\n"
                      "Weather Station welcomes you!\n");

class SerialConsole {
private:
    WSPreferences *preferences;
    Stream *serial;
    char recvBuffer[255];
    size_t recvBytes;

public:
    SerialConsole(Stream *s, WSPreferences *prefs);
    size_t begin();
    void runOnce(size_t timeout);
};


#endif //WEATHER_STATION_SERIALCONSOLE_H
