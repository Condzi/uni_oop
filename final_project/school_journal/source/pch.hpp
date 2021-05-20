#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
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

constexpr bool ENABLE_DEBUG_PRINT = true;

template <typename ...TArgs>
void debug_print( char const* fmt, TArgs ...args ) {
  if constexpr ( ENABLE_DEBUG_PRINT )
  {
    if constexpr ( sizeof...(TArgs) == 0 ){
      std::puts( fmt );
    } else {
      char buffer[512] = {};
      s32 written = std::snprintf( buffer, 512 - 1, fmt,
                              args... );
      assert( written < 512 );

      std::puts( buffer );
    }
  }
}

#define SJ_STRINGIFY_IMPL( x ) #x
#define SJ_STRINGIFY( x ) SJ_STRINGIFY_IMPL( x )

#define SJ_THROW( message ) \
throw std::runtime_error( "File: " __FILE__ ":" SJ_STRINGIFY(__LINE__) ":\n" + std::string{ (message) } )

#define SJ_CHECK_FILE( file, message ) \
if( !(file).good() ) SJ_THROW( message )
}
