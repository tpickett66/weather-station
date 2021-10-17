//
// Created by Tyler Pickett on 10/9/21.
//

#include "SerialConsole.h"
/*
 * Commands:
 * h - help: print current contextual help
 * s - ssid: Set SSID
 * m - mqtt: configure MQTT (CA, Cert, Key)
 */

SerialConsole::SerialConsole(Stream *s, WSPreferences *prefs) {
    this->preferences = prefs;
    this->serial = s;

    this->recvBytes = 0;
    this->state = WAITING_FOR_COMMAND;
}

size_t SerialConsole::begin() {
    this->serial->print(banner);
    return 0;
}

void SerialConsole::runOnce(size_t timeout) {
    char received;

    size_t expiration = millis() + timeout;

    while (serial->available() > 0 && millis() < expiration) {
        received = serial->read();

        if (received == NEWLINE) {
            recvBuffer[recvBytes] = '\0'; // Make it easy to use string functions by making this look more like a string
            if (state == WAITING_FOR_COMMAND) {
                commandReceived();
            } else if (state == WAITING_FOR_INPUT) {
                // handleInput();
            }

            recvBytes = 0;
        } else {
            recvBuffer[recvBytes] = received;
            recvBytes++;
        }
    }
}

size_t SerialConsole::commandReceived() {
    if (strcmp("help", recvBuffer) == 0) {
        serial->println(helpText);
    } else if (strcmp("reset", recvBuffer) == 0) {
        ESP.restart(); // bye bye
    } else {
        char *unknownMessage;
        unknownMessage = (char *)malloc(32);
        snprintf(unknownMessage, 32, "Unknown command '%s'.", recvBuffer);
        serial->println(unknownMessage);
        serial->println(helpText);
        free(unknownMessage);
    }
    return 0;
}
