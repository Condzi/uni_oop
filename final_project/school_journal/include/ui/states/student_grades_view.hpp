#pragma once

#include "basic_menu_state.hpp"

namespace sj
{
class Student_Grades_View final : public Basic_Menu_State {
public:
  Student_Grades_View( Terminal& terminal_, Database& database_, App& app_,
                       Key index_, Key course_id_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  Key const index;
  Key const course_id;
  bool const no_grades;
};
}
