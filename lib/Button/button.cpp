#include "button.h"

void IRAM_ATTR Button::isr_func() {
    debounce_tmr = (millis() + 20);
}

Button::Button(int pin, void (*btn_func) (void)) {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), isr_func, FALLING);
    func = btn_func;
}