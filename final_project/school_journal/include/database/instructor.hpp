#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
// @ToDo: It's a basic version, only for Student view. Maybe
// add simmillar version for Instructor view, which has more
// info about Instructor and less about the student?
// Since they don't need to know a lot about each other.
class Instructor final : public Database_Element {
public:
  Instructor( std::string const& names_, std::string const& surname_,
              u64 key_ );

  [[nodiscard]]
  std::string get_names() const;
  [[nodiscard]]
  std::string get_surname() const;

private:
  std::string names;
  std::string surname;
};
}