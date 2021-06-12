#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
class Course final : public Database_Element {
public:
  Course( std::string const& name_, s32 ects_, 
          Key instructor_id_, Key key_ );

  [[nodiscard]]
  std::string const& get_name() const;

  [[nodiscard]]
  u32 get_ects() const;

  [[nodiscard]]
  Key get_instructor_id() const;

private:
  std::string name;
  Key instructor_id;
  s32 ects;
};
}