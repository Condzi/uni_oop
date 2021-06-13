#pragma once

#include "database/database.hpp"
#include "terminal.hpp"

namespace sj
{
class App;

class State {
public:
  State( Terminal& terminal_, Database& database_, App& app_ );
  virtual ~State() = default;

  virtual 
  void on_switch() = 0;

  // Optionally, returns state to which wants to change.
  [[nodiscard]] virtual 
  State* update() = 0;

protected:
  Terminal& terminal;
  Database& database;
  App& app;
};
}