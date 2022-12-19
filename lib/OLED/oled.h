#include <U8g2lib.h>
#include <Wire.h>

class Display {
public:
    void init();
    void SetUnits(char units_str[4]);
    void UpdateDisp(float speed_inp, int sats, float timer);
    void ErrorMsg(const char* message);
private:
    char units[4];
    double speed;
};