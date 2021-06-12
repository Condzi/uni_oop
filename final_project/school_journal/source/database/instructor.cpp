#include "pch.hpp"

#include "database/instructor.hpp"

namespace sj
{
Instructor::Instructor( std::string const& names_, 
                        std::string const& surname_,
                        std::vector<Key>&& courses_ids_,
                        Key key_ ) :
   Database_Element( key_ ),
   names( names_ ),
   surname( surname_ ),
   courses_ids( std::move( courses_ids_ ) )
{}

std::string Instructor::get_names() const {
  return names;
}

std::string Instructor::get_surname() const {
  return surname;
}

std::vector<Key> const& Instructor::get_courses_ids() const {
  return courses_ids;
}
}
