#include "pch.hpp"

#include "database/field_of_study.hpp"

namespace sj
{
Field_Of_Study::Field_Of_Study( std::string const& short_name_, 
                                std::string const& full_name_,
                                u64 key_ ) :
  Database_Element( key_ ),
  short_name( short_name_ ),
  full_name( full_name_ )
{}

std::string Field_Of_Study::get_short_name() const {
  return short_name;
}

std::string Field_Of_Study::get_full_name() const {
  return full_name;
}
}
