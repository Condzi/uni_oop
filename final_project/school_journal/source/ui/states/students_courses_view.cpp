#include "pch.hpp"

#include "ui/states/student_courses_view.hpp"
#include "ui/states/student_overview.hpp"

namespace sj
{
[[nodiscard]] static
s32 options_count_helper( Key index, Database& db ) {
  auto student = db.create_student( index );
  auto count = static_cast<s32>( student.get_enrolled_courses_ids().size() );
  if( count == 0 ) {
    count = 1;
  }
    
  return count;
}

[[nodiscard]] static
bool has_courses( Key index, Database& db ) {
  return !db.create_student( index ).get_enrolled_courses_ids().empty();
}

Student_Courses_View::Student_Courses_View( Terminal& terminal_, 
                                            Database& database_, App& app_,
                                            Key index_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    options_count_helper( index_, database_ ) },
  index( index_ ),
  no_courses( !has_courses( index, database ) )
{
  terminal.set_title( "User_Selection -> Student_Overview ->"
                      "Student_Courses_View" );
}

void Student_Courses_View::on_switch() {
  title = "Enrolled courses";

  if( no_courses ) {
    options_labels[0] = "No enrolled courses found.";
  } else {
    auto const student     = database.create_student( index );
    auto const courses_ids = student.get_enrolled_courses_ids();

    for( size_t i = 0; i < courses_ids.size(); i++ ) {
      auto course = database.create_course( courses_ids[i] );

      options_labels[i] = course.get_name() + " (" + 
                          std::to_string( courses_ids[i] ) + ")";
    }
  }

  if( no_courses ) {
    cursor_char = ' ';
  }

  display_options();
  display_cursor();
}

State* Student_Courses_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return to Student Overview?" )) {
      return new Student_Overview{ terminal, database, app, index };
    }
  }

  if( input == 1 && !no_courses ) {
    // @ToDo: create new student grades view for courses_ids[current_option]
  }

  display_cursor();

  return nullptr;
}
}