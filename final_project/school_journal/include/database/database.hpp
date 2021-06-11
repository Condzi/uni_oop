#pragma once

#include "csv_file.hpp"

#include "student.hpp"
#include "instructor.hpp"

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
  Student create_student( s32 index );

  [[nodiscard]]
  Instructor create_instructor( s32 id );

  //void add_grade()

  // void add_grade, void add_enrollment...

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
};
}