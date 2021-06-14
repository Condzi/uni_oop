#pragma once

#include "basic_menu_state.hpp"

namespace sj
{
class Enrollments_Courses_List final : public Basic_Menu_State {
public:
  Enrollments_Courses_List( Terminal& terminal_, Database& database_, App& app_, 
                            Key index_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  Key const index;

  [[nodiscard]]
  bool is_course_already_enrolled( Key course_id ) const;
};
}