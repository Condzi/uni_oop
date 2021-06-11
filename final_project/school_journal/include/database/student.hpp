#pragma once

#include "pch.hpp"

#include "database_element.hpp"
#include "grade.hpp"
#include "field_of_study.hpp"
#include "course.hpp"

namespace sj
{
class Student final : public Database_Element {
public:
  Student( std::vector<Course>&& enrolled_courses_,
           std::vector<Grade>&& grades_,
           Field_Of_Study const& field_of_study_,
           std::string const& names_,
           std::string const& surname_,
           u64 key_ );


  [[nodiscard]]
  std::vector<Course> const& get_enrolled_courses() const;

  [[nodiscard]]
  std::vector<Grade> const& get_grades() const;

  [[nodiscard]]
  Field_Of_Study const& get_field_of_study() const;

  [[nodiscard]]
  std::string const& get_names() const;

  [[nodiscard]]
  std::string const& get_surname() const;

private:
  std::vector<Course> enrolled_courses;
  std::vector<Grade> grades;
  Field_Of_Study field_of_study;

  std::string names;
  std::string surname;
};
}
