#include "pch.hpp"

#include "ui/states/instructor_overview.hpp"
#include "ui/states/instructor_data_view.hpp"

namespace sj
{
Instructor_Data_View::Instructor_Data_View( Terminal& terminal_,
                                            Database& database_, 
                                            App &app_, Key id_) :
  Basic_Menu_State{ terminal_, database_, app_, 4 },
  instructor( database.create_instructor( id_ ) ),
  id( id_ )
{
  terminal.set_title( "User_Selection -> Instructor_Overview -> "
                      "Instructor_Data_View" );
}

void Instructor_Data_View::on_switch() {
  title = "Your data";

  auto courses_count = instructor.get_courses_ids().size();

  options_labels[0] = "Name(s): " + instructor.get_names();
  options_labels[1] = "Surname: " + instructor.get_surname();
  options_labels[2] = "     ID: " + std::to_string( id );
  options_labels[3] = "Courses: " + 
                      std::to_string( courses_count );

  cursor_char = ' ';
  display_options();
}

State* Instructor_Data_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want " 
                       "to return to Student Overview?" ) ) {
      return new Instructor_Overview{ terminal, database, app, id };
    }
  }

  return nullptr;
}
}
