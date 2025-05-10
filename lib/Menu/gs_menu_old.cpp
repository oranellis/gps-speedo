#include "gs_menu.h"

MenuItem::SubMenu::SubMenu(std::string menu_name) : menu_name_(menu_name) {}

MenuAction MenuItem::SubMenu::HandleClick(ClickDirection dir) {
  switch (dir) {
    case ClickDirection::Up:
      return MenuAction::MoveUp;
    case ClickDirection::Down:
      return MenuAction::MoveDown;
    case ClickDirection::Right:
      // Trigger menu to change to
      return MenuAction::SubMenu;
  }
}

std::string MenuItem::SubMenu::GetLine(uint8_t char_length) {

}

MenuItem::Select::Select(std::string display_string, std::function<void()> func) : display_string_(display_string), func_(func) {}

MenuAction MenuItem::Select::HandleClick(ClickDirection dir) {
  switch (dir) {
    case ClickDirection::Up:
      return MenuAction::MoveUp;
    case ClickDirection::Down:
      return MenuAction::MoveDown;
  }
}

std::string MenuItem::Select::GetLine(uint8_t char_length) {

}

MenuItem::CheckBox::CheckBox(std::string display_string) {}

MenuAction MenuItem::CheckBox::HandleClick(ClickDirection dir) {
  switch (dir) {
    case ClickDirection::Up:
      return MenuAction::MoveUp;
    case ClickDirection::Down:
      return MenuAction::MoveDown;
  }
}

std::string MenuItem::CheckBox::GetLine(uint8_t char_length) {

}

MenuItem::Number::Number(std::string display_string, int16_t num_init, int16_t min, int16_t max, uint16_t step)
  : display_string_(display_string),
    num_(num_init),
    min_(min),
    max_(max),
    step_(step)
{}

MenuAction MenuItem::Number::HandleClick(ClickDirection dir) {

}

std::string MenuItem::Number::GetLine(uint8_t char_length) {

}
