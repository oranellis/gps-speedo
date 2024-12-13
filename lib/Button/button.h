#include <Arduino.h>

class Button {
 public:
  Button(int pin, unsigned long* debounce_tmr_var, void (*btn_func)());
  void check();
  void setFunc(void (*btn_func)());

 private:
  void (*func)();
  int pin_num;
  unsigned long* debounce_tmr;
  bool active;
};