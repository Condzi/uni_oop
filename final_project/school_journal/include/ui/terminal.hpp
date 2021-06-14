#pragma once

#include "pch.hpp"

namespace sj 
{
class Terminal final {
public:
  enum class Input {
    Cursor_Up = 0,
    Cursor_Down,
    Enter,
    Escape
  };

  Terminal();
  Terminal( Terminal const& ) = delete;
  Terminal( Terminal&& ) = delete;
  Terminal& operator=( Terminal const& ) = delete;
  Terminal&& operator=( Terminal&& ) = delete;

  void set_title( std::string const& title );
  void set_pen_coords( Coords coords );
  void move_pen( Coords dt );

  [[nodiscard]]
  Coords get_pen_coords() const;

  [[nodiscard]]
  bool is_key_pressed( Input type ) const;

  void pen_write( std::string const& text ) const;
  void clear_screen() const;

  void wait_for_enter();
  void reset_input_state();
  
  void update();

private:
  void* input;
  void* output;

  s32 width;
  s32 height;

  Coords pen_coords;

  bool input_state[4];

  void update_dimensions();
  void update_input_state();
};
}