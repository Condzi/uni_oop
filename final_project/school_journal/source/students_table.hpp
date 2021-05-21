#pragma once

#include "table.hpp"

namespace sj
{
class Students_Table final : public Table {
public:
  struct Record final {
    s32 index;
    std::string names;
    std::string surname;
    s32 field_of_study;
    s32 ECTS_deficit;
  };

  static inline constexpr char NAME[] = "Students";

  Students_Table( Table::Access_Type access_type );

  [[nodiscard]]
  std::vector<Record*> find_students_from_field_of_study( s32 fod_id );

  [[nodiscard]]
  std::vector<Record*> find_students_with_ECTS_deficit();

private:
  std::vector<Record> records;

  void parse_data_to_internal_structure() override;
};

std::ostream& operator<<( std::ostream& o, Students_Table::Record const& record );
}