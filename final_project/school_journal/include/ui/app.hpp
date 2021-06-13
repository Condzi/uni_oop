#pragma once

#include "pch.hpp"
#include "state.hpp"

namespace sj
{
class App final {
public:
  App();
  ~App();

  void request_quit();
  void run();

private:
  Terminal terminal;
  Database db;
  bool exit = false;
  State* current_state;
};
}
