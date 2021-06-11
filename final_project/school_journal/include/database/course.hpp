#pragma once

#include "pch.hpp"

#include "database_element.hpp"
#include "instructor.hpp"

namespace sj
{
class Course final : public Database_Element {
public:
  Course( std::string const& name_, u32 ects_, 
          Instructor const& instructor_, u64 key_ );

  [[nodiscard]]
  std::string const& get_name() const;

  [[nodiscard]]
  u32 get_ects() const;

  [[nodiscard]]
  Instructor const& get_instructor() const;

private:
  Instructor instructor;
  std::string name;
  u32 ects;
};
}