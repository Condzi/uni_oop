#pragma once

#include "basic_menu_state.hpp"

namespace sj
{
class Instructor_Overview final : public Basic_Menu_State {
public:
  Instructor_Overview( Terminal& terminal_, Database& database_, App& app_, 
                   Key id_ );
  
  void on_switch() override;

  [[nodiscard]]
  State* update() override;

private:
  Instructor instructor;
  Key id;
};
}