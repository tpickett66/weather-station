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

const __FlashStringHelper *const helpText = F("------------------------------\n"
                                              "help  - print this message\n"
                                              "reset - restart the system\n"
                                              "ssid  - edit WiFi SSID\n"
                                              "pass  - edit WiFi Password\n"
                                              "host  - edit WiFi Hostname\n");

const __FlashStringHelper *const retainingText = F("Retaining current value.");

const __FlashStringHelper *const clearingText = F("Clearing stored value.");

enum STATE {
    WAITING_FOR_COMMAND,
    WAITING_FOR_INPUT,
    PROCESSING_COMMAND
};

enum COMMAND {
    HOST,
    NO_COMMAND,
    SSID,
    WIPASS,
};

class SerialConsole {
private:
    WSPreferences *preferences;
    Stream *serial;
    char recvBuffer[255];
    size_t recvBytes;

    STATE state;
    COMMAND command;

    size_t commandReceived();
    size_t handleInput();

public:
    SerialConsole(Stream *s, WSPreferences *prefs);
    size_t begin();
    void runOnce(size_t timeout);
};


#endif //WEATHER_STATION_SERIALCONSOLE_H
