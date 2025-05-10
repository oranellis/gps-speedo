#pragma once

#include <functional>
#include <string>

#include "gs_options.h"

struct NumberMenuItem {
  std::function<void()> action_;
  std::string display_text_;
}

class GSMenu {
  
};
