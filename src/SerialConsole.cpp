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
}

size_t SerialConsole::begin() {
    this->serial->print(banner);
    return 0;
}

void SerialConsole::runOnce(size_t timeout) {
    char received;
    Stream *s = this->serial;

    size_t expiration = millis() + timeout;

    while (s->available() > 0 && millis() < expiration) {
        received = s->read();

        if (received == NEWLINE) {
            // check state and handle input appropriately
            char output[this->recvBytes];
            s->println(this->recvBytes);
            this->recvBuffer[this->recvBytes] = '\0';
            s->println(this->recvBuffer);
            strcpy(output, this->recvBuffer);
            s->println(output);
            // reset receive
            this->recvBytes = 0;
        } else {
            this->recvBuffer[this->recvBytes] = received;
            String val(this->recvBytes, 10);
            val.concat(": ");
            val.concat(received);
            s->println(val.c_str());
            this->recvBytes++;
        }
    }
}
