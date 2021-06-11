#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
class Course final : public Database_Element {
public:
  Course( std::string const& name_, u32 ects_, 
          s32 instructor_id_, u64 key_ );

  [[nodiscard]]
  std::string const& get_name() const;

  [[nodiscard]]
  u32 get_ects() const;

  [[nodiscard]]
  s32 get_instructor_id() const;

private:
  std::string name;
  s32 instructor_id;
  u32 ects;
};
}