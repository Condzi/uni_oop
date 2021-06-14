#include "pch.hpp"

#include "ui/states/instructor_overview.hpp"
#include "ui/states/user_selection.hpp"
#include "ui/states/instructor_data_view.hpp"
#include "ui/states/instructor_courses_view.hpp"

namespace sj
{
Instructor_Overview::Instructor_Overview( Terminal& terminal_, Database& database_,
                                  App &app_, Key id_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 2 },
  instructor{ database.create_instructor( id_ ) },
  id( id_ )
{
  terminal.set_title( "User_Selection -> Instructor_Overview" );
}

void Instructor_Overview::on_switch() {
  title = "Welcome, " + instructor.get_names() + ".";

  options_labels[0] = "Your data";
  options_labels[1] = "Courses and students";

  display_options();
}

State* Instructor_Overview::update() {
  display_cursor();

  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return to the user selection screen?" ) ) {
      return new User_Selection{ terminal, database, app };
    }
  }
  
  if( input == 1 ) {
    switch( current_option ) {
      case 0: {
        return new Instructor_Data_View{ terminal, database, app, id };
      } break;
      case 1: {
        return new Instructor_Courses_View{ terminal, database, app, id };
      } break;
    }
  }

  return nullptr;
}
}
