#pragma once

#include "ui/states/basic_menu_state.hpp"

namespace sj
{
class Instructor_Student_Grades_View final : public Basic_Menu_State {
public:
  Instructor_Student_Grades_View( Terminal& terminal_, 
                                  Database& database_, App& app_,
                                  Key id_, Key course_id_, 
                                  Key student_id_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  std::vector<Key> grades_ids;

  Key const instructor_id;
  Key const course_id;
  Key const student_id;
  bool const no_grades;

  [[nodiscard]]
  bool add_grade();
};
}