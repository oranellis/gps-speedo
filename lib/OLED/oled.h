#include <U8g2lib.h>
#include <Wire.h>

#define STR_MAX_LEN 64

class Display {
 public:
  void init();
  U8G2* GetU8G2();
  void SetUnits(char* units_str);
  void UpdateDispAccel(float speed_inp, int sats, float timer, char* units);
  void UpdateDispSpeed(float speed_inp, int sats, float max_speed, char* units);
  void ErrorMsg(const char* message);
  void Clear();
  void Init(const char *text);
  /// @brief Prints a line of text to the screen and increments the y_pos, does
  /// not send the buffer
  /// @param y_pos Base line y position for the text, is incrememted by the text
  /// height after draw
  /// @param format The text with format specifiers
  /// @param ... The format matches
  void PrintLine(unsigned short *y_pos, const char *format, ...);
  /// @brief Prints a line of text to the screen, does not send the buffer
  /// @param y_pos Base line y position for the text
  /// @param format The text with format specifiers
  /// @param ... The format matches
  void PrintLine(unsigned short y_pos, const char *format, ...);
  void Render();

 private:
  U8G2* u8g2;
  char* units;
  double speed;
};
