#include <Arduino.h>

class Button {
public:
    Button(int pin, void (*btn_func) (void));
    void check();
private:
    static void IRAM_ATTR isr_func();
    void* func;
    long debounce_tmr;
    bool active;
};