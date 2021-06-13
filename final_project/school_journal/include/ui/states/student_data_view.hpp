#pragma once

#include "basic_menu_state.hpp"

namespace sj
{
class Student_Data_View final : public Basic_Menu_State {
public:
  Student_Data_View( Terminal& terminal_, Database& database_, App& app_,
                     Key index_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  Key const index;
  Student student;
};
}