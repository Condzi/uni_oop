#pragma once

#include "ui/states/basic_menu_state.hpp"

namespace sj
{
class Instructor_Students_View final : public Basic_Menu_State {
public:
  Instructor_Students_View( Terminal& terminal_, Database& database_, App& app_,
                            Key id_, Key course_id_ );

  void on_switch() override;
  [[nodiscard]]
  State* update() override;

private:
  Key const instructor_id;
  Key const course_id;
  bool const no_students;

  std::vector<Key> students_ids;
};
}