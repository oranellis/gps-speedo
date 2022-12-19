#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <defines.h>

Display display;
NAV_PVT pvt;

double prev_speed = 0;
double ground_speed = 0;
float print_val = 0;
int interp_count = 0;
uint16 last_gps_refresh = 0;
uint8 gps_hz = 0;
uint16 last_disp_refresh = 0;

unsigned long cur_time = 0;


float round1(float num) {
    return ((float)((int)(num * 10)))/10;
}

void setup() {
    Serial.begin(BAUDRATE);
    display.init();
    InitGPS();
    int start_time = millis();

    while (!ProcessGPS(&pvt)) {
        if (millis() - start_time > 5000) {
            display.NoMessages();
        }
    }
    delay(500);
}

void loop() {
    if (ProcessGPS(&pvt)) {
        cur_time = millis();
        gps_hz = (cur_time - last_gps_refresh) / 1000;
        last_gps_refresh = cur_time;

        prev_speed = ground_speed;
        ground_speed = pvt.gnd_speed;
        interp_count = 0;
        last_disp_refresh = cur_time;
    } else if ((millis() - last_gps_refresh) > 1000)  {
        while (!ProcessGPS(&pvt)) {
            display.NoMessages();
        }
        cur_time = millis();
        gps_hz = (cur_time - last_gps_refresh) / 1000;
        last_gps_refresh = cur_time;

        prev_speed = ground_speed;
        ground_speed = pvt.gnd_speed;
        interp_count = 0;
        last_disp_refresh = cur_time;
    }

    if ((millis() - last_disp_refresh) >= (1000 / DISPLAY_HZ)) {
        print_val = Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * 0.0036;
        display.SetSpeed(print_val, pvt.num_sv);
    }
}