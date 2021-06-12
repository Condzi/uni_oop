#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
class Student final : public Database_Element {
public:
  Student( std::vector<Key>&& enrolled_courses_ids_,
           std::vector<Key>&& grades_ids_,
           Key field_of_study_id_,
           std::string const& names_,
           std::string const& surname_,
           Key key_ );


  [[nodiscard]]
  std::vector<Key> const& get_enrolled_courses_ids() const;

  [[nodiscard]]
  std::vector<Key> const& get_grades_ids() const;

  [[nodiscard]]
  Key get_field_of_study_id() const;

  [[nodiscard]]
  std::string const& get_names() const;

  [[nodiscard]]
  std::string const& get_surname() const;

private:
  std::vector<Key> enrolled_courses_ids;
  std::vector<Key> grades_ids;
  Key field_of_study_id;

  std::string names;
  std::string surname;
};
}
