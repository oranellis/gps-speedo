#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <defines.h>

Display display;
NAV_PVT pvt;

double prev_speed = 0;
double ground_speed = 0;
int print_val = 0;
int interp_count = 0;
uint16 last_gps_refresh = 0;
uint8 gps_hz = 0;
uint16 last_disp_refresh = 0;


float round1(float num) {
    return ((float)((int)(num * 10)))/10;
}

void setup() {
    Serial.begin(BAUDRATE);
    display.init();
}

void loop() {
    if (ProcessGPS(pvt)) {
        unsigned long cur_time = millis();
        gps_hz = (last_gps_refresh - cur_time) / 1000;
        last_gps_refresh = cur_time;

        prev_speed = ground_speed;
        ground_speed = pvt.gnd_speed;
        interp_count = 0;
        //display.SetSpeed(Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * 0.0036);
        last_disp_refresh = cur_time;
    }

    if ((last_disp_refresh - millis()) >= (1000 / DISPLAY_HZ)) {
        print_val = Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * 0.0036;
        //display.SetSpeed(print_val);
        display.SetSpeed(112.45);
    }
}