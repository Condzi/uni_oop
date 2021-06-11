#pragma once

#include "csv_file.hpp"

namespace sj
{
// Factory for Student and Instructor
class Database
{
public:
  void set_folder( std::string const& path );

  void load_from_folder();
  void save_to_folder();

  // void add_grade, void add_enrollment...

private:
  struct {
    CSV_File courses;
    CSV_File enrollments;
    CSV_File fields_of_study;
    CSV_File grades;
    CSV_File grades_comments;
    CSV_File instructors;
    CSV_File students;
  } files;

  std::string folder;
  bool unsaved_changes = false;
};
}