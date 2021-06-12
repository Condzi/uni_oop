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
      (x).set_path( folder + #x ".csv" );  \
      (x).load_from_file_and_parse()

    SJ_LOAD_TABLE( courses );
    SJ_LOAD_TABLE( enrollments );
    SJ_LOAD_TABLE( fields_of_study );
    SJ_LOAD_TABLE( grades );
    SJ_LOAD_TABLE( instructors );
    SJ_LOAD_TABLE( students );

    #undef SJ_LOAD_TABLE
  } catch( std::runtime_error const& ex ) {
    debug_print(" !!! Exception thrown while loading from folder:\n%s\n", ex.what() );
    // @ToDo: rethrow exception, with different desc?
  }

  ready_to_read = true;
}

void Database::save_to_folder() {
  // @Todo: debug only
  folder = "non_prod/";

  try {
   #define SJ_SAVE_TABLE( x )    \
      (x).set_path( folder + #x ".csv" );  \
      (x).save()

    SJ_SAVE_TABLE( courses );
    SJ_SAVE_TABLE( enrollments );
    SJ_SAVE_TABLE( fields_of_study );
    SJ_SAVE_TABLE( grades );
    SJ_SAVE_TABLE( instructors );
    SJ_SAVE_TABLE( students );

    #undef SJ_SAVE_TABLE
  } catch( std::runtime_error const& ex) {
    debug_print( " !!! Exception thrown while saving to folder:\n%s\n", ex.what() );
  }
}

Student Database::create_student( Key index ) const {
  throw_if_not_ready_to_read();

  std::map<std::string, std::string> row;
  try {
    row = students.get_row( index );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find student with id %d.\n", index );
    // @ToDo: rethrow?
  }

  auto s_names = row["Names"];
  auto s_surname = row["Surname"];
  auto s_field_of_study_id = convert_string_to_s32( row["Field_of_study"] );

  // 1. Find courses in enrollments.
  // 2. Find grades.

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

  // 1.2. Get courses keys from enrollments table.
  std::vector<Key> s_courses;
  for( auto key : enrolled_courses_keys ) {
    row = enrollments.get_row( key );
    s_courses.push_back( convert_string_to_s32( row["Course_ID"] ) );
  }

  // 2. Find grades
  std::vector<Key> s_grades_ids;
  // Not really a bug -- just a student before enrollments.
  if( s_courses.empty() ) {
    debug_print( "Student %d has no courses!\n", index );
  } else {
    for( auto const& column : grades.get_data().columns ) {
      if( column.name == "Student_ID" ) {
        s32 idx = 0;
        for( auto const& student_index : column.values ) {
          if( convert_string_to_s32( student_index ) == index ) {
            s_grades_ids.push_back( grades.get_data().key.values[idx] );
          }

          idx++;
        }
        break;
      }
    }
  }

  Student student( std::move( s_courses ), std::move( s_grades_ids ),
                   s_field_of_study_id, s_names, s_surname, index );

  return student;
}

Instructor Database::create_instructor( Key id ) const {
  throw_if_not_ready_to_read();

  std::map<std::string, std::string> row;
  try {
    row = instructors.get_row( id );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find Instructor with id %d.\n", id );
    // @ToDo: rethrow?
  }

  auto const i_names   = row["Names"];
  auto const i_surname = row["Surname"];

  std::vector<Key> i_courses_ids;

  auto const& courses_data = courses.get_data();
  for( auto const& column : courses_data.columns ) {
    if( column.name == "Instructor" ) {
      s32 idx = 0;
      for( auto const& instructor_id : column.values ) {
        auto const instructor_id_as_s32 = 
                              convert_string_to_s32( instructor_id );
        if( instructor_id_as_s32 == id ) {
          i_courses_ids.push_back( courses_data.key.values[idx] );
        }

        idx++;
      }
      break;
    }
  }

  return { i_names, i_surname, std::move( i_courses_ids ), id };
}

Grade Database::create_grade( Key id ) const {
  throw_if_not_ready_to_read();
  
  std::map<std::string, std::string> row;
  try {
    row = grades.get_row( id );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find Grade with id %d.\n", id );
    // @ToDo: rethrow?
  }

  auto const grade     = Grade::string_to_value( row["Grade"] );
  auto const comment   = row["Comment"];
  auto const course_id = convert_string_to_s32( row["Course_ID"] );

  return { grade, comment, course_id, id };
}

Course Database::create_course( Key id ) const {
  throw_if_not_ready_to_read();
  
  std::map<std::string, std::string> row;
  try {
    row = courses.get_row( id );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find Course with id %d.\n", id );
    // @ToDo: rethrow?
  }

  auto const name          = row["Name"];
  auto const ects          = convert_string_to_s32( row["ECTS"] );
  auto const instructor_id = convert_string_to_s32( row["Instructor"] );

  return { name, ects, instructor_id, id };
}

Field_Of_Study Database::create_field_of_study(Key id) const {
  throw_if_not_ready_to_read();
  
  std::map<std::string, std::string> row;
  try {
    row = fields_of_study.get_row( id );
  } catch( std::runtime_error const& ex ) {
    debug_print( "Can't find Field Of Study with id %d.\n", id );
    // @ToDo: rethrow?
  }
  
  return { row["Short_name"], row["Full_name"], id };
}

void Database::throw_if_not_ready_to_read() const {
  if( !ready_to_read ) {
    debug_print( "Database is not ready for reading!\n" );
    SJ_THROW( "Database is not ready for reading!" );
  }
}
}
