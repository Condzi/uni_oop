#pragma once

#include "csv_file.hpp"

#include "grade.hpp"
#include "student.hpp"
#include "instructor.hpp"
#include "field_of_study.hpp"

namespace sj
{
// Factory for Student and Instructor
class Database final
{
public:
  void set_folder( std::string const& folder_ );

  void load_from_folder();
  void save_to_folder();

  [[nodiscard]]
  Student create_student( Key index ) const;

  [[nodiscard]]
  Instructor create_instructor( Key id ) const;

  [[nodiscard]]
  Grade create_grade( Key id ) const;

  [[nodiscard]]
  Course create_course( Key id ) const;

  [[nodiscard]]
  Field_Of_Study create_field_of_study( Key id ) const; 

  // Checks if student and course exist, and if student is signed
  // up for given course.
  void add_grade( Grade::Value value, std::string comment, 
                  Key student_index, Key course_id );

  void add_enrollment( Key student_index, Key course_id );

private:
  CSV_File courses;
  CSV_File enrollments;
  CSV_File fields_of_study;
  CSV_File grades;
  CSV_File instructors;
  CSV_File students;

  std::string folder;
  bool unsaved_changes = false;
  bool ready_to_read = false;

  void throw_if_not_ready_to_read() const;
};
}