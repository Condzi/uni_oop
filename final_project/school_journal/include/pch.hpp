#pragma once
/*
  Precompiled header with utility stuff like
  common types or STD library includes.
*/

#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <map>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <exception>

namespace sj
{
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8  = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using f32 = float;
using f64 = double;

using Key = s32;

struct Coords final {
  s32 x = 0, y = 0;
};

#define SJ_STRINGIFY_IMPL( x ) #x
#define SJ_STRINGIFY( x ) SJ_STRINGIFY_IMPL( x )

#define SJ_THROW( message ) \
throw std::runtime_error( __FILE__ \
                          ": " SJ_STRINGIFY( __LINE__ ) \
                          " Info: " + std::string{ (message) } )

#define SJ_CHECK_FILE( file, message ) \
if( (file).bad() ) SJ_THROW( message )

[[nodiscard]] inline
s32 str_to_s32( std::string const& str ) {
  s32 value;
  auto const begin = str.c_str();
  auto const end = begin + str.size();

  auto const[p, ec] = std::from_chars( begin, end, value );
  if( ec != std::errc() ) {
    SJ_THROW( "Error when converting '" + str + "' to s32." );
  }

  return value;
}
}
