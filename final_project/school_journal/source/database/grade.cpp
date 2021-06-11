#include "pch.hpp"

#include "database/grade.hpp"

namespace sj
{
Grade::Grade( Value value_, std::string const& comment_, u64 key_ ) :
  Database_Element( key_ ),
  value( value_ ),
  comment( comment_ )
{}

std::string_view Grade::get_value_as_string() const {
  return Grade::value_to_string( value );
}

std::string Grade::get_comment() const {
  if( comment.empty() )
    return "No comment.";

  return comment;
}
}
