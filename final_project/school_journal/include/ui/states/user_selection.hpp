#pragma once

#include "ui/state.hpp"

namespace sj
{
class User_Selection final : public State {
public:
  User_Selection( Terminal& terminal_, Database& database_, App& app_ );

  void on_switch() override;

  [[nodiscard]]
  std::optional<State::Type> update() override;

private:
  bool cursor_needs_redrawing = true;
  s32 selected_option = 0;
  struct {
    s32 x = 4;
    s32 y = 4;
  } const cursor_start_pos;

  [[nodiscard]]
  bool try_to_set_student( Key index );

  [[nodiscard]]
  bool try_to_set_instructor( Key id );

  void handle_up_down_input();

  void redraw_cursor();
};
}
