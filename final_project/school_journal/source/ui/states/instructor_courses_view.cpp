#include "pch.hpp"

#include "ui/states/instructor_courses_view.hpp"
#include "ui/states/instructor_overview.hpp"

namespace sj
{
[[nodiscard]] static
s32 options_count_helper( Key id, Database& db ) {
  auto instr = db.create_instructor( id );
  auto count = static_cast<s32>( instr.get_courses_ids().size() );
  if( count == 0 ) {
    count = 1;
  }
    
  return count;
}

[[nodiscard]] static
bool has_courses( Key id, Database& db ) {
  return !db.create_instructor( id ).get_courses_ids().empty();
}

Instructor_Courses_View::Instructor_Courses_View( Terminal& terminal_, 
                                                  Database& database_, App& app_,
                                                  Key id_ ) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    options_count_helper( id_, database_ ) },
  id( id_ ),
  no_courses( !has_courses( id, database ) )
{
  terminal.set_title( "User_Selection -> Instructor_Overview -> "
                      "Instructor_Courses_View" );
}

void Instructor_Courses_View::on_switch() {
  title = "Courses instructed";

  if( no_courses ) {
    options_labels[0] = "No enrolled courses found.";
  } else {
    auto const instr        = database.create_instructor( id );
    auto const& courses_ids = instr.get_courses_ids();

    for( size_t i = 0; i < courses_ids.size(); i++ ) {
      auto course = database.create_course( courses_ids[i] );

      options_labels[i] = course.get_name() + " (" + 
                          std::to_string( courses_ids[i] ) + ")";
    }
  }

  if( no_courses ) {
    cursor_char = ' ';
  }

  display_options();
  display_cursor();
}

State* Instructor_Courses_View::update() {
  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return "
                       "to the instructor overview?" ) ) {
      return new Instructor_Overview{ terminal, database, app, id };
    }
  }

  if( input == 1 && !no_courses ) {
    auto instr = database.create_instructor( id );
    auto course_id = instr.get_courses_ids()[current_option];

    // @ToDo: switch to grade view
  }

  display_cursor();

  return nullptr;
}
}