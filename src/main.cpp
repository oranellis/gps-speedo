#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <defines.h>


NAV_PVT pvt;

double prev_speed = 0;
double ground_speed = 0;
int print_val = 0;
int interp_count = 0;
uint16 last_gps_refresh = 0;
uint8 gps_hz = 0;
uint16 last_disp_refresh = 0;
uint8 disp_hz = 0;
Display display;


void setup() {
    Serial.begin(BAUDRATE);
    display.init();
}

void loop() {
    if (ProcessGPS(pvt)) {
        gps_hz = (last_gps_refresh - millis()) / 1000;

        prev_speed = ground_speed;
        ground_speed = pvt.gnd_speed;
        interp_count = 0;
    }

    print_val = Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * 0.0036;
    display.SetSpeed(print_val);
}