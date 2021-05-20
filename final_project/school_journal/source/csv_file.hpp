#pragma once

namespace sj
{
// 1st row has column names.
// 1st column is the key.
class CSV_File final {
public:
  struct CSV_Data final {
    struct Key_Column final {
      std::string name;
      std::vector<s32> values;
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
  void move_new_data( CSV_Data&& new_data );

  [[nodiscard]]
  CSV_Data const& get_data() const;

  void parse();
  void save();

private:
  bool successfull_parsing = false;
  CSV_Data data;
  std::string path;

  [[nodiscard]]
  std::vector<std::string> split_line( std::string line );
};
}
