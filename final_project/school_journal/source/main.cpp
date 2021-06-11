#include "pch.hpp"

#include "database/database.hpp"

// @Todo: regenerate database before testing!

int main() {
  sj::Database db;

  db.set_folder( "database/" );
  db.load_from_folder();

  auto s = db.create_student(100101);

  sj::debug_print( "\n\nStudent data:" );
  sj::debug_print( "%s %s", s.get_names().c_str(), s.get_surname().c_str() );
  sj::debug_print( "%d, %s %s", s.get_key(), s.get_field_of_study().get_short_name().c_str(), s.get_field_of_study().get_full_name().c_str() );
  
  sj::debug_print( "\nEnrolled courses:" );

  for( auto const& course : s.get_enrolled_courses() )
    sj::debug_print(" %s, %d ects, %s %s", course.get_name().c_str(), course.get_ects(), course.get_instructor().get_names().c_str(), course.get_instructor().get_surname().c_str() );

  sj::debug_print("\nGrades:");
  for( auto const& grade : s.get_grades() ) {
    sj::debug_print(" %s: %s, '%s', %s %s", grade.get_course().get_name().c_str(), 
                                            grade.get_value_as_string().c_str(),
                                            grade.get_comment().c_str(), 
                                            grade.get_course().get_instructor().get_names().c_str(), 
                                            grade.get_course().get_instructor().get_surname().c_str() );
  }
}
