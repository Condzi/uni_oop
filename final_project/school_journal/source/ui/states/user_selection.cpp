#include "pch.hpp"

#include "ui/states/user_selection.hpp"
#include "ui/app.hpp"

namespace sj
{
User_Selection::User_Selection( Terminal& terminal_, Database& database_,
                                App& app_ ) :
  State( terminal_, database_, app_ )
{}

void User_Selection::on_switch() {
  selected_option = 0;
  terminal.clear_screen();

  terminal.set_pen_position( 10, 2 );
  terminal.pen_write( "Select User" );

  terminal.set_pen_position( 6, 4 );
  terminal.pen_write( "Student" );
  terminal.move_pen( 0, 1 );
  terminal.pen_write( "Instructor" );
  terminal.move_pen( 0, 1 );
  terminal.pen_write( "Student (enrollments)" );

  terminal.set_pen_position( cursor_start_pos.x, cursor_start_pos.y );
  terminal.pen_write( ">" );
}

std::optional<State::Type> User_Selection::update() {

  handle_up_down_input();

  if( terminal.is_key_pressed( Terminal::Input_Type::Enter ) ) {
    terminal.set_pen_position( 4, 9 );
    terminal.pen_write( "Type the person's ID: " );
    
    Key id;
    std::cin >> id;
    
    switch( selected_option ) {
      case 0: {
        if( try_to_set_student( id ) ) {
          return State::Type::Student;
        }
      } break;

      case 1: {
        if( try_to_set_instructor( id ) ) {
          return State::Type::Instructor;
        }
      } break;

      case 2: {
      if( try_to_set_student( id ) ) {
          return State::Type::Enrollments;
        }
      } break;

      default: SJ_THROW( "Wrong selected_option value!" );
    }

    return {};
  }

  redraw_cursor();

  return {};
}

bool User_Selection::try_to_set_student( Key index ) {
   try {
     auto s = database.create_student( index );
   }
   catch( ... ) {
     terminal.clear_screen();
     terminal.set_pen_position( 5, 5 );
     terminal.pen_write(
     "Student with index " + std::to_string( index ) +
     " has not been found. Try again.");
     terminal.wait_for_enter();
     // To redraw the screen UI.
     on_switch();
     return false;
   }

   app.set_user_id( index );
   app.set_user_type( App::User_Type::Student );

   terminal.clear_screen();
   terminal.set_pen_position( 5, 5 );
   auto s = database.create_student( index );
   terminal.pen_write( "Hello, " + s.get_names() + "!" );
   terminal.wait_for_enter();

   return true;
}

bool User_Selection::try_to_set_instructor( Key id ) {
   try {
     auto s = database.create_instructor( id );
   }
   catch( ... ) {
     terminal.clear_screen();
     terminal.set_pen_position( 5, 5 );
     terminal.pen_write(
     "Instructor with ID " + std::to_string( id ) +
     " has not been found. Try again.");
     terminal.wait_for_enter();
     // To redraw the screen UI.
     on_switch();
     return false;
   }

   app.set_user_id( id );
   app.set_user_type( App::User_Type::Instructor );

   terminal.clear_screen();
   terminal.set_pen_position( 5, 5 );
   auto s = database.create_instructor( id );
   terminal.pen_write( "Welcome, " + s.get_names() + "." );
   terminal.wait_for_enter();

   return true;
}

void User_Selection::handle_up_down_input() {
  if( terminal.is_key_pressed( Terminal::Input_Type::Cursor_Up ) ) {
    terminal.pen_write( " " );
    cursor_needs_redrawing = true;

    if( selected_option == 0 ) {
      selected_option = 2;
    } else {
      selected_option--;
    }
  }

  if( terminal.is_key_pressed( Terminal::Input_Type::Cursor_Down ) ) {
    terminal.pen_write( " " );
    cursor_needs_redrawing = true;

    if( selected_option == 2 ) {
      selected_option = 0;
    } else {
      selected_option++;
    }
  }
}

void User_Selection::redraw_cursor() {
  if( cursor_needs_redrawing ) {
    terminal.set_pen_position( cursor_start_pos.x, 
                               cursor_start_pos.y + selected_option );
    terminal.pen_write( ">" );
    cursor_needs_redrawing = false;
  }
}
}
