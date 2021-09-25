#include <Arduino.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "../.pio/libdeps/esp32dev/U8g2/src/U8x8lib.h"

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void setup() {
    u8x8.begin();
    u8x8.setFont(u8x8_font_8x13_1x2_f);
}

void loop() {
    //
}