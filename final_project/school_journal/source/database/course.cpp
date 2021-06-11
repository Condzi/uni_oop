#include "pch.hpp"

#include "database/course.hpp"

namespace sj
{
Course::Course( std::string const& name_, 
                u32 ects_, Instructor const& instructor_,
                u64 key_ ) : 
    Database_Element( key_ ),
    name( name_ ),
    ects( ects_ ),
    instructor( instructor_ )
{}

std::string const& Course::get_name() const {
  return name;
}

u32 Course::get_ects() const {
  return ects;
}

Instructor const& Course::get_instructor() const {
  return instructor;
}
}
