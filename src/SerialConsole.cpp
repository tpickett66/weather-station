//
// Created by Tyler Pickett on 10/9/21.
//

#include "SerialConsole.h"


SerialConsole::SerialConsole(Stream *s, WSPreferences *prefs) {
    this->preferences = prefs;
    this->serial = s;

    this->recvBytes = 0;
    this->state = WAITING_FOR_COMMAND;
    this->command = NO_COMMAND;
}

size_t SerialConsole::begin() {
    this->serial->print(banner);
    return 0;
}

void SerialConsole::runOnce(size_t timeout) {
    char received;

    size_t start = millis();

    while (serial->available() > 0 && millis() - start < timeout) {
        received = serial->read();

        if (received == NEWLINE) {
            recvBuffer[recvBytes] = '\0'; // Make it easy to use string functions by making this look more like a string
            // TODO: This whole state machine could be way better make it so.
            if (state == WAITING_FOR_COMMAND) {
                commandReceived();
            } else if (state == WAITING_FOR_INPUT) {
                handleInput();
            }

            recvBytes = 0;
        } else {
            recvBuffer[recvBytes] = received;
            recvBytes++;
        }
    }
}

size_t SerialConsole::commandReceived() {
    char buf[64];
    char message[128];
    size_t len;

    if (strncmp("help", recvBuffer, 5) == 0) {
        serial->println(helpText);
    } else if (strncmp("reset", recvBuffer, 6) == 0) {
        ESP.restart(); // bye bye
    } else if (strncmp("ssid", recvBuffer, 5) == 0) {
        command = SSID;
        state = WAITING_FOR_INPUT;
        len = preferences->wiFiSsidLoad(buf);
        if (len > 0) {
            snprintf(message, len + 16, "Current SSID: '%s'", buf);
            serial->println(message);
        } else {
            serial->println(F("SSID is currently unset."));
        }
        serial->println(
                F("Enter a new SSID, press enter to keep current value or enter a zero (0) to clear the stored value."));
    } else if (strncmp("pass", recvBuffer, 5) == 0) {
        command = WIPASS;
        state = WAITING_FOR_INPUT;
        if (preferences->wiFiPassSet()) {
            serial->println(
                    F("WiFi Password is currently set, enter a new value, press enter to keep current value, enter zero (0) to clear the current value."));
        } else {
            serial->println(
                    F("WiFi Password is currently unset, please enter a value or press enter to leave unset."));
        }
    } else if (strncmp("host", recvBuffer, 5) == 0) {
        command = HOST;
        state = WAITING_FOR_INPUT;
        len = preferences->wiFiHostLoad(buf);
        if (len > 0) {
            snprintf(message, len + 20, "Current Hostname: '%s'", buf);
            serial->println(message);
        } else {
            serial->println(F("Hostname is currently unset."));
        }
        serial->println(
                F("Enter a new Hostname, press enter to keep current value or enter a zero (0) to clear the stored value."));
    } else {
        snprintf(message, 32, "Unknown command '%s'.", recvBuffer);
        serial->println(message);
        serial->println(helpText);
    }
    return 0;
}

size_t SerialConsole::handleInput() {
    char value[128];
    if (recvBytes == 0) {
        serial->println(retainingText);
        command = NO_COMMAND;
        state = WAITING_FOR_COMMAND;
        return 0;
    }

    strncpy(value, recvBuffer, recvBytes + 1);
    switch (command) {
        case NO_COMMAND:
            break;
        case HOST:
            if (strncmp("0", value, 2) == 0) {
                preferences->wiFiHostClear();
                serial->println(clearingText);
            } else {
                preferences->wiFiHostStore(value);
                serial->printf("Saved '%s' as new Hostname.\n", value);
            }
            command = NO_COMMAND;
            state = WAITING_FOR_COMMAND;
            break;
        case SSID:
            if (strncmp("0", value, 2) == 0) {
                preferences->wiFiSsidClear();
                serial->println(clearingText);
            } else {
                preferences->wiFiSsidStore(value);
                serial->printf("Saved '%s' as new SSID.\n", value);
            }
            command = NO_COMMAND;
            state = WAITING_FOR_COMMAND;
            break;
        case WIPASS:
            if (strncmp("0", value, 2) == 0) {
                preferences->wiFiPassClear();
                serial->println(clearingText);
            } else {
                preferences->wiFiPassStore(value);
                serial->println(F("Saving new WiFi Password."));
            }
            command = NO_COMMAND;
            state = WAITING_FOR_COMMAND;
            break;
    }

    return 0;
}
