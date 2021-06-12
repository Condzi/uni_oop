#include "pch.hpp"

#include "database/student.hpp"

namespace sj
{
Student::Student( std::vector<Key>&& enrolled_courses_ids_,
                  std::vector<Key>&& grades_ids_,
                  Key field_of_study_id_,
                  std::string const& names_,
                  std::string const& surname_,
                  Key key_ ) :
   Database_Element( key_ ),
   enrolled_courses_ids{ std::move( enrolled_courses_ids_ ) },
   grades_ids{ std::move( grades_ids_ ) },
   field_of_study_id( field_of_study_id_ ),
   names( names_ ),
   surname( surname_ )
{}

std::vector<Key> const& Student::get_enrolled_courses_ids() const {
  return enrolled_courses_ids;
}

std::vector<Key> const& Student::get_grades_ids() const {
  return grades_ids;
}

Key Student::get_field_of_study_id() const {
  return field_of_study_id;
}

std::string const& Student::get_names() const {
  return names;
}

std::string const& Student::get_surname() const {
  return surname;
}
}
