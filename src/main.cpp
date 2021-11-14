#include <cstdio>

#include <Arduino.h>
//#include <ESPmDNS.h>
//#include <PubSubClient.h>
#include <SparkFunBME280.h>
#include <U8x8lib.h>
#include <WiFi.h>
//#include <WiFiClientSecure.h>
#include <Wire.h>

#include "main.h"
#include "SerialConsole.h"
#include "WSPreferences.h"

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

#define BME_CHIP_ID 0x60
BME280 bme280;
struct BME280_SensorMeasurements measurements;
struct ReadingPackage readings;
bool sensor_has_humidity;

size_t millisBetweenReads = 10000;
size_t lastReadMillis = -20000; // Initialize the last reading to be in the past, so we get a reading right away.
unsigned int readCount = 0;

WSPreferences preferences;
SerialConsole console(&Serial, &preferences);
int displayReadings(ReadingPackage *, unsigned int);
int takeReadings(ReadingPackage *);

void setup() {
    preferences.begin();
    Serial.begin(115200);
    console.begin();

    WiFi.setAutoReconnect(true);
    if (preferences.wiFiSsidSet() && preferences.wiFiPassSet()) {
        size_t waits = 0;
        char ssid[33], pass[64];
        preferences.wiFiSsidLoad(ssid);
        preferences.wiFiPassLoad(pass);

        char hostname[64];
        preferences.wiFiHostLoad(hostname);
        WiFi.setHostname(hostname);

        Serial.print("Connecting to WiFi network '");
        Serial.print(ssid);
        Serial.print("' as '");
        Serial.print(hostname);
        Serial.print("' ..");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);

        while (WiFi.status() != WL_CONNECTED && waits < 20) {
            Serial.print('.');
            delay(500);
            waits++;
        }
        if (WiFi.status() != WL_CONNECTED) {
            Serial.printf("Connecting to WiFi failed (%d).\n", WiFi.status());
            while (true);
        }

        Serial.println(WiFi.localIP());
    }

    Wire.begin();
    Wire.setClock(400000);

    // TODO: Scan I2C bus and determine what is plugged in.
    u8x8.begin();
    u8x8.setFont(u8x8_font_8x13_1x2_f);

//    bme280.setI2CAddress(0x76);
    if (!bme280.beginI2C()) {
        u8x8.println(F("Sensor failed!"));
        while(true);
    };
    bme280.setMode(MODE_SLEEP);
    // TODO: Remove this silly flag once we're using a dedicated temperature and humidity sensor.
    sensor_has_humidity = bme280.readRegister(BME280_CHIP_ID_REG) == BME_CHIP_ID;

    u8x8.drawUTF8(0, 0, "Temp:      °C");
    u8x8.drawString(2, 2, "RH:");
    u8x8.drawString(0,4, "Pres:       kPa");
    u8x8.drawString(1,6, "Cnt:");
}

void loop() {
    unsigned long now = millis();
    if (now - lastReadMillis >= millisBetweenReads) {
        takeReadings(&readings);
        readCount++;
        displayReadings(&readings, readCount);
        lastReadMillis = now;
    }
    console.runOnce(100);
}

int displayReadings(ReadingPackage * package, unsigned int readingsTaken) {
    String temp;
    if (package->temperature >= 100 || package->temperature < 0) {
        temp = String(package->temperature, 1);
    } else {
        temp = String(package->temperature, 2);
    }
    u8x8.drawUTF8(6, 0, temp.c_str());

    if (sensor_has_humidity) {
        String humidity = String(package->humidity, 0);
        humidity.concat(F("%"));
        u8x8.drawString(6, 2, humidity.c_str());
    }

    float kpa = package->pressure/1000;
    String pressure;
    if (kpa >= 100) {
        pressure = String(kpa, 1);
    } else {
        pressure = String(kpa, 2);
    }
    u8x8.drawString(6,4, pressure.c_str());

    char count[20];
    sprintf(count, "%d", readingsTaken);
    u8x8.drawString(6, 6, count);
    
    return 0;
}

int takeReadings(ReadingPackage * package) {
    bme280.setMode(MODE_FORCED); //Wake up sensor and take reading
    while(!bme280.isMeasuring()) ; //Wait for sensor to start measurement
    while(bme280.isMeasuring()) ; //Hang out while sensor completes the reading

    bme280.readAllMeasurements(&measurements, 0);
    package->temperature = measurements.temperature;
    package->pressure = measurements.pressure;
    
    // TODO: Remove this silly flag once we're using a dedicated temperature and humidity sensor.
    if (sensor_has_humidity) {
        package->humidity = measurements.humidity;
    } else {
        package->humidity = -1.0;
    }
    
    return 0;
}
