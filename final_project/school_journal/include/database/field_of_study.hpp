#pragma once

#include "pch.hpp"

#include "database_element.hpp"

namespace sj
{
class Field_Of_Study final : public Database_Element {
public:
  Field_Of_Study( std::string const& short_name_, 
                  std::string const& full_name_,
                  u64 key_ );

  [[nodiscard]]
  std::string get_short_name() const;
  [[nodiscard]]
  std::string get_full_name() const;

private:
  std::string short_name;
  std::string full_name;
};
}