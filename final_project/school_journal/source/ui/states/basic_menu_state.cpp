#include "pch.hpp"

#include "ui/states/basic_menu_state.hpp"

namespace sj
{
Basic_Menu_State::Basic_Menu_State( Terminal &terminal_,
                                    Database &database_,
                                    App &app_,
                                    s32 options_count_ ) :
  State( terminal_, database_, app_ ),
  options_count( std::clamp( options_count_, 0, MAX_OPTIONS_COUNT - 1 ) ),
  INPUT_POS( OPTIONS_POS.x, OPTIONS_POS.y + options_count ),
  ERROR_POS( OPTIONS_POS.x, INPUT_POS.y + 1 )
{
  if( options_count_ >= MAX_OPTIONS_COUNT ) {
    prompt_error( "Too many options to display. Showing only first " +
                  std::to_string( options_count ) + "." );
  }
}

void Basic_Menu_State::display_options() {
  terminal.clear_screen();

  terminal.set_pen_coords( TITLE_POS );
  terminal.pen_write( "\x1B[4m" + title + "\x1B[24m" );

  terminal.set_pen_coords( OPTIONS_POS );
  for( auto const& label : options_labels ) {
    terminal.pen_write( label );
    terminal.move_pen( {0, 1} );
  }

  cursor_needs_redrawing = true;
}

void Basic_Menu_State::display_cursor() {
  if( cursor_needs_redrawing ) {
    Coords cursor_pos = CURSOR_STARTING_POS;
    cursor_pos.y += current_option;
    terminal.set_pen_coords( cursor_pos );
    terminal.pen_write( std::string{ cursor_char } );
    cursor_needs_redrawing = false;
  }
}

s32 Basic_Menu_State::update_input() {
  if( terminal.is_key_pressed( Terminal::Input::Cursor_Up ) ) {
    terminal.pen_write( " " );
    cursor_needs_redrawing = true;

    if( current_option == 0 ) {
      current_option = options_count - 1;
    } else {
      current_option--;
    }
  }

  if( terminal.is_key_pressed( Terminal::Input::Cursor_Down ) ) {
    terminal.pen_write( " " );
    cursor_needs_redrawing = true;

    if( current_option == options_count - 1 ) {
      current_option = 0;
    } else {
      current_option++;
    }
  }

  if( terminal.is_key_pressed( Terminal::Input::Enter ) ) {
    return 1;
  }
  if( terminal.is_key_pressed( Terminal::Input::Escape ) ) {
    return -1;
  } 

  return 0;
}

void Basic_Menu_State::prompt_error( std::string const& label ) {
  terminal.set_pen_coords( ERROR_POS );
  terminal.pen_write( label );
  terminal.move_pen( {0, 1} );
  terminal.pen_write( "Press Enter to continue." );
  terminal.wait_for_enter();
}

bool Basic_Menu_State::yes_no_prompt( std::string const& label ) {
  char c;
  ask_for_input("\x1B[4m" + label + "\x1B[24m [Y/N]: ", c );

  if( c == 'Y' || c == 'y' ) {
    return true;
  }

  terminal.clear_screen();
  display_options();
  display_cursor();

  return false;
}
}
