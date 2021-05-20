#include "pch.hpp"

#include "table.hpp"

namespace sj
{

Table::Table( std::string const& table_name, u8 access_type_ ) :
  name( table_name ),
  access_type( access_type_ )
{
  data_source.set_path( "database/" + table_name + ".csv" );
  data_source.parse();
}

Table::~Table() {
  save_pending_changes();
}

Table::Read_Only_Record Table::find_record( s32 key ) const {
  if( !is_reading_allowed() ) {
    debug_print( "Table::find_record: no reading permissions (%s).", name.c_str() );
    return {};
  }

  auto const& raw_data = data_source.get_data();

  size_t idx = 0;
  for( auto k : raw_data.key.values ) {
    if( key == k ) {
      break;
    }

    idx++;
  }

  if( idx == raw_data.key.values.size() ) {
    SJ_THROW( "can't find record for key " + std::to_string( key ) + " in table " 
              + name + "." );
  }

  Read_Only_Record record;

  record.data.emplace_back( raw_data.key.name, std::to_string( key ) );
  for( auto const& column : raw_data.columns ) {
    record.data.emplace_back( column.name, column.values[idx] );
  }

  return record;
}

std::vector<Table::Read_Only_Record> 
Table::query( std::string const& column, s32 key ) const {
  if( !is_reading_allowed() ) {
    debug_print( "Table::find_record_by_custom_key: no reading permissions (%s).", name.c_str() );
    return {};
  }

  auto const& raw_data = data_source.get_data();
  // It faster to convert key to string instead of
  // converting every column value to an integer.
  auto const key_as_str = std::to_string( key );

  // unsigned value overflow on purpose.
  size_t idx = -1;
  std::vector<size_t> ids;
  for( auto col : raw_data.columns ) {
    if( col.name == column ) {
      idx = 0;
      for( auto k : col.values ) {
        if( key_as_str == k ) {
          ids.push_back( idx );
        }
        idx++;
      }

      break;
    }
  }

  if( ids.empty() ) {
    SJ_THROW( "can't find record for key " + std::to_string( key ) + 
              " in column '" + column + "' in table " + name + "." );
  }

  std::vector<Read_Only_Record> records;
  records.reserve( ids.size() );

  for( auto id : ids ) {
    Read_Only_Record record;
    record.data.emplace_back( raw_data.key.name, std::to_string( raw_data.key.values[id] ) );
    for( auto const& column : raw_data.columns ) {
      record.data.emplace_back( column.name, column.values[id] );
    }

    records.push_back( record );
  }

  return records;
}

bool Table::is_reading_allowed() const {
  return access_type & Access_Type::Read;
}

bool Table::is_writing_allowed() const {
  return access_type & Access_Type::Write;
}

void Table::save_pending_changes()
{
  if( pending_changes && is_writing_allowed() ) {
    data_source.save();
  }
}
}