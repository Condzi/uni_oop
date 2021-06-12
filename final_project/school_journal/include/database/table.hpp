#pragma once

#include "pch.hpp"

#include "csv_file.hpp"

namespace sj
{
class Table final {
public:
  using Column = CSV_File::Column;
  using Columns = CSV_File::Columns;

  struct Key_Column final {
    std::string name;
    std::vector<Key> content;
  };

  Table( std::string const& filename );

  [[nodiscard]]
  std::string const& get_folder() const;

  [[nodiscard]]
  std::string const& get_filename() const;

  [[nodiscard]]
  std::string get_full_path() const;

  void set_folder( std::string const& folder );

  // <row name, value>
  [[nodiscard]]
  std::map<std::string, std::string> get_row( s32 key ) const;

  [[nodiscard]]
  Column const& get_column( std::string const& name ) const;

  [[nodiscard]]
  Key_Column const& get_key_column() const;

  void add_row( std::map<std::string, std::string> const& row );

  void load_and_parse();
  void save();

  [[nodiscard]]
  bool does_row_exist( Key key ) const;

private:
  std::string folder;
  std::string filename;

  CSV_File csv;
  Key_Column keys;
  Columns columns;
};
}
