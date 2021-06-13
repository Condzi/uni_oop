#pragma once

#include "states/user_selection.hpp"

namespace sj
{
class App final {
public:
  App();
  ~App();

  void run();

private:
  Terminal terminal;
  Database db;
  bool exit = false;
  State* current_state;

  State* make_state( State::Type type );
};
}
