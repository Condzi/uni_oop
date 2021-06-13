#include "pch.hpp"

#include "ui/state.hpp"

namespace sj
{
State::State( Terminal& terminal_, Database& database_ ) :
  terminal( terminal_ ),
  database( database_ )
{}
}
