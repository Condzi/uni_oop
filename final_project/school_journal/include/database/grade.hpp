#pragma once

#include "pch.hpp"

#include "database_element.hpp"
#include "course.hpp"

namespace sj
{
class Grade final : public Database_Element {
public:
  enum class Value {
    Two,
    Three,
    Three_Half,
    Four,
    Four_Half,
    Five,
    Five_Half
  };

  Grade( Value value_, std::string const& comment_, Key course_id_,
         Key key_ );

  [[nodiscard]] constexpr static
  std::string_view value_to_string( Value const& val ) {
    switch( val ) {
      case Value::Two:        return "2.0";
      case Value::Three:      return "3.0";
      case Value::Three_Half: return "3.5";
      case Value::Four:       return "4.0";
      case Value::Four_Half:  return "4.5";
      case Value::Five:       return "5.0";
      case Value::Five_Half:  return "5.5";
      default: return "<?>";
    }
  }

  [[nodiscard]] static
  Value string_to_value( std::string const& str );

  [[nodiscard]]
  std::string get_value_as_string() const;

  [[nodiscard]]
  f32 get_value_as_float() const;

  [[nodiscard]]
  std::string get_comment() const;

  [[nodiscard]]
  Key get_course_id() const;

private:
  Value value;
  std::string comment;
  Key course_id;
};
}
