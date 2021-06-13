#pragma once

#include "ui/states/basic_menu_state.hpp"

namespace sj
{
class User_Selection final : public Basic_Menu_State {
public:
  User_Selection( Terminal& terminal_, Database& database_, App& app_ );

  void on_switch() override;

  [[nodiscard]]
  State* update() override;

private:
  [[nodiscard]]
  bool check_if_student_exists( Key index );

  [[nodiscard]]
  bool check_if_instructor_exists( Key id );
};
}
