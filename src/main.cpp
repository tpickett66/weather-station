#include <Arduino.h>
#include <Wire.h>
#include "U8x8lib.h"
#include "../.pio/libdeps/esp32dev/U8g2/src/U8x8lib.h"

#include "SparkFunBME280.h"
#include "../.pio/libdeps/esp32dev/SparkFun BME280/src/SparkFunBME280.h"

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

#define BME_CHIP_ID 0x60
BME280 bme280;
struct BME280_SensorMeasurements measurements;
bool sensor_has_humidity;

void setup() {
    Wire.begin();
    Wire.setClock(400000);

    u8x8.begin();
    u8x8.setFont(u8x8_font_8x13_1x2_f);

//    bme280.setI2CAddress(0x76);
    if (!bme280.beginI2C()) {
        u8x8.println(F("Sensor failed!"));
        while(true);
    };
    bme280.setMode(MODE_SLEEP);
    sensor_has_humidity = bme280.readRegister(BME280_CHIP_ID_REG) == BME_CHIP_ID;

    u8x8.drawUTF8(0, 0, "Temp:      °C");
    u8x8.drawString(0, 2, "  RH:");
    u8x8.drawString(0,4, "Pres:       kPa");
}

void loop() {
    bme280.setMode(MODE_FORCED); //Wake up sensor and take reading
    while(!bme280.isMeasuring()) ; //Wait for sensor to start measurement
    while(bme280.isMeasuring()) ; //Hang out while sensor completes the reading

    bme280.readAllMeasurements(&measurements, 0);

    String temp;

    if (measurements.temperature >= 100 || measurements.temperature < 0) {
        temp = String(measurements.temperature, 1);
    } else {
        temp = String(measurements.temperature, 2);
    }
    u8x8.drawUTF8(6, 0, temp.c_str());

    if (sensor_has_humidity) {
        String humidity = String(measurements.humidity, 0);
        humidity.concat(F("%"));
        u8x8.drawString(6, 2, humidity.c_str());
    }

    float kpa = measurements.pressure/1000;
    String pressure;
    if (kpa >= 100) {
        pressure = String(kpa, 1);
    } else {
        pressure = String(kpa, 2);
    }
    u8x8.drawString(6,4, pressure.c_str());

    delay(10000);
}