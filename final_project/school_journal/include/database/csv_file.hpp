#pragma once

#include "pch.hpp"

namespace sj
{
class CSV_File final {
public:
  struct Column final {
    std::string name;
    std::vector<std::string> content;
  };

  using Columns = std::vector<Column>;

  CSV_File( std::string const& path_ );

  void set_path( std::string const& path_ );

  [[nodiscard]]
  Columns const& get_columns() const;

  [[nodiscard]]
  Columns& get_columns_mutable();

  [[nodiscard]]
  std::string const& get_path() const;

  [[nodiscard]]
  bool is_parsed() const;

  void load_and_parse();
  void save();

private:
  Columns columns;
  std::string path;
  bool parsed = false;

  static
  std::vector<std::string> split_line( std::string line );
};
}