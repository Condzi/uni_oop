#include "pch.hpp"

#include "table.hpp"

int main() {
  try {
    sj::Table students( "Grades", sj::Table::Access_Type::Read );
    auto records = students.query( "Student_ID", 100101 );

    if( records.empty() ) {
      std::cout << "No records found." << std::endl;
      return 0;
    }
    std::cout << records.size() << " records found. Data: " << std::endl;

    for( auto const& record : records ) {
      for( auto const& col : record.data ) {
        std::cout << " _________________" << std::endl;
        std::cout << " " << col.name << ": " << col.value << std::endl;
      }
      std::cout << "===============================" << std::endl;
    }

  } catch( std::exception const& ex ) {
    std::cout << "\n!!! EXCEPTION: \n" << ex.what() << std::endl;
  }
}
