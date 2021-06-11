#include "pch.hpp"

#include "database/grade.hpp"

namespace sj
{
Grade::Grade( Value value_, std::string const& comment_, 
              Course const& course_, u64 key_ ) :
  Database_Element( key_ ),
  value( value_ ),
  comment( comment_ ),
  course( course_ )
{}

Grade::Value Grade::string_to_value( std::string const &str ) {
  if( str == "2.0" ) return Value::Two;
  if( str == "3.0" ) return Value::Three;
  if( str == "3.5" ) return Value::Three_Half;
  if( str == "4.0" ) return Value::Four;
  if( str == "4.5" ) return Value::Four_Half;
  if( str == "5.0" ) return Value::Five;
  if( str == "5.5" ) return Value::Five_Half;

  SJ_THROW( "Wrong string for grade value!" );
}

std::string Grade::get_value_as_string() const {
  auto sw = Grade::value_to_string( value );
  return { sw.begin(), sw.end() };
}

std::string Grade::get_comment() const {
  if( comment.empty() )
    return "No comment.";

  return comment;
}

Course const& Grade::get_course() const {
  return course;
}
}
