#pragma once

#include "pch.hpp"

namespace sj 
{
class Terminal final {
public:
  enum class Input_Type {
    Cursor_Up = 0,
    Cursor_Down,
    Enter,
    Escape
  };

  Terminal();
  Terminal( Terminal const& ) = delete;
  Terminal( Terminal&& ) = delete;
  Terminal& operator=(Terminal const&) = delete;
  Terminal&& operator=(Terminal&&) = delete;

  void set_title( std::string const& title );
  void set_pen_position( s32 x, s32 y );

  [[nodiscard]]
  s32 get_pen_x() const;

  [[nodiscard]]
  s32 get_pen_y() const;

  [[nodiscard]]
  bool is_key_pressed( Input_Type type );

  void pen_write( std::string const& text );
  void clear_screen();
  
  void update();

private:
  void* input;
  void* output;

  s32 width;
  s32 height;

  struct {
    s32 x = 0, y = 0;
  } pen_position;

  bool input_state[4];

  void update_dimensions();
  void update_input_state();
};
}