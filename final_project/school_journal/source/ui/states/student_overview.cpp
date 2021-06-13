#include "pch.hpp"

#include "ui/states/student_overview.hpp"
#include "ui/states/user_selection.hpp"

namespace sj
{
Student_Overview::Student_Overview( Terminal& terminal_, Database& database_,
                                    App& app_, Key index_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 2 },
  index( index_ ),
  student{ database.create_student( index ) }
{}

void Student_Overview::on_switch() {
  title = "Hello, " + student.get_names() + "!";
  options_labels[0] = "Your data";
  options_labels[1] = "Courses and grades";

  display_options();
}

State* Student_Overview::update() {
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
        // @ToDo: student data view
      } break;
      case 1: {
        // @ToDo courses (and grades) view
      } break;
    }
  }

  return nullptr;
}
}
