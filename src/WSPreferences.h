//
// Created by Tyler Pickett on 10/9/21.
//

#ifndef WEATHER_STATION_WSPREFERENCES_H
#define WEATHER_STATION_WSPREFERENCES_H

#include <Arduino.h>
#include <Preferences.h>

#define PREFERENCES_NAMESPACE "weather-station"
#define NOT_SET "not-set"

#define SSID_KEY "ssid"
#define PASS_KEY "wifipass"

class WSPreferences {
private:
    Preferences preferences;

public:
    bool begin();

    /*
     * Accessors Needed:
     * Network Hostname (string)
     * CA (string)
     * Client Cert (String)
     * Private Key (String)
     */

    /**
     * Check if an SSID has been set
     *
     * @return bool Whether or not an SSID has been stored
     */
    bool wiFiSsidSet();

    /**
     * Save SSID for later use.
     * @param ssid The character data to save
     * @return The number of bytes written to storage
     */
    size_t wiFiSsidStore(char *ssid);

    /**
     * Retrieve the saved SSID
     * @param buf A buffer to write the SSID into, should be 32 bytes since that's how long an SSID can be.
     * @return The number of bytes loaded into buf
     */
    size_t wiFiSsidLoad(char *buf);

    bool wiFiPassSet();
    size_t wiFiPassStore(char *pass);
    size_t wiFiPassLoad(char *buf);
};


#endif //WEATHER_STATION_WSPREFERENCES_H
