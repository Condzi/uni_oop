#include "pch.hpp"

#include "database/instructor.hpp"

namespace sj
{
Instructor::Instructor( std::string const& names_, 
                        std::string const& surname_,
                        u64 key_ ) :
   Database_Element( key_ ),
   names( names_ ),
   surname( surname_ )
{}

std::string Instructor::get_names() const {
  return names;
}

std::string Instructor::get_surname() const {
  return surname;
}
}
