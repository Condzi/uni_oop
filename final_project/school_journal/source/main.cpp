#include "pch.hpp"

#include "database/database.hpp"

#include <Windows.h>

// @Todo: regenerate database before testing!

int main() {
  sj::Database db;

  SetConsoleTitleA("MY TITLE!");
  db.set_folder( "non_prod/" );
  db.load_from_folder();

  auto s = db.create_student( 100101 );

  sj::debug_print( "\n\nStudent data:" );
  sj::debug_print( "Name(s): %s | Surname: %s", s.get_names().c_str(), s.get_surname().c_str() );
  auto fos = db.create_field_of_study( s.get_field_of_study_id() );
  sj::debug_print( "Index: %d | Field of study: %s (%s)", s.get_key(), fos.get_short_name().c_str(), fos.get_full_name().c_str() );
  
  sj::debug_print( "\nEnrolled courses:" );

  for( auto course_id : s.get_enrolled_courses_ids() ) {
    auto course = db.create_course( course_id );
    auto instructor = db.create_instructor( course.get_instructor_id() );
    sj::debug_print(" %s worth %d ECTS | %s %s", course.get_name().c_str(), course.get_ects(), instructor.get_names().c_str(), instructor.get_surname().c_str() );
  }

  sj::debug_print("\nGrades:");
  for( auto grade_id : s.get_grades_ids() ) {
    auto grade = db.create_grade( grade_id );
    auto course = db.create_course( grade.get_course_id() );
    auto instructor = db.create_instructor( course.get_instructor_id() );

    sj::debug_print(" Course '%s': %s | Comment: '%s' | Given by %s %s", 
                                            course.get_name().c_str(), 
                                            grade.get_value_as_string().c_str(),
                                            grade.get_comment().c_str(), 
                                            instructor.get_names().c_str(), 
                                            instructor.get_surname().c_str() );
  }

  //db.add_grade( sj::Grade::Value::Four_Half, "Test grade", s.get_key(), s.get_enrolled_courses_ids().front() );
  db.save_to_folder();

  std::cin.get();
}
