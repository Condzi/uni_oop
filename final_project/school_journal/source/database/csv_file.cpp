#include "pch.hpp"

#include "database/csv_file.hpp"

namespace sj
{
void CSV_File::set_path( std::string const &new_path ) {
  path = new_path;
}

void CSV_File::move_new_data( CSV_Data &&new_data )  {
  data = std::move( new_data );
}

CSV_File::CSV_Data const& CSV_File::get_data() const {
  if( !successfull_parsing ) {
    SJ_THROW( "Attempt to get_data from '" + path + "', which was not successfully parsed." );
  }

  return data;
}

std::map<std::string, std::string> CSV_File::get_row( s32 key ) const {
  if( !successfull_parsing ) {
    SJ_THROW( "Attempt to get_row from '" + path + "', which was not successfully parsed." );
  }

  std::map<std::string, std::string> result;
  
  size_t idx = 0;
  for( auto k : data.key.values ) {
    if( k == key )
      break;
    idx++;
  }

  if( idx == data.key.values.size() ) {
    SJ_THROW( "Can't get_row for key " + std::to_string( key ) + " "
              "from " + path + "." );
  }
  
  for( auto const& column : data.columns ) {
    result[column.name] =  column.values[idx];
  }

  return result;
}

void CSV_File::load_from_file_and_parse() {
  successfull_parsing = false;

  std::ifstream file( path );
  SJ_CHECK_FILE( file, "error after attempting to open '" + path + "'." );

  std::vector<std::string> column_names;
  std::string line;

  if( std::getline( file, line ) ) {
    SJ_CHECK_FILE( file, "error after attempting to read first line from '" + path + "'.");
    column_names = split_line( line );

    data.key.name = column_names[0];
    // UTF files seem to put some bytes at the beginning
    // of the file, which I don't care about.
    for( s64 i = 0; i < data.key.name.size(); ) {
      if( data.key.name[i] < 1 ) {
        data.key.name.erase( i, 1 );
      } else {
        i++;
      }
    }

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

    if( line.empty() ) {
      continue;
    }

    auto const values_in_line = split_line( line );
    if( values_in_line.size() != expected_number_of_values_in_line ) {
      SJ_THROW( "number of expected values in line " + line_number_str + " is different." );
    }

    auto const key_value = convert_string_to_s32( values_in_line[0] );
    data.key.values.push_back( key_value );

    for( size_t i = 1; i < values_in_line.size(); i++ ) {
      data.columns[i - 1].values.push_back( values_in_line[i] );
    }
  }
  
  SJ_CHECK_FILE( file, "error reading file from '" + path + "'." );

  successfull_parsing = true;
}

void CSV_File::save() {
  std::ofstream file( path );
  SJ_CHECK_FILE( file, "error after attempting to open '" + path + "'." );

  // Every column has the same number of rows.
  auto const number_of_value_rows = data.key.values.size();

  std::ostringstream line_builder;
  
  line_builder << data.key.name;
  for( auto const& column : data.columns ) {
    line_builder << ',' << column.name;
  }
  line_builder << '\n';

  if( !(file << line_builder.str()) ) {
    SJ_THROW( "Writing to file failed when writing column names." );
  }

  for( size_t i = 0; i < number_of_value_rows; i++ ) {
    line_builder.str("");

    line_builder << data.key.values[i];
    for( auto const& column : data.columns ) {
      line_builder << ',' << column.values[i];
    }
    line_builder << '\n';

    if( !(file << line_builder.str()) ) {
      SJ_THROW( "Writing to file failed at line " + std::to_string( i ) + "." );
    }
  }
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
