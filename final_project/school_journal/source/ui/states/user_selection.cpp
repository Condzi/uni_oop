#include "pch.hpp"

#include "ui/states/user_selection.hpp"
#include "ui/states/student_overview.hpp"
#include "ui/states/instructor_overview.hpp"
#include "ui/app.hpp"

namespace sj
{
User_Selection::User_Selection( Terminal& terminal_, Database& database_,
                                App& app_ ) :
  Basic_Menu_State( terminal_, database_, app_, 3 )
{
  terminal.set_title( "User_Selection" );
}

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
    if( yes_no_prompt( "Are you sure you want to exit?" ) ) {
      app.request_quit();
    }
  } else if ( input == 1 ) {  
    switch( current_option ) {
      case 0: {
        Key index;
        ask_for_input( "Index: ", index );
        if( check_if_student_exists( index ) ) {
          return new Student_Overview{ terminal, database, app, index };
        }
      } break;
      case 1: {
        Key id;
        ask_for_input( "ID: ", id );
        if( check_if_instructor_exists( id ) ) {
          return new Instructor_Overview{ terminal, database, app, id };
        }
      } break;
      case 2: {
        Key index;
        ask_for_input( "Index: ", index );
        if( check_if_student_exists( index ) ) {
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
  
  return true;
}
}
