#include "pch.hpp"

#include "ui/states/instructor_students_view.hpp"
#include "ui/states/instructor_student_grades_view.hpp"

namespace sj
{
[[nodiscard]] static
std::vector<Key> find_grades_from_course( Key student_id, Key course_id,
                                          Database& db ) {
  auto student = db.create_student( student_id );
  auto grade_ids = student.get_grades_ids();
  std::vector<Key> grades_ids;

  for( auto grade_id : grade_ids ) {
    auto grade = db.create_grade( grade_id );
    if( grade.get_course_id() == course_id ) {
      grades_ids.push_back( grade_id );
    }
  }

  return grades_ids;
}

[[nodiscard]] static
s32 options_count_helper( Key student_id, Key course_id, Database& db ) {
  auto grades = find_grades_from_course( student_id, course_id, db );
     
  if( grades.empty() ) {
    return 1;
  }

  return static_cast<s32>( grades.size() ) + 1;
}

[[nodiscard]] static
bool has_grades( Key student_id, Key course_id, Database& db ) {
  return !find_grades_from_course( student_id, course_id, db ).empty();
}

Instructor_Student_Grades_View::Instructor_Student_Grades_View( 
                                                    Terminal& terminal_, 
                                                    Database& database_, App& app_,
                                                    Key id_, Key course_id_,
                                                    Key student_id_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    options_count_helper( student_id_, course_id_, database_ ) },
  instructor_id( id_ ),
  course_id( course_id_ ),
  student_id( student_id_ ),
  no_grades( !has_grades( student_id, course_id, database ) )
{
  terminal.set_title( "User_Selection -> Instructor_Overview -> "
                      "Instructor_Courses_View -> Instructor_Students_View -> "
                      "Instructor_Student_Grades_View" );

    grades_ids = find_grades_from_course( student_id, course_id,
                                                 database );

    std::sort( grades_ids.begin(), grades_ids.end() );
}

void Instructor_Student_Grades_View::on_switch() {
  auto const student = database.create_student( student_id );
  auto const student_id_str = std::to_string( student_id );
  auto const course = database.create_course( course_id );
  auto const course_id_str = std::to_string( course_id );

  title = "Grades for " + student_id_str + " from " + course.get_name() + "(" + 
          course_id_str + ")";

  if( no_grades ) {
    options_labels[0] = "No grades found. Add first grade here.";
  } else {
    options_labels[0] = "Add grade.";


    for( size_t i = 0; i < grades_ids.size(); i++ ) {
      auto const grade = database.create_grade( grades_ids[i] );
      auto i_str = std::to_string( i + 1 );

      options_labels[i + 1] = i_str + ". " + grade.get_value_as_string();
    }
  }

  display_options();
  display_cursor();
}

State* Instructor_Student_Grades_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return "
                       "to the instructor students view?" ) ) {
      return new Instructor_Students_View{ terminal, database, app,
                                           instructor_id, course_id };
    }
  }

  if( input == 1 ) {
    if( no_grades || current_option == 0 ) {
      if( add_grade() ) {
        return new Instructor_Student_Grades_View{ terminal, database, app,
                                                   instructor_id, course_id,
                                                   student_id };
      }
    } else {
      auto grade = database.create_grade( grades_ids[current_option - 1] );
      // A little workaround, since there's no proper "prompt_info".
      prompt_error( "Comment for " + std::to_string( current_option ) + 
                    ". grade: " + grade.get_comment() );  
      display_options();
    }
  }

  display_cursor();

  return nullptr;
}

bool Instructor_Student_Grades_View::add_grade() {
  std::string input;
  ask_for_input( "Adding a grade. Specify the mark (2.0 - 5.5): ", input );
  Grade::Value value;

  try {
    value = Grade::string_to_value( input );
  } catch ( ... ) {
    prompt_error( "Value '" + input + "' is invalid. Try again." );
    on_switch();
    return false;
  }

  ask_for_input( "Adding a grade. Leave a comment "
                 "or 'x' if you don't have any: ", input );

  if( input == "x" || input == "X" ) {
    input.clear();
  }

  try {
    database.add_grade( value, input, student_id, course_id );
  } catch ( std::exception const& ex ) {
    prompt_error( std::string{ "Couldn't add a grade. Details: " } + ex.what() );
    display_options();
    return false;
  }

  return true;
}
}
