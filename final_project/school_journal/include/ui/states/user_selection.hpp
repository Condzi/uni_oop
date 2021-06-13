#pragma once

#include "ui/state.hpp"

namespace sj
{
class User_Selection final : public State {
public:
  User_Selection( Terminal& terminal_, Database& database_ );

  void on_switch() override;
  [[nodiscard]]
  std::optional<State::Type> update() override;

private:
  s32 selected_option = 0;
  struct {
    s32 x = 4;
    s32 y = 3;
  } const cursor_start_pos;
};
}
