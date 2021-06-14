#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
class Instructor final : public Database_Element {
public:
  Instructor( std::string const& names_, std::string const& surname_,
              std::vector<Key>&& courses_ids_, 
              std::vector<Key>&& students_ids_, Key key_ );

  [[nodiscard]]
  std::string get_names() const;
  
  [[nodiscard]]
  std::string get_surname() const;

  [[nodiscard]]
  std::vector<Key> const& get_courses_ids() const;

  [[nodiscard]]
  std::vector<Key> const& get_students_ids() const;

private:
  std::string names;
  std::string surname;
  std::vector<Key> courses_ids;
  std::vector<Key> students_ids;
};
}