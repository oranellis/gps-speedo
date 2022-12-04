#include <U8g2lib.h>
#include <Wire.h>

class Display {
public:
    void init();
    void SetUnits(char units_str[4]);
    void SetSpeed(float speed_inp);
private:
    char units[4];
    double speed;
};