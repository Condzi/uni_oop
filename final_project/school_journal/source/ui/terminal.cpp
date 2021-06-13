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

void Terminal::set_pen_position( s32 x, s32 y ) {
  pen_position.x = std::clamp( x, 0, width );
  pen_position.y = std::clamp( y, 0, height );
}

void Terminal::move_pen( s32 dx, s32 dy ) {
  set_pen_position( pen_position.x + dx, pen_position.y + dy );
}

s32 Terminal::get_pen_x() const {
  return pen_position.x;
}

s32 Terminal::get_pen_y() const {
  return pen_position.y;
}

bool Terminal::is_key_pressed( Input_Type type ) {
  return input_state[static_cast<s32>( type )];
}

void Terminal::pen_write( std::string const& text ) const {
  std::cout << "\x1B[" << pen_position.y << ";" << pen_position.x << "H";
  std::cout << text;
}

void Terminal::clear_screen() const {
  system( "cls" );
}

void Terminal::wait_for_enter() const {
  std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
  std::cin.get();
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
  std::memset( input_state, 0, sizeof( bool ) * 4 );

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
          input_state[static_cast<s32>(Input_Type::Cursor_Up)] = true;
        } break;
        case VK_DOWN: {
          input_state[static_cast<s32>(Input_Type::Cursor_Down)] = true;
        } break;
        case VK_RETURN: {
          input_state[static_cast<s32>(Input_Type::Enter)] = true;
        } break;
        case VK_ESCAPE: {
          input_state[static_cast<s32>(Input_Type::Escape)] = true;
        } break;
        }
      }
    }
  }
}
}
