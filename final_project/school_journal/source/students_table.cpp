#include "pch.hpp"

#include "students_table.hpp"

namespace sj
{
Students_Table::Students_Table( Table::Access_Type access_type ) :
  Table{ NAME, access_type }
{}

std::vector<Students_Table::Record*> Students_Table::find_students_from_field_of_study( s32 fod_id ) {
  std::vector<Record*> result;

  for( auto& record : records ) {
    if( record.field_of_study == fod_id ) {
      result.push_back( &record );
    }
  }

  return result;
}

std::vector<Students_Table::Record*> Students_Table::find_students_with_ECTS_deficit() {
  std::vector<Record*> result;

  for( auto& record : records ) {
    if( record.ECTS_deficit > 0 ) {
      result.push_back( &record );
    }
  }

  return result;
}

void Students_Table::parse_data_to_internal_structure()
{
  auto const& raw_csv = data_source.get_data();

  records.resize( raw_csv.key.values.size() );

  for( size_t i = 0; i < records.size(); i++ ) {
    auto& record = records[i];

    record.index = raw_csv.key.values[i];
    // Assuming that columns are always in the same order.
    // We can be more robust by doing a linear search instead
    // of simple array indexing, but it will do for now.
    record.names = raw_csv.columns[0].values[i];
    record.surname = raw_csv.columns[1].values[i];
    record.field_of_study = convert_string_to_s32( raw_csv.columns[2].values[i] );
    record.ECTS_deficit = convert_string_to_s32( raw_csv.columns[3].values[i] );
  }
}

std::ostream& operator<<( std::ostream& o, Students_Table::Record const& record ) {
  o << "Index:          " << record.index << std::endl;
  o << "Names:          " << record.names << std::endl;
  o << "Surname:        " << record.surname << std::endl;
  o << "Field of study: " << record.field_of_study << std::endl;
  o << "ECTS deficit:   " << record.ECTS_deficit;

  return o;
}
}