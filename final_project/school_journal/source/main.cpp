#include "pch.hpp"

#include "ui/terminal.hpp"

using namespace sj;
int main() {
  using Input = Terminal::Input_Type;

  bool run = true;
  Terminal terminal;
  s32 x = 0, y = 0;
  bool moved = false;

  while( run ) {
    if( terminal.is_key_pressed( Input::Cursor_Up ) ) {
      y--;
      moved = true;
    }

    if( terminal.is_key_pressed( Input::Cursor_Down ) ) {
      y++;
      moved = true;
    }

    if( terminal.is_key_pressed( Input::Enter ) ) {
      x++;
      moved = true;
    }
    if( terminal.is_key_pressed( Input::Escape ) ) {
      x--;
      moved = true;
    }

    terminal.update();

    if( moved ) {
      terminal.pen_write( " " );
      terminal.set_pen_position( x, y );
      terminal.pen_write( "X" );
      moved = false;
    }
  }
}
