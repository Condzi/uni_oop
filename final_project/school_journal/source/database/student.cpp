#include "pch.hpp"

#include "database/student.hpp"

namespace sj
{
Student::Student( std::vector<Course>&& enrolled_courses_,
                  std::vector<Grade>&& grades_,
                  Field_Of_Study const& field_of_study_,
                  std::string const& names_,
                  std::string const& surname_,
                  u64 key_ ) :
  Database_Element( key_ ),
  enrolled_courses{ std::move( enrolled_courses_ ) },
  grades{ std::move( grades_ ) },
  field_of_study( field_of_study_ ),
  names( names_ ),
  surname( surname_ )
{}

std::vector<Course> const& Student::get_enrolled_courses() const {
  return enrolled_courses;
}

std::vector<Grade> const& Student::get_grades() const {
  return grades;
}

Field_Of_Study const& Student::get_field_of_study() const {
  return field_of_study;
}

std::string const& Student::get_names() const {
  return names;
}

std::string const& Student::get_surname() const {
  return surname;
}
}
