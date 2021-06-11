#include "pch.hpp"

#include "database/course.hpp"

namespace sj
{
Course::Course( std::string const& name_, 
                u32 ects_, s32 instructor_id_, u64 key_ ) : 
    Database_Element( key_ ),
    name( name_ ),
    ects( ects_ ),
    instructor_id( instructor_id_ )
{}

std::string const& Course::get_name() const {
  return name;
}

u32 Course::get_ects() const {
  return ects;
}

s32 Course::get_instructor_id() const {
  return instructor_id;
}
}
