#include "pch.hpp"

#include "students_table.hpp"

void print_records( std::vector<sj::Students_Table::Record*> const& records ) {
  if( records.empty() ) {
      std::cout << "No records found." << std::endl;
      return;
    }
    std::cout << records.size() << " records found. Data: " << std::endl;

    for( auto const* record : records ) {
      std::cout << *record << std::endl;
      std::cout << "===============================" << std::endl;
    }
}

int main() {
  try {
    sj::Students_Table students( sj::Table::Access_Type::Read );
    students.load_from_file_and_parse();

    std::cout << "Searching for students from field of study of ID 1:\n";
    auto fos = students.find_students_from_field_of_study( 1 );
    print_records( fos );

    std::cout << "\n\n\nSearching for students with ECTS deficit:\n";
    auto ects_deficit = students.find_students_with_ECTS_deficit();
    print_records( ects_deficit );

  } catch( std::exception const& ex ) {
    std::cout << "\n!!! EXCEPTION: \n" << ex.what() << std::endl;
  }
}
