#include "circle.hpp"

// For M_PI. <cmath> doesn't expose the M_PI macro on msvc.
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

using std::cout;

Circle::Circle() {
  color_  = "white";
  radius_ = 1; 
}

Circle::Circle( string const& color, double radius ) {
  color_  = color;
  radius_ = radius;
}

void Circle::set_color( string const& color ) {
  color_ = color;
}

void Circle::set_radius( double radius ) {
  radius_ = radius;
}

auto Circle::get_color() const -> string const& {
  return color_;
}

auto Circle::get_radius() const -> double {
  return radius_;
}

auto Circle::get_area() const -> double {
  return M_PI*radius_*radius_;
}

auto Circle::compare_area( Circle const& other ) const -> Circle const& {
  // We can avoid call to get_area, since bigger radius results in 
  // bigger area. Unless we allow negative radius.
  if( get_radius() > other.get_radius() ) {
    return *this;
  }
  return other;
}

void Circle::describe() const {
  cout << "Circle::color_     = \"" << color_   << "\"\n";
  cout << "Circle::radius_    = "   << radius_  << '\n';
  cout << "Circle::get_area() = "   << get_area();
}
