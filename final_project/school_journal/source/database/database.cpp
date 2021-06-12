#include "pch.hpp"

#include "database/database.hpp"

namespace sj
{
Database::Database() {
  set_folder( "database/" );
}

void Database::set_folder( std::string const &folder ) {
  courses.set_folder( folder );
  enrollments.set_folder( folder );
  fields_of_study.set_folder( folder );
  instructors.set_folder( folder );
  students.set_folder( folder );
  grades.set_folder( folder );
}

void Database::load_from_folder() {
  ready_to_read = false;
  try {
    courses.load_and_parse();
    enrollments.load_and_parse();
    fields_of_study.load_and_parse();
    grades.load_and_parse();
    instructors.load_and_parse();
    students.load_and_parse();
  } catch( std::runtime_error const& ex ) {
    debug_print(" !!! Exception thrown while loading from folder:\n%s\n", ex.what() );
    throw std::runtime_error{ex};
  }

  ready_to_read = true;
}

void Database::save_to_folder() {
  if( !unsaved_changes ) {
    return;
  }

  try {
    courses.save();
    enrollments.save();
    fields_of_study.save();
    grades.save();
    instructors.save();
    students.save();
  } catch( std::runtime_error const& ex) {
    debug_print( " !!! Exception thrown while saving to folder:\n%s\n", ex.what() );
    throw ex;
  }

  unsaved_changes = false;
}

Student Database::create_student( Key index ) const {
  throw_if_not_ready_to_read();

  std::map<std::string, std::string> row;
  try {
    row = students.get_row( index );
  } catch( std::runtime_error const& ex ) {
    SJ_THROW( "Student with index " + std::to_string( index ) + 
              " doesn't exist." );
  }

  auto s_names = row["Names"];
  auto s_surname = row["Surname"];
  auto s_field_of_study_id = convert_string_to_s32( row["Field_of_study"] );

  // 1. Find courses in enrollments.
  // 2. Find grades.

  // 1.1. Find all enrollments for this student.
  auto const& enrollment_student_ids = enrollments.get_column( "Student_Index" );
  std::vector<s32> courses_keys;
 
  s32 idx = 0;
  for( auto const& e_index : enrollment_student_ids.content ) {
    auto const e_index_as_s32 = convert_string_to_s32( e_index );
    if( e_index_as_s32 == index ) {
      courses_keys.push_back( enrollments.get_key_column().content[idx] );
    }
  
    idx++;
  }


  // 1.2. Get courses keys from enrollments table.
  std::vector<Key> s_courses;
  for( auto key : courses_keys ) {
    row = enrollments.get_row( key );
    s_courses.push_back( convert_string_to_s32( row["Course_ID"] ) );
  }

  // 2. Find grades
  std::vector<Key> s_grades_ids;
  if( s_courses.empty() ) {
    // Not really a bug -- just a student before enrollments.
    debug_print( "Student %d has no courses!\n", index );
  } else {
    auto const& grades_student_ids = grades.get_column( "Student_ID" );
    idx = 0;
    for( auto const& g_index : grades_student_ids.content ) {
      if( convert_string_to_s32( g_index ) == index ) {
         s_grades_ids.push_back( grades.get_key_column().content[idx] );
      }
      idx++;
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
    SJ_THROW( "Instructor with id " + std::to_string( id ) + 
              " doesn't exist." );
  }

  auto const i_names   = row["Names"];
  auto const i_surname = row["Surname"];

  std::vector<Key> i_courses_ids;

  auto const& instructor_id_courses = courses.get_column( "Instructor" );
  s32 idx = 0;
  for( auto const& i_id : instructor_id_courses.content ) {
    auto const i_id_as_s32 = convert_string_to_s32( i_id );
    if( i_id_as_s32 == id ) {
      i_courses_ids.push_back( courses.get_key_column().content[idx] );
    }

    idx++;
  }

  return { i_names, i_surname, std::move( i_courses_ids ), id };
}

Grade Database::create_grade( Key id ) const {
  throw_if_not_ready_to_read();
  
  std::map<std::string, std::string> row;
  try {
    row = grades.get_row( id );
  } catch( std::runtime_error const& ex ) {
    SJ_THROW( "Grade with id " + std::to_string( id ) + " doesn't exist." );
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
    SJ_THROW( "Course with id " + std::to_string( id ) + " doesn't exist." );
  }

  auto const name          = row["Name"];
  auto const ects          = convert_string_to_s32( row["ECTS"] );
  auto const instructor_id = convert_string_to_s32( row["Instructor"] );

  return { name, ects, instructor_id, id };
}

Field_Of_Study Database::create_field_of_study( Key id ) const {
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

void Database::add_grade( Grade::Value value, std::string comment,
                          Key student_index, Key course_id ) {
  throw_if_not_ready_to_read();

  if( comment.empty() ) {
    comment = "No comment.";
  } else {
    // Spaces in comments would cause problems.
    std::replace( comment.begin(), comment.end(), ',', ' ' );
  }

  if( !students.does_row_exist( student_index ) ) {
    SJ_THROW( "Can't add grade: no matching student with index " +
              std::to_string( student_index ) + "." );
  }
  if( !courses.does_row_exist( course_id ) ) {
    SJ_THROW( "Can't add grade: no matching course with id " +
              std::to_string( course_id ) + "." );
  }

  Student student = create_student( student_index );
  auto student_courses = student.get_enrolled_courses_ids();
  if( std::find( student_courses.begin(), student_courses.end(), course_id )
       == student_courses.end() ) {
       
    SJ_THROW( "Can't add grade: student " + std::to_string( student_index ) + 
              " isn't signed up for course " +
              std::to_string( course_id ) + "." );
  }

  std::map<std::string, std::string> row;

  row["Course_ID"]  = std::to_string( course_id );
  row["Student_ID"] = std::to_string( student_index );
  row["Grade"]   = Grade::value_to_string( value );
  row["Comment"] = comment;

  grades.add_row( row );

  unsaved_changes = true;
}

void Database::add_enrollment( Key student_index, Key course_id ) {
  throw_if_not_ready_to_read();

  if( !students.does_row_exist( student_index ) ) {
    SJ_THROW( "Can't add enrollment: no matching student with index " +
              std::to_string( student_index ) + "." );
  }
  if( !courses.does_row_exist( course_id ) ) {
    SJ_THROW( "Can't add enrollment: no matching course with id " +
              std::to_string( course_id ) + "." );
  }

  Student student = create_student( student_index );
  auto student_courses = student.get_enrolled_courses_ids();
  if( !( std::find( student_courses.begin(), student_courses.end(), course_id )
       == student_courses.end() ) ) {
       
    SJ_THROW( "Can't add enrollment: student " 
              + std::to_string( student_index ) +
              " already signed up for course " +
              std::to_string( course_id ) + "." );
  }

  std::map<std::string, std::string> row;

  row["Course_ID"]  = std::to_string( course_id );
  row["Student_Index"] = std::to_string( student_index );

  enrollments.add_row( row );

  unsaved_changes = true;
}

void Database::throw_if_not_ready_to_read() const {
  if( !ready_to_read ) {
    SJ_THROW( "Database is not ready for reading!" );
  }
}
}
