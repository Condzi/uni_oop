#pragma once

#include <string>

using std::string;

class Circle final {
public:
  Circle();
  Circle( string const& color, double radius );

  void set_color( string const& color );
  void set_radius( double radius );

  [[nodiscard]]
  auto get_color() const -> string const&;
  [[nodiscard]]
  auto get_radius() const -> double;
  [[nodiscard]]
  auto get_area() const -> double;

  [[nodiscard]]
  auto compare_area( Circle const& other ) const -> Circle const&;

  void describe() const;

private:
  string color_;
  double radius_;
};
