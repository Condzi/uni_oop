#include "pch.hpp"

#include "database/grade.hpp"

namespace sj
{
Grade::Grade( Value value_, std::string const& comment_, 
              Key course_id_, Key key_ ) :
  Database_Element( key_ ),
  value( value_ ),
  comment( comment_ ),
  course_id( course_id_ )
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

f32 Grade::get_value_as_float() const {
  switch( value ) {
    case Value::Two:        return 2.0f;
    case Value::Three:      return 3.0f;
    case Value::Three_Half: return 3.5f;
    case Value::Four:       return 4.0f;
    case Value::Four_Half:  return 4.5f;
    case Value::Five:       return 5.0f;
    case Value::Five_Half:  return 5.5f;
  }

  return -1;
}

std::string Grade::get_comment() const {
  if( comment.empty() )
    return "No comment.";

  return comment;
}

Key Grade::get_course_id() const {
  return course_id;
}
}
