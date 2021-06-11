#pragma once

#include "pch.hpp"

namespace sj
{
class Database_Element {
public:
  Database_Element( u64 key_ ) : key( key_ ) {}
  virtual ~Database_Element() = default;

  [[nodiscard]]
  u64 get_key() const { return key; }

private:
  u64 key;
};
}