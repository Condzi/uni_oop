#pragma once

#include "pch.hpp"

namespace sj
{
// @ToDo: Rename to CSVTable ?
// 1st row has column names.
// 1st column is the key.
class CSV_File final {
public:
  struct CSV_Data final {
    struct Key_Column final {
      std::string name;
      std::vector<Key> values;
    };

    struct Column final {
      std::string name;
      std::vector<std::string> values;
    };

    // We're not using std::map, because we care about
    // the order of the columns. Plus, it's not a big overhead
    // since we have small number of columns.
    Key_Column key;
    std::vector<Column> columns;
  };

  CSV_File() = default;

  void set_path( std::string const& new_path );
  // @ToDo: Unused?
  void move_new_data( CSV_Data&& new_data );

  [[nodiscard]]
  CSV_Data const& get_data() const;

  // <row name, value>
  [[nodiscard]]
  std::map<std::string, std::string> get_row( s32 key ) const;

  [[nodiscard]]
  bool does_row_exist( Key key ) const;

  void add_row( std::map<std::string, std::string> const& row );

  void load_from_file_and_parse();
  void save();

private:
  bool successfull_parsing = false;
  CSV_Data data;
  std::string path;

  [[nodiscard]]
  std::vector<std::string> split_line( std::string line ) const;
};
}
