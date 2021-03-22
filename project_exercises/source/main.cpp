#include <iostream>
using std::cout;

#include "circle.hpp"

int main()
{
  Circle c_default;
  Circle c{"red", 300.12 };

  c_default.describe();
  cout << "\n----------\n";
  c.describe();
}