//
// Created by Tyler Pickett on 10/9/21.
//

#include "WSPreferences.h"

bool WSPreferences::begin() {
    if (!preferences.begin(PREFERENCES_NAMESPACE, false)) {
        log_e("Initializing inner preferences object failed!");
    };

    return true;
}

/* SSID methods */
bool WSPreferences::wiFiSsidSet() {
    return preferences.isKey(SSID_KEY);
}

size_t WSPreferences::wiFiSsidStore(char *ssid) {
    auto bytesWritten = preferences.putString(SSID_KEY, ssid);
    return bytesWritten;
}

size_t WSPreferences::wiFiSsidLoad(char *buf) {
    if (wiFiSsidSet()) {
        return preferences.getString(SSID_KEY, buf, 32);
    } else {
        return 0;
    }
}

bool WSPreferences::wiFiSsidClear() {
    return preferences.remove(SSID_KEY);
}
/* SSID methods */

/* WiFi Password methods */
bool WSPreferences::wiFiPassSet() {
    return preferences.isKey(PASS_KEY);
}

size_t WSPreferences::wiFiPassStore(char *pass) {
    return preferences.putString(PASS_KEY, pass);
}

size_t WSPreferences::wiFiPassLoad(char *buf){
    if (wiFiPassSet()) {
        return preferences.getString(PASS_KEY, buf, 63);
    } else {
        return 0;
    }
}

bool WSPreferences::wiFiPassClear() {
    return preferences.remove(PASS_KEY);
}
/* WiFi Password methods */
