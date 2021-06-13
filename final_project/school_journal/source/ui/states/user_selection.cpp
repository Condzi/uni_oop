#include "pch.hpp"

#include "ui/states/user_selection.hpp"
#include "ui/app.hpp"

namespace sj
{
User_Selection::User_Selection( Terminal& terminal_, Database& database_,
                                App& app_ ) :
  Basic_Menu_State( terminal_, database_, app_, 3 )
{}

void User_Selection::on_switch() {
  title = "Select user";
  options_labels[0] = "Student";
  options_labels[1] = "Instructor";
  options_labels[2] = "Enrollments";

  display_options();
}

State* User_Selection::update() {
  display_cursor();

  auto input = update_input();

  if( input == -1 ) {
    char c;
    ask_for_input("\x1B[4mAre you sure you want to exit?\x1B[24m [Y/N]: ", c );

    if( c == 'Y' || c == 'y' ) {
      app.request_quit();
    } else {
      terminal.clear_screen();
      display_options();
      display_cursor();
    }
  } else if ( input == 1 ) {
    Key id;
    ask_for_input( "Specify ID/Index: ", id );

    switch( current_option ) {
      case 0: {
        if( check_if_student_exists( id ) ) {
          // @ToDo
        }
      } break;
      case 1: {
        if( check_if_instructor_exists( id ) ) {
          // @ToDo
        }
      } break;
      case 2: {
        if( check_if_student_exists( id ) ) {
          // @ToDo
        }
      }
    }
  }

  return nullptr;
}

bool User_Selection::check_if_student_exists( Key index ) {
   try {
     auto s = database.create_student( index );
   }
   catch( ... ) {
     prompt_error( "Student with index " + std::to_string( index ) +
     " has not been found." );
     terminal.clear_screen();
     display_options();
     display_cursor();

     return false;
   }

   auto s = database.create_student( index );
   // @ToDo: remove me
   prompt_error( "Hello, " + s.get_names() + "!" );
   terminal.clear_screen();
   display_options();
   display_cursor();

   return true;
}

bool User_Selection::check_if_instructor_exists( Key id ) {
   try {
     auto i = database.create_instructor( id );
   }
   catch( ... ) {
     prompt_error( "Instructor with ID " + std::to_string( id ) +
     " has not been found." );
     terminal.clear_screen();
     display_options();
     display_cursor();

     return false;
   }

   auto s = database.create_instructor( id );
   // @ToDo: remove me
   prompt_error( "Welcome, " + s.get_names() + "." );
   terminal.clear_screen();
   display_options();
   display_cursor();

   return true;
}
}
