#include "pch.hpp"

#include "ui/states/enrollments_courses_list.hpp"

#include "ui/states/user_selection.hpp"

namespace sj
{
Enrollments_Courses_List::Enrollments_Courses_List( Terminal& terminal_,
                                                    Database& database_,
                                                    App& app_, Key index_) :
  Basic_Menu_State{ terminal_, database_, app_, 
                    database_.get_all_courses().size() },
  index( index_ )
{
  terminal.set_title( "User_Selection -> Enrollments" );
}

void Enrollments_Courses_List::on_switch() {
  auto student = database.create_student( index );
  auto all_courses = database.get_all_courses();

  title = "Enrollments | " + std::to_string( index );

  for( size_t i = 0; i < all_courses.size(); i++ ) {
    auto const course_id = all_courses[i];
    auto const course = database.create_course( course_id );
    auto const i_str = std::to_string( i + 1 );
    auto const course_id_str = std::to_string( course_id );

    options_labels[i] = i_str + ". " + course.get_name() +  "(" +
                        course_id_str + ")";

    if( is_course_already_enrolled( course_id ) ) {
      options_labels[i] = "\x1B[37m" + options_labels[i];
      options_labels[i] += "\x1B[97m"; 
    }
  }

  display_options();
}

State * Enrollments_Courses_List::update() {
  display_cursor();

  auto input = update_input();

  if( input == -1 ) {
    if( yes_no_prompt( "Are you sure you want to return to the "
                       "user selection screen?" ) ) {
      return new User_Selection{ terminal, database, app };
    }
  } else if( input == 1 ) {
    auto const course_id = database.get_all_courses()[current_option];
    if( is_course_already_enrolled( course_id ) ) {
      prompt_error( "You are already enrolled in this course." );
      display_options();
    } else {
      try {
        database.add_enrollment( index, course_id );
      } catch ( std::exception const& ex ) {
        prompt_error( std::string{ "Couldn't enroll for the course. Details: " } +
                      ex.what() );
        display_options();
        return nullptr;
      }

      prompt_error( "You successfully enrolled for course (" + 
                    std::to_string( course_id ) + ")." );

      return new Enrollments_Courses_List{ terminal, database, app, index };
    }
  }

  return nullptr;
}

bool Enrollments_Courses_List::is_course_already_enrolled( Key course_id ) const {
  auto student = database.create_student( index );
  auto enrolled = student.get_enrolled_courses_ids();
  auto it = std::find( enrolled.begin(), enrolled.end(), course_id );

  return !(it == enrolled.end());
}
}
