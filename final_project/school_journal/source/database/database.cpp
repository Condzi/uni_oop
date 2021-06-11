#include "pch.hpp"

#include "database/database.hpp"

namespace sj
{
void Database::set_folder( std::string const& folder_ ) {
  // @ToDo: check if folder exist
  folder = folder_;
}

void Database::load_from_folder() {

  try {
    #define SJ_LOAD_TABLE( x )    \
      (x).set_path( folder + #x );  \
      (x).load_from_file_and_parse()

    SJ_LOAD_TABLE( courses );
    SJ_LOAD_TABLE( enrollments );
    SJ_LOAD_TABLE( fields_of_study );
    SJ_LOAD_TABLE( grades );
    SJ_LOAD_TABLE( instructors );
    SJ_LOAD_TABLE( students );

    #undef SJ_LOAD_TABLE
  } catch( std::runtime_error const& ex ) {
    debug_print(" !!! Exception thrown:\n%s\n", ex.what() );
    // @ToDo: rethrow exception, with different desc?
  }

  ready_to_read = true;
}

Student Database::create_student( s32 index ) {

  // (underscores used to avoid ambiguity with fields)

  std::map<std::string, std::string> temp;
  try {
    temp = students.get_row( index );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find student with id %d.\n", index );
    // @ToDo: rethrow?
  }

  auto s_names = temp["Names"];
  auto s_surname = temp["Surname"];
  auto s_field_of_study_id = convert_string_to_s32( temp["Field_of_study"] );

  // 1. Find courses in enrollments.
  // 2. If courses found, find grades.
  std::vector<Course> s_courses;

  // 1.1. Find all enrollments for this student.
  auto const& enrollment_data = enrollments.get_data();
  std::vector<s32> enrolled_courses_keys;
  for( auto const& column : enrollment_data.columns ) {
    if( column.name == "Student_Index" ) {
      s32 idx = 0;
      for( auto const& enrollment_index : column.values ) {
        auto const enrollment_index_as_s32 = 
                              convert_string_to_s32( enrollment_index );
        if( enrollment_index_as_s32 == index ) {
          enrolled_courses_keys.push_back( enrollment_data.key.values[idx] );
        }

        idx++;
      }
      break;
    }
  }

  // 1.2. Get courses keys from enrollments.
  std::vector<s32> courses_keys;
  for( auto key : enrolled_courses_keys ) {
    temp = enrollments.get_row( key );
    courses_keys.push_back( convert_string_to_s32( temp["Course_ID"] ) );
  }

  // 1.3. Create courses.
  for( auto key : courses_keys ) {
    temp = courses.get_row( key );

    auto course_name = temp["Name"];
    auto course_ects = convert_string_to_s32( temp["ECTS"] );
    auto course_id = key;

    auto instructor_id = convert_string_to_s32( temp["Instructor"] );

    temp = instructors.get_row( instructor_id );
    auto instructor_names = temp["Names"];
    auto instructor_surname = temp["Surname"];

    Instructor instructor( instructor_names, 
                           instructor_surname, instructor_id );

    s_courses.emplace_back( course_name, course_ects, instructor, course_id );
  }

  temp = fields_of_study.get_row( s_field_of_study_id );
  Field_Of_Study s_field_of_study( temp["Short_name"], temp["Full_name"], s_field_of_study_id );


  if( s_courses.empty() ) {
    debug_print( "Student %d has no courses!\n", index );
  }

  std::vector<s32> s_grades_keys;
  for( auto const& column : grades.get_data().columns ) {
    if( column.name == "Student_ID" ) {
      s32 idx = 0;
      for( auto const& student_index : column.values ) {
        if( convert_string_to_s32( student_index ) == index ) {
          s_grades_keys.push_back( grades.get_data().key.values[idx] );
        }

        idx++;
      }
      break;
    }
  }

  std::vector<Grade> s_grades;
  for( auto key : s_grades_keys ) {
    temp = grades.get_row( key );

    auto course_id = convert_string_to_s32( temp["Course_ID"] );
    auto grade = Grade::string_to_value( temp["Grade"] );
    auto comment = temp["Comment"];

    auto course = std::find_if( s_courses.begin(), s_courses.end(), 
                                [&]( auto const& c ) {
        return c.get_key() == course_id;
    } );

    s_grades.emplace_back( grade, comment, *course, key );
  }

  Student student( std::move( s_courses ), std::move( s_grades ),
                   s_field_of_study, s_names, s_surname, index );

  return student;
}
}
