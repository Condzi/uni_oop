#include "pch.hpp"

#include "ui/terminal.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace sj
{
Terminal::Terminal() :
  input( GetStdHandle( STD_INPUT_HANDLE ) ),
  output( GetStdHandle( STD_OUTPUT_HANDLE ) )
{
  // Set text color to bright white
  std::cout << "\x1B[97m";
  // Set background color to black
  std::cout << "\x1B[40m";

  update_dimensions();
  set_title( "Title" );
  clear_screen();
}

void Terminal::set_title( std::string const& title ) {
  SetConsoleTitleA( title.c_str() );
}

void Terminal::set_pen_coords( Coords coords ) {
  pen_coords.x = std::clamp( coords.x, 0, width );
  pen_coords.y = std::clamp( coords.y, 0, height );
}

void Terminal::move_pen( Coords dt ) {
  dt.x += pen_coords.x;
  dt.y += pen_coords.y;
  set_pen_coords( dt );
}

Coords Terminal::get_pen_coords() const {
  return pen_coords;
}

bool Terminal::is_key_pressed( Input type ) const {
  return input_state[static_cast<s32>( type )];
}

void Terminal::pen_write( std::string const& text ) const {
  std::cout << "\x1B[" << pen_coords.y << ";" << pen_coords.x << "H";
  std::cout << text;
}

void Terminal::clear_screen() const {
  system( "cls" );
}

void Terminal::wait_for_enter() const {
  std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
  std::cin.get();
}

void Terminal::reset_input_state() {
  for( auto& b : input_state ) {
    b = false;
  }
}

void Terminal::update() {
  update_dimensions();
  update_input_state();
}

// Input and dimensions code is based on
// https://github.com/cmuratori/termbench/blob/main/termbench.cpp

void Terminal::update_dimensions() {
  CONSOLE_SCREEN_BUFFER_INFO terminal;
  GetConsoleScreenBufferInfo( output, &terminal);
  width  = terminal.srWindow.Right  - terminal.srWindow.Left;
  height = terminal.srWindow.Bottom - terminal.srWindow.Top;
}

void Terminal::update_input_state() {
  reset_input_state();

  while( WaitForSingleObject( input, 0 ) == WAIT_OBJECT_0 ) {
    INPUT_RECORD record;
    DWORD record_count = 0;

    ReadConsoleInputA( input, &record, 1, &record_count );

    if( record_count ) {
      if( record.EventType == KEY_EVENT &&
        record.Event.KeyEvent.bKeyDown &&
        record.Event.KeyEvent.wRepeatCount == 1 ) {

        switch( record.Event.KeyEvent.wVirtualKeyCode ) {
        case VK_UP: {
          input_state[static_cast<s32>(Input::Cursor_Up)] = true;
        } break;
        case VK_DOWN: {
          input_state[static_cast<s32>(Input::Cursor_Down)] = true;
        } break;
        case VK_RETURN: {
          input_state[static_cast<s32>(Input::Enter)] = true;
        } break;
        case VK_ESCAPE: {
          input_state[static_cast<s32>(Input::Escape)] = true;
        } break;
        }
      }
    }
  }
}
}
