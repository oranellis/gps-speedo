#include <gps.h>
#include <oled.h>
#include <defines.h>


NAV_PVT pvt;
int ground_speed;
uint16 last_gps_refresh = 0;
uint8 gps_hz;

void setup() {
    Serial.begin(BAUDRATE);
}

void loop() {
    if (ProcessGPS(pvt)) {
        gps_hz = (last_gps_refresh - millis()) / 1000;
        ground_speed = pvt.gnd_speed;
        DisplayNum(ground_speed);
    }

}