//
// Created by Tyler Pickett on 10/9/21.
//

#ifndef WEATHER_STATION_WSPREFERENCES_H
#define WEATHER_STATION_WSPREFERENCES_H

#include <Arduino.h>
#include <Preferences.h>

#define PREFERENCES_NAMESPACE "weather-station"

#define SSID_KEY "ssid"
#define PASS_KEY "wpass"
#define HOST_KEY "host"
#define MQTT_HOST_KEY "mqtt"

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

    /**
     * Clear the currently stored SSID
     * @return Whether or not the SSID was successfully cleared.
     */
    bool wiFiSsidClear();

    bool wiFiPassSet();
    size_t wiFiPassStore(char *pass);
    size_t wiFiPassLoad(char *buf);
    bool wiFiPassClear();
    
    bool wiFiHostSet();
    size_t wiFiHostStore(char *host);
    size_t wiFiHostLoad(char *buf);
    bool wiFiHostClear();
    
    bool mqttHostSet();
    size_t mqttHostStore(char *host);
    size_t mqttHostLoad(char *buf);
    bool mqttHostClear();
};


#endif //WEATHER_STATION_WSPREFERENCES_H
