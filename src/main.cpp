#include <gps.h>
#include <oled.h>
#include <interp.h>
#include <button.h>
#include <defines.h>

Display display;
U8G2* disp;
NAV_PVT pvt;

enum states{Accel, Menu, Error, ChangeVar};
uint8_t state = Accel;

short error = 0;
String error_msg;

Button* menu_btn;

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
        case Error :
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
            
        case Menu :
            debounce_tmr = 0;
            detachInterrupt(MENU_PIN);
            /*disp->setFont(u8g2_font_profont17_tr);
            disp->setCursor(0, 12);
            disp->print("HELLO");
            disp->setCursor(0, 25);
            disp->print("HELLO");
            disp->setCursor(0, 38);
            disp->print("HELLO");
            disp->setCursor(0, 51);
            disp->print("HELLO");
            disp->setCursor(0, 64);
            disp->print("HELLO");*/
            if (disp->userInterfaceSelectionList("Title", 1, "Exit\nVar1\nRefresh\nDUCK\nlol\nfkldsjf\nx3\nCancel") == 1) {
                state = Accel;
            } else {
                state = ChangeVar;
            }
            attachInterrupt(digitalPinToInterrupt(MENU_PIN), ISR_MENU, RISING);
            break;

        case ChangeVar :
            debounce_tmr = 0;
            detachInterrupt(MENU_PIN);
            disp->clearBuffer();
            state = disp->userInterfaceSelectionList("Select Mode:", 1, "Accel\nMenu\nError\nChangeVar")-1;
            attachInterrupt(digitalPinToInterrupt(MENU_PIN), ISR_MENU, RISING);
            //disp->userInterfaceInputValue("Select Voltage", "DAC= ", &state, 0, 5, 1, " V");
            //disp->sendBuffer();
    }
}