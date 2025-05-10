#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "oled.h"

enum struct MenuItemType {
  SubMenu,
  Select,
  CheckBox,
  Number,
};

enum struct ClickDirection {
  Up,
  Right,
  Down,
};

enum struct MenuAction {
  None,
  MoveUp,
  MoveDown,
  MenuBack,
  CloseMenu,
  SubMenu,
};

class IMenuItem {
  std::shared_ptr<Options>
public:
  virtual MenuAction HandleClick(ClickDirection dir) = 0;
  virtual std::string GetLine(uint8_t char_length) = 0;
};

namespace MenuItem {
class SubMenu : IMenuItem {
  const std::string menu_name_;

public:
  SubMenu(std::string menu_name);
  MenuAction HandleClick(ClickDirection dir) override;
  std::string GetLine(uint8_t char_length) override;
};

class Select : IMenuItem {
  const std::string display_string_;
  const std::function<void()> func_;

public:
  Select(std::string display_string, std::function<void()> func);
  MenuAction HandleClick(ClickDirection dir) override;
  std::string GetLine(uint8_t char_length) override;
};

class CheckBox : IMenuItem {
  const std::string display_string_;
  bool selected_;

public:
  CheckBox(std::string display_string);
  MenuAction HandleClick(ClickDirection dir) override;
  std::string GetLine(uint8_t char_length) override;
};

class Number : IMenuItem {
  const std::string display_string_;
  const std::int16_t min_;
  const std::int16_t max_;
  const std::uint16_t step_;
  std::int16_t num_;
  bool capture_cursor_;

public:
  Number(std::string display_string, int16_t num_init, int16_t min, int16_t max, uint16_t step);
  MenuAction HandleClick(ClickDirection dir) override;
  std::string GetLine(uint8_t char_length) override;
};
}

class Menu {
  std::vector<IMenuItem> items;
  uint8_t position;

public:
  void DrawMenu();
};
