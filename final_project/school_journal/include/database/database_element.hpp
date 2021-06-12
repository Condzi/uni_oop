#pragma once

#include "pch.hpp"

namespace sj
{
class Database_Element {
public:
  Database_Element( Key key_ ) : key( key_ ) {}
  virtual ~Database_Element() = default;

  [[nodiscard]]
  Key get_key() const { return key; }

private:
  Key key;
};
}