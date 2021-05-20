#include "pch.hpp"

#include "csv_file.hpp"

namespace sj
{
void CSV_File::set_path( std::string const &new_path ) {
  path = new_path;
}

void CSV_File::move_new_data( CSV_Data &&new_data )  {
  data = std::move( new_data );
}

CSV_File::CSV_Data const& CSV_File::get_data() {
  if( !successfull_parsing ) {
    SJ_THROW( "Attempt to get_data from '" + path + "', which was not successfully parsed." );
  }

  return data;
}

void CSV_File::parse() {
  std::ifstream file( path );
  SJ_CHECK_FILE( file, "error after attempting to open '" + path + "'." );

  std::vector<std::string> column_names;
  std::string line;

  if( std::getline( file, line ) ) {
    SJ_CHECK_FILE( file, "error after attempting to read first line from '" + path + "'.");
    column_names = split_line( line );

    data.key.name = column_names[0];
    data.columns.resize( column_names.size() - 1 );
    for( size_t i = 0; i < column_names.size() - 1; i++ ) {
      data.columns[i].name = column_names[i + 1];
    }

    debug_print( "CSV_File::parse(): found %d columns in '%s'.", column_names.size(), path.c_str() );
  }

  auto const expected_number_of_values_in_line = column_names.size();
  size_t line_number = 1;

  while( std::getline( file, line ) ) {
    line_number++;
    auto const line_number_str = std::to_string( line_number );
    SJ_CHECK_FILE( file, "error after attempting to read line " + 
                          line_number_str + " from '" + path + "'.");

    if( line.empty() ) {
      continue;
    }

    auto const values_in_line = split_line( line );
    if( values_in_line.size() != expected_number_of_values_in_line ) {
      SJ_THROW( "number of expected values in line " + line_number_str + " is different." );
    }

    auto const key_str = values_in_line[0];
    char const* key_str_begin = key_str.c_str();
    char const* key_str_end = key_str_begin + key_str.size();
    s32 key_value = -1;
    auto const[p, ec] = std::from_chars( key_str_begin, key_str_end, key_value );

    if( ec != std::errc() ) {
      SJ_THROW( "error occurred when attempting to convert " + key_str + " to an s32." );
    }

    data.key.values.push_back( key_value );

    for( size_t i = 1; i < values_in_line.size(); i++ ) {
      data.columns[i - 1].values.push_back( values_in_line[i] );
    }
  }

  successfull_parsing = true;
}

std::vector<std::string> CSV_File::split_line( std::string line ) {
  std::vector<std::string> words;

  line.erase( std::remove_if( line.begin(), line.end(), [](char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }), line.end() );

  std::istringstream ss( line );

  std::string splitted;
  while( std::getline( ss, splitted, ',') ) {
    words.push_back( splitted );
  }

  return words;
}
}
