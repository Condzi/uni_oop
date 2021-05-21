#pragma once

#include "csv_file.hpp"

namespace sj
{
class Table {
public:
  struct Read_Only_Record final {
    struct Column final {
      std::string name;
      std::string value;
    };

    std::vector<Column> data;
  };

  enum Access_Type : u8 { None = 0, Read = 1, Write = 2 };

  Table( std::string const& table_name, u8 access_type_ );
  Table( Table const& ) = delete;
  Table( Table&& ) = delete;
  Table& operator=(Table const&) = delete;
  Table&& operator=(Table const&&) = delete;

  virtual ~Table();

  void load_from_file_and_parse();

  [[nodiscard]]
  Read_Only_Record find_record( s32 key ) const;

  // @ToDo: not needed?
  // Key as string? There's no reason for it to be an integer
  // Also, we could accept a list of <column, key> pairs to search
  // for.
  [[nodiscard]] std::vector<Read_Only_Record> 
  query( std::string const& column, s32 key ) const;

  [[nodiscard]]
  bool is_reading_allowed() const;
  [[nodiscard]]
  bool is_writing_allowed() const;

  void save_pending_changes();

protected:
  bool pending_changes;
  CSV_File data_source;

  virtual void parse_data_to_internal_structure() = 0;

private:
  std::string const name;
  u8 const access_type;
};
}
