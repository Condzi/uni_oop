#include "pch.hpp"

#include "ui/states/student_courses_view.hpp"
#include "ui/states/student_grades_view.hpp"

namespace sj
{
[[nodiscard]] static
s32 count_grades( Key index, Key course_id, Database& db ) {
  auto student = db.create_student( index );
  auto grade_ids = student.get_grades_ids();
  s32 count = 0;

  for( auto grade_id : grade_ids ) {
    auto grade = db.create_grade( grade_id );
    if( grade.get_course_id() == course_id ) {
      count++;
    }
  }

  return count;
}

[[nodiscard]] static
s32 options_count_helper( Key index, Key course_id, Database& db ) {
  auto count = count_grades( index, course_id, db );
  
  if( count == 0 ) {
    count = 1;
  }
    
  return count;
}

[[nodiscard]] static
bool has_grades( Key index, Key course_id, Database& db ) {
  auto count = count_grades( index, course_id, db );

  return count > 0;
}

Student_Grades_View::Student_Grades_View( Terminal& terminal_, 
                                          Database& database_, App& app_,
                                          Key index_, Key course_id_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    options_count_helper( index_, course_id_, database_ ) },
  index( index_ ),
  course_id( course_id_ ),
  no_grades( !has_grades( index, course_id, database ) )
{
  terminal.set_title( "User_Selection -> Student_Overview -> "
                      "Student_Courses_View -> Student_Grades_View" );
}

void Student_Grades_View::on_switch() {
  auto student = database.create_student( index );
  auto course = database.create_course( course_id );
  auto instructor = database.create_instructor( course.get_instructor_id() );
  auto instructor_str = instructor.get_names() + " " + instructor.get_surname();

  title = "Grades for " + course.get_name() + "(" + 
          std::to_string( course_id ) + ") by " + instructor_str;

  if( no_grades ) {
    options_labels[0] = "No grades found.";
  } else {
    auto grade_ids = student.get_grades_ids();
    s32 counter = 1;
    for( auto grade_id : grade_ids ) {
      auto grade = database.create_grade( grade_id );
      if( grade.get_course_id() == course_id ) {
        auto mark = grade.get_value_as_string();
        auto counter_str = std::to_string( counter );
        auto comment = grade.get_comment();

        options_labels[counter - 1] = counter_str + " | " + mark + 
                                      " | " + comment;
        counter++;
      }
    }
  }

  cursor_char = ' ';
  display_options();
}
State* Student_Grades_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return to "
                       "the courses view?" ) ) {
      return new Student_Courses_View{ terminal, database, app, index };
    }
  }

  return nullptr;
}
}