#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <button.h>
#include <defines.h>

Display display;
U8G2* disp;
NAV_PVT pvt;

enum states{Accel, Dist, Speed, Menu, ChangeMode, ChangeUnit};
uint8_t state = Accel;
uint8_t active_state = Accel;

short error = 0;
String error_msg;

String units_str = "km/h";
double units_mult = 0.0036;  // multiplication by mm/s to reach desired units

char unit_kmh[] = "km/h";
char* unit_kmh_ptr = unit_kmh;
char unit_mph[] = "mph";
char* unit_mph_ptr = unit_mph;
char unit_ms[] = "m/s";
char* unit_ms_ptr = unit_ms;

Button* menu_btn;

double prev_speed = 0;
double ground_speed = 0;
float print_val = 0;
int interp_count = 0;
unsigned long last_gps_refresh = 0;
uint8 gps_hz = 0;
unsigned long last_disp_refresh = 0;

unsigned long cur_time = 0;

unsigned long debounce_tmr = 0;
uint8_t start_speed = 1;    // currently required to be uint8_t due to u8g2 var function, to be changed to allow higher values
uint8_t end_speed = 101;    // ^
bool acc_started = false;
bool acc_ready = true;
unsigned long acc_start_time;
long timer;



void IRAM_ATTR ISR_MENU() {
  debounce_tmr = (millis() + 20);
}

void switchMenu() {
    if (state == Menu) {
        state = Accel;
    } else {
        state = Menu;
    }
}

void setup() {
    pinMode(12, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(12), ISR_MENU, RISING);
    menu_btn = new Button(12, &debounce_tmr, switchMenu);

    display.init();
    disp = display.GetU8G2();
    Serial.begin(BAUDRATE);
    InitGPS();
    Serial.end();
    Serial.begin(115200);
    
    
    //int start_time = millis();

    /*while (!ProcessGPS(&pvt)) {
        if (millis() - start_time > 5000) {
            state = Error;
            error_msg = "No GPS Data";
            break;
        }
    }
    delay(200);*/
}

void loop() {
    menu_btn->check();

    switch (state) {
        case Accel :
            if (ProcessGPS(&pvt)) {
                error = 0;
                cur_time = millis();
                gps_hz = (cur_time - last_gps_refresh) / 1000;
                last_gps_refresh = cur_time;

                prev_speed = ground_speed;
                ground_speed = pvt.gnd_speed;
                interp_count = 0;
                last_disp_refresh = cur_time;

                if (acc_started) {
                    timer = millis() - acc_start_time;
                    if ((int)(ground_speed * units_mult) > end_speed) {
                        acc_ready = 0;
                        acc_started = 0;
                    } else if ((int)(ground_speed * units_mult) < start_speed) {
                        timer = 0;
                        acc_ready = 1;
                        acc_started = 0;
                    }
                } else if ((acc_ready) && ((int)(ground_speed * units_mult) > start_speed)) {
                    acc_start_time = millis();
                    timer = 0;
                    acc_started = 1;
                    acc_ready = 0;
                } else if ((!acc_ready) && ((int)(ground_speed * units_mult) < start_speed)) {
                    timer = 0;
                    acc_ready = 1;
                }

            } else if ((millis() - last_gps_refresh) > 1000)  {
                error = 1;
                error_msg = "No GPS Data";
            }

            if (((millis() - last_disp_refresh) >= (1000 / DISPLAY_HZ)) && (error == 0)) {
                print_val = Interp(prev_speed, ground_speed, interp_count, INTERP_NUM) * units_mult;
                display.UpdateDisp(print_val, pvt.num_sv, ((float)timer / 1000));
            } else if (error == 1) {
                display.ErrorMsg(error_msg.c_str());
            }
            break;
            
        case Menu :
            detachInterrupt(MENU_PIN);
            debounce_tmr = 0;
            switch (disp->userInterfaceSelectionList("Settings", 1, "Change Mode\nChange Start Speed\nChange End Speed\nChange Distance\nChange Units\nExit")) {
                case 1:
                    state = ChangeMode;
                    break;
                case 2:
                    disp->userInterfaceInputValue("Change Start Speed", "", &start_speed, 0, 244, 3, " V");
                    break;
                case 3:
                    disp->userInterfaceInputValue("Change Start Speed", "", &end_speed, 0, 244, 3, " V");
                    break;
                case 5:
                    switch (disp->userInterfaceSelectionList("Select Units", 1, "km/h\nmph\nm/s")) {
                        case 1:
                            units_str = "km/h";
                            display.SetUnits(unit_kmh_ptr);
                            units_mult = 0.0036;
                            break;
                        case 2:
                            units_str = "mph";
                            display.SetUnits(unit_mph_ptr);
                            units_mult = 0.002236936;
                        case 3:
                            units_str = "m/s";
                            display.SetUnits(unit_ms_ptr);
                            units_mult = 0.001;
                    }
                case 6:
                    state = active_state;
            }
            delay(10);
            attachInterrupt(digitalPinToInterrupt(MENU_PIN), ISR_MENU, RISING);
            break;

        case ChangeMode :
            detachInterrupt(MENU_PIN);
            debounce_tmr = 0;
            disp->clearBuffer();
            state = disp->userInterfaceSelectionList("Select Mode:", 1, "Acceleration\nDistance\nTop Speed")-1;
            active_state = state;
            delay(10);
            attachInterrupt(digitalPinToInterrupt(MENU_PIN), ISR_MENU, RISING);
            //disp->userInterfaceInputValue("Select Voltage", "DAC= ", &state, 0, 5, 1, " V");
    }
}