#pragma once

#include <cstdint>

enum struct DisplayUnits {
  kmph,
  mph,
  mps,
};

enum struct State {
  Accel,
  Distance,
  Speed,
  Options,
};

struct Options {
  uint32_t debounce_tmr = 0;
  uint16_t start_speed = 1;
  uint16_t end_speed = 101;
  DisplayUnits display_unit_ = DisplayUnits::kmph;
  State state_ = State::Accel;
};
