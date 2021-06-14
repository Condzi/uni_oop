#pragma once

#include "basic_menu_state.hpp"

namespace sj
{
class Instructor_Courses_View final : public Basic_Menu_State {
public:
  Instructor_Courses_View( Terminal& terminal_, Database& database_, App& app_,
                           Key index_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  Key const id;
  bool const no_courses;
};
}
