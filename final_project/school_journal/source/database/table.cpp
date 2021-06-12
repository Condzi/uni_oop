#include "pch.hpp"

#include "database/table.hpp"

namespace sj
{
Table::Table( std::string const& filename_ ) :
  csv( filename_ + ".csv" ),
  folder( "" ),
  filename( filename_ )
{}

std::string const& Table::get_folder() const {
  return folder;
}

std::string const& Table::get_filename() const {
  return filename;
}

std::string Table::get_full_path() const {
  return folder + filename + ".csv";
}

void Table::set_folder( std::string const& folder_ ) {
  folder = folder_;

  csv.set_path( get_full_path() );
}

std::map<std::string, std::string> Table::get_row( s32 key ) const {
  if( !csv.is_parsed() ) {
    SJ_THROW( "Attempt to get_row from '" + csv.get_path() + "', which was not successfully parsed." );
  }

  auto it = std::find( keys.content.begin(), keys.content.end(), key );
  if( it == keys.content.end() ) {
    SJ_THROW( "Can't get_row for key " + std::to_string( key ) + " "
              "from " + csv.get_path() + "." );
  }
  
  auto idx = std::distance( keys.content.begin(), it );

  std::map<std::string, std::string> result;
  for( auto const& column : columns ) {
    result[column.name] =  column.content[idx];
  }

  return result;
}

Table::Column const& Table::get_column( std::string const& name ) const {
  auto it = std::find_if( columns.begin(), columns.end(), [&]( auto& c ) {
    return c.name == name;
  });

  if( it == columns.end() ) {
    SJ_THROW( "Can't find column '" + name + "' in file '" + 
              csv.get_path() + "'." );
  }

  return *it;
}

Table::Key_Column const& Table::get_key_column() const {
  return keys;
}

bool Table::does_row_exist( Key key ) const {
  auto const begin = keys.content.cbegin();
  auto const end   = keys.content.cend();

  return !(std::find( begin, end, key ) == end);
}

void Table::add_row( std::map<std::string, std::string> const& row ) {
  if( !csv.is_parsed() ) {
    SJ_THROW( "Can't add_row before a parsing (" + csv.get_path() + ")." );
  }

  for( auto& column : columns ) {
    std::string str_to_put;
    try {
      str_to_put = row.at( column.name ); 
    } catch( std::exception const& ex ) {
      // Literally a bug that should be catched in tests. 
      // That's why we're not removing previously added rows.
      SJ_THROW( "Can't add_row -- no value for '" + column.name + "'." );
    }

    column.content.emplace_back( str_to_put );
  }

  // For simplicity - assuming that the last value
  // in key column is the biggest one ever.
  // Therefore, also assuming removing a row is not possible.
  auto last_key_value = keys.content.back();
  last_key_value++;
  keys.content.push_back( last_key_value );
}

void Table::load_and_parse() {
  csv.load_and_parse();

  auto const& raw = csv.get_columns();
  // First column in every CSV is a key.
  keys.content.reserve( raw.front().content.size() );

  keys.name = raw.front().name;
  for( auto const& key_str : raw.front().content ) {
    keys.content.emplace_back( str_to_s32( key_str ) );
  }

  columns.resize( raw.size() - 1 );

  std::copy( raw.begin() + 1, raw.end(), columns.begin() );
}

void Table::save() {
  auto& raw = csv.get_columns_mutable();
  // Since we're only adding rows, we can just add the new ones.
  auto const rows_to_add = keys.content.size() - raw.front().content.size();

  for( size_t i = raw.front().content.size(); i < keys.content.size(); i++ ) {
    raw.front().content.push_back( std::to_string( keys.content[i] ) );

    for( size_t j = 1; j < raw.size(); j++ ) {
      raw[j].content.push_back( columns[j - 1].content[i] );
    }
  }

  csv.save();
}
}
