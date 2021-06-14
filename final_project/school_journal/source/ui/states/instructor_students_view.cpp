#include "pch.hpp"

#include "ui/states/instructor_students_view.hpp"
#include "ui/states/instructor_courses_view.hpp"

namespace sj
{
[[nodiscard]] static
std::vector<Key> find_students_for_course( Key instructor_id, Key course_id,
                                           Database& db ) {
  auto instr = db.create_instructor( instructor_id );
  auto const& students = instr.get_students_ids();
  std::vector<Key> result;

  for( auto student_id : students ) {
    auto student = db.create_student( student_id );
    auto courses = student.get_enrolled_courses_ids();

    auto it = std::find( courses.begin(), courses.end(), course_id );
    if( it != courses.end() ) {
      result.push_back( student_id );
    }
  }

  return result;
}

[[nodiscard]] static
s32 options_count_helper( Key instructor_id, Key course_id, Database& db ) {
  auto students = find_students_for_course( instructor_id, course_id, db );
     
  if( students.empty() ) {
    return 1;
  }

  return students.size();
}

[[nodiscard]] static
bool has_students( Key instructor_id, Key course_id, Database& db ) {
  return !find_students_for_course( instructor_id, course_id, db ).empty();
}

Instructor_Students_View::Instructor_Students_View( Terminal& terminal_, 
                                                    Database& database_, App& app_,
                                                    Key id_, Key course_id_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    options_count_helper( id_, course_id_, database_ ) },
  instructor_id( id_ ),
  course_id( course_id_ ),
  no_students( !has_students( instructor_id, course_id, database ) )
{
  terminal.set_title( "User_Selection -> Instructor_Overview -> "
                      "Instructor_Courses_View -> Instructor_Students_View" );
}

void Instructor_Students_View::on_switch() {
  auto course = database.create_course( course_id );
  title = "Students attending " + course.get_name() + "(" + 
          std::to_string( course_id ) + ")";

  if( no_students ) {
    options_labels[0] = "No students found.";
  } else {
    auto students_ids = find_students_for_course( instructor_id, course_id,
                                                  database );

    std::sort( students_ids.begin(), students_ids.end() );

    for( size_t i = 0; i < students_ids.size(); i++ ) {
      auto const index = students_ids[i];
      auto const index_str = std::to_string( index );
      auto const student = database.create_student( index );

      options_labels[i] = index_str + " | " + student.get_surname() + 
                          " " + student.get_names();
    }
  }

  if( no_students ) {
    cursor_char = ' ';
  }

  display_options();
  display_cursor();
}

State* Instructor_Students_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return "
                       "to the instructor courses view?" ) ) {
      return new Instructor_Courses_View{ terminal, database, app, instructor_id };
    }
  }

  if( input == 1 && !no_students ) {
    // @ToDo: switch to grade view
  }

  display_cursor();

  return nullptr;
}
}