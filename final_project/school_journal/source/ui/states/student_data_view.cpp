#include "pch.hpp"

#include "ui/states/student_overview.hpp"
#include "ui/states/student_data_view.hpp"

namespace sj
{
Student_Data_View::Student_Data_View( Terminal& terminal_, 
                                      Database& database_, App& app_,
                                      Key index_ ) :
  Basic_Menu_State( terminal_, database_, app_, 5 ),
  index( index_ ),
  student( database.create_student( index ) )
{
  terminal.set_title( "User_Selection -> Student_Overview ->"
                      "Student_Data_View" );
}


void Student_Data_View::on_switch() {
  title = "Your data";

  auto const field_of_study = database.create_field_of_study( student.get_field_of_study_id() );
  auto courses_count = student.get_enrolled_courses_ids().size();

  options_labels[0] = "Name(s): " + student.get_names();
  options_labels[1] = "Surname: " + student.get_surname();
  options_labels[2] = "  Index: " + std::to_string( index );
  options_labels[3] = "Field of study: " + 
                      field_of_study.get_short_name() + 
                      " (" + field_of_study.get_full_name() 
                      + ")";
  options_labels[4] = "Courses: " + 
                      std::to_string( courses_count );

  cursor_char = ' ';
  display_options();
}

State* Student_Data_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want " 
                       "to return to Student Overview?" ) ) {
      return new Student_Overview{ terminal, database, app, index };
    }
  }

  return nullptr;
}
}