#include <U8g2lib.h>
#include <Wire.h>

class Display {
 public:
  void init();
  U8G2* GetU8G2();
  void SetUnits(char* units_str);
  void UpdateDispAccel(float speed_inp, int sats, float timer, char* units);
  void UpdateDispSpeed(float speed_inp, int sats, float max_speed, char* units);
  void ErrorMsg(const char* message);

 private:
  U8G2* u8g2;
  char* units;
  double speed;
};