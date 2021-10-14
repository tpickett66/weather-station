//
// Created by Tyler Pickett on 10/9/21.
//

#include "WSPreferences.h"

bool WSPreferences::begin() {
    Preferences p;
    p.begin(PREFERENCES_NAMESPACE, false);

    this->preferences = p;
    return true;
}

bool WSPreferences::wiFiSsidSet() {
    return this->preferences.isKey(SSID_KEY);
}

size_t WSPreferences::wiFiSsidStore(char *ssid) {
    auto bytesWritten = this->preferences.putString(SSID_KEY, ssid);
    return bytesWritten > 0;
}

size_t WSPreferences::wiFiSsidLoad(char *buf) {
    return this->preferences.getString(SSID_KEY, buf, 32);
}

bool WSPreferences::wiFiPassSet() {
    return this->preferences.isKey(PASS_KEY);
}

size_t WSPreferences::wiFiPassStore(char *pass) {
    auto bytesWritten = this->preferences.putString(PASS_KEY, pass);
    return bytesWritten > 0;
}

size_t WSPreferences::wiFiPassLoad(char *buf){
    return this->preferences.getString(PASS_KEY, buf, 63);
}
