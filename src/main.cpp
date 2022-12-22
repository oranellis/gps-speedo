#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <defines.h>

Display display;
NAV_PVT pvt;

enum states{Accel, Menu, Error};
enum states state = Accel;

short error = 0;
String error_msg;

double prev_speed = 0;
double ground_speed = 0;
float print_val = 0;
int interp_count = 0;
unsigned long last_gps_refresh = 0;
uint8 gps_hz = 0;
unsigned long last_disp_refresh = 0;

unsigned long cur_time = 0;

int menu = 0;
unsigned long debounce_tmr = 0;
int start_speed = 1;
int end_speed = 101;
bool acc_started = false;
bool acc_ready = true;
unsigned long acc_start_time;
long timer;



void IRAM_ATTR ISR_MENU() {
  debounce_tmr = (millis() + 20);
}

void setup() {
    pinMode(12, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(12), ISR_MENU, FALLING);

    display.init();
    Serial.begin(BAUDRATE);
    InitGPS();
    Serial.end();
    Serial.begin(115200);
    int start_time = millis();

    while (!ProcessGPS(&pvt)) {
        if (millis() - start_time > 5000) {
            state = Error;
            error_msg = "No GPS Data";
            break;
        }
    }
    delay(200);
}

void loop() {
    switch (state) {
        case Accel :
            if (millis() >= debounce_tmr && debounce_tmr != 0) {
                debounce_tmr = 0;
                if (!digitalRead(12)) {
                    if (menu) {
                        menu = 0;
                    } else {
                        menu = 1;
                    }
                }
            }

            /*while (menu) {
                if (!menu) {
                    menu = 0;
                }
            }*/

            if (ProcessGPS(&pvt)) {
                error = 0;
                cur_time = millis();
                gps_hz = (cur_time - last_gps_refresh) / 1000;
                last_gps_refresh = cur_time;

                prev_speed = ground_speed;
                ground_speed = pvt.gnd_speed;
                interp_count = 0;
                last_disp_refresh = cur_time;

                /*if (acc_started) {
                    timer = millis() - acc_start_time;
                    if ((int)(ground_speed * 0.0036) > end_speed) {
                        acc_ready = 0;
                        acc_started = 0;
                    } else if ((int)(ground_speed * 0.0036) < start_speed) {
                        timer = 0;
                        acc_ready = 1;
                        acc_started = 0;
                    }
                } else if ((acc_ready) && ((int)(ground_speed * 0.0036) > start_speed)) {
                    acc_start_time = millis();
                    timer = 0;
                    acc_started = 1;
                    acc_ready = 0;
                } else if ((!acc_ready) && ((int)(ground_speed * 0.0036) < start_speed)) {
                    timer = 0;
                    acc_ready = 1;
                } */

            } else if ((millis() - last_gps_refresh) > 1000)  {
                error = 1;
                error_msg = "No GPS Data";
            }

            if (((millis() - last_disp_refresh) >= (1000 / DISPLAY_HZ)) && (error == 0)) {
                print_val = Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * 0.0036;
                display.UpdateDisp(print_val, pvt.num_sv, ((float)timer / 1000));
            } else if (error == 1) {
                display.ErrorMsg(error_msg.c_str());
            }
            break;
        default :
            while (1) {
                ;;
            }
    }
}