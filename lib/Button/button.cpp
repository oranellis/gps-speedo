#include "button.h"

Button::Button(int pin, unsigned long* debounce_tmr_var, void (*btn_func) ()) {
    pin_num = pin;
    debounce_tmr = debounce_tmr_var;
    func = btn_func;
}

void Button::check() {
    if (millis() >= *debounce_tmr && *debounce_tmr != 0) {
        *debounce_tmr = 0;
        if (digitalRead(pin_num)) {
            func();
        }
    }
}

void Button::setFunc(void (*btn_func) ()) {
    func = btn_func;
}