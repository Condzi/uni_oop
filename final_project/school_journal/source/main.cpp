#include "pch.hpp"

#include "csv_file.hpp"

int main() {
  sj::CSV_File file;

  file.set_path( "W:/uni_oop/final_project/db_test.csv" );
  sj::debug_print( "Attempting to parse...\n" );
  file.parse();

  auto data = file.get_data();

  std::cout.setf( std::ios::right );
  std::cout << data.key.name << "    ";
  for( auto const& col : data.columns ) {
      std::cout << col.name << "\t\t";
  }
  std::cout << std::endl;

  for( size_t i = 0; i < data.key.values.size(); i++ ) {
    std::cout << data.key.values[i] << "    ";
    
    for( auto const& col : data.columns ) {
      std::cout << col.values[i] << "\t\t";
    }

    std::cout << std::endl;
  }

  file.set_path( "data_test_2.csv" );
  file.save();

  sj::debug_print( "\nAttempt completed." );
}
