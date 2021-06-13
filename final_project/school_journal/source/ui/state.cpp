#include "pch.hpp"

#include "ui/state.hpp"

namespace sj
{
State::State( Terminal& terminal_, Database& database_, App& app_ ) :
  terminal( terminal_ ),
  database( database_ ),
  app( app_ )
{}
}
