#include "pch.hpp"

#include "ui/states/user_selection.hpp"

namespace sj
{
User_Selection::User_Selection( Terminal& terminal_, Database& database_ ) :
  State( terminal_, database_ )
{}

void User_Selection::on_switch() {
  terminal.clear_screen();

  terminal.set_pen_position( 10, 2 );
  terminal.pen_write( "Select User" );

  terminal.set_pen_position( 6,3 );
  terminal.pen_write( "Student" );
  terminal.move_pen( 0, 1 );
  terminal.pen_write( "Instructor" );
  terminal.move_pen( 0, 1 );
  terminal.pen_write( "Student (enrollments)" );

  terminal.set_pen_position( cursor_start_pos.x, cursor_start_pos.y );
  terminal.pen_write( ">" );
}

std::optional<State::Type> User_Selection::update() {
  bool redraw_cursor = false;

  if( terminal.is_key_pressed( Terminal::Input_Type::Cursor_Up ) ) {
    terminal.pen_write( " " );
    redraw_cursor = true;

    if( selected_option == 0 ) {
      selected_option = 2;
    } else {
      selected_option--;
    }

  }

  if( terminal.is_key_pressed( Terminal::Input_Type::Cursor_Down ) ) {
    terminal.pen_write( " " );
    redraw_cursor = true;

    if( selected_option == 2 ) {
      selected_option = 0;
    } else {
      selected_option++;
    }
  }

  if( terminal.is_key_pressed( Terminal::Input_Type::Enter ) ) {
    switch( selected_option ) {
      case 0: return State::Type::Student;
      case 1: return State::Type::Instructor;
      case 2: return State::Type::Enrollments;
      default: SJ_THROW( "Wrong selected_option value!" );
    }
  }

  if( redraw_cursor ) {
    terminal.set_pen_position( cursor_start_pos.x, 
                               cursor_start_pos.y + selected_option );
    terminal.pen_write( ">" );
  }

  return {};
}
}
