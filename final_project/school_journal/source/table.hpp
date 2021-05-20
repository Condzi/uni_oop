#pragma once

#include "csv_file.hpp"

namespace sj
{
class Table
{
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

  [[nodiscard]]
  Read_Only_Record find_record( s32 key ) const;

  [[nodiscard]] std::vector<Read_Only_Record> 
  query( std::string const& column, s32 key ) const;

  [[nodiscard]]
  bool is_reading_allowed() const;
  [[nodiscard]]
  bool is_writing_allowed() const;

  void save_pending_changes();

private:
  std::string const name;
  u8 access_type;
  bool pending_changes;
  CSV_File data_source;
};
}
