#include "pch.hpp"

#include "database/csv_file.hpp"

namespace sj
{
CSV_File::CSV_File( std::string const& path_ ) :
  path( path_ ),
  parsed( false )
{}

void CSV_File::set_path( std::string const &path_ ) {
  path = path_;
}

CSV_File::Columns const& CSV_File::get_columns() const  {
  return columns;
}

CSV_File::Columns& CSV_File::get_columns_mutable()  {
  return columns;
}

std::string const& CSV_File::get_path() const {
  return path;
}

bool CSV_File::is_parsed() const {
  return parsed;
}

void CSV_File::load_and_parse() {
  parsed = false;

  std::ifstream file( path );
  if( !file.is_open() ) {
    SJ_THROW( "Unable to open file '" + path + "' for reading." );
  }
  SJ_CHECK_FILE( file, "error after opening '" + path + "' for reading." );

  std::vector<std::string> column_names;
  std::string line;

  if( std::getline( file, line ) ) {
    column_names = split_line( line );
    columns.reserve( column_names.size() );

    // UTF files seem to put some bytes at the beginning
    // of the file, which I don't care about.
    auto& first_name = column_names.front();
    for( s64 i = 0; i < first_name.size(); ) {
      if( first_name[i] < 1 ) {
        first_name.erase( i, 1 );
      } else {
        i++;
      }
    }

    for( auto const& name : column_names ) {
      columns.emplace_back( name, std::vector<std::string>{} );
    }

    debug_print( "CSV_File::parse(): found %d columns in '%s'.", column_names.size(), path.c_str() );
  }
  SJ_CHECK_FILE( file, "Error after reading first line from '" + path + "'.");
  
  // expected number of values in a row/line.
  auto const expected_no = column_names.size();
  auto const expected_no_str = std::to_string( expected_no );
  size_t line_number = 1;

  while( std::getline( file, line ) ) {
    line_number++;
    auto const line_number_str = std::to_string( line_number );

    if( line.empty() ) {
      continue;
    }

    auto const values_in_line = split_line( line );
    if( values_in_line.size() != expected_no ) {
      SJ_THROW( "Number of expected values in file '" + path + 
                ", line " + line_number_str + " is different from "
                "expected " + expected_no_str + "." );
    }

    for( size_t i = 0; i < values_in_line.size(); i++ ) {
      columns[i].content.push_back( values_in_line[i] );
    }
  }
  
  SJ_CHECK_FILE( file, "Error reading file from '" + path + "'." );

  parsed = true;
}

void CSV_File::save() {
  if( !parsed ) {
    // This functionality wasn't needed in this project,
    // so I don't bother with implementing it.
    SJ_THROW( "Creating CSV files which weren't parsed first is not supported." );
  }

  std::ofstream file( path );
  if( !file.is_open() ) {
    SJ_THROW( "Unable to open file '" + path + "' for writing." );
  }

  SJ_CHECK_FILE( file, "Error after opening '" + path + "' for writing." );

  std::ostringstream line_builder;
  
  for( auto const& column : columns ) {
    line_builder << ',' << column.name;
  }
  line_builder << '\n';

  // str_to_save is used to remove ',' from the beginning of the lines
  std::string str_to_save = line_builder.str();
  str_to_save.erase( 0, 1 );
  file << str_to_save;

  // Every column has the same number of rows.
  auto const number_of_value_rows = columns.front().content.size();
  for( size_t i = 0; i < number_of_value_rows; i++ ) {
    line_builder.str("");

    for( auto const& column : columns ) {
      line_builder << ',' << column.content[i];
    }
    line_builder << '\n';
    
    str_to_save = line_builder.str();
    str_to_save.erase( 0, 1 );

    file << str_to_save;
  }

  SJ_CHECK_FILE( file, "Error after writing to '" + path + "'." );
}

 
std::vector<std::string> CSV_File::split_line( std::string line ) {
  std::vector<std::string> words;

  if( line.empty() ) {
    SJ_THROW( "Empty line passed to split_line." );
  }

  std::istringstream ss( line );

  std::string splitted;
  while( std::getline( ss, splitted, ',' ) ) {
    words.push_back( splitted );
  }

  return words;
}
}
