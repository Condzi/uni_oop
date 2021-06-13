#include "pch.hpp"

#include "ui/app.hpp"

int main() {
  try {
    sj::App app;
    app.run();
  } catch ( std::exception const& ex ) {
    sj::Terminal t;
    t.clear_screen();
    t.set_pen_coords( {5,2} );
    t.pen_write( "Something unexpected happened!" );
    t.move_pen( {-2, 1} );
    t.pen_write( "Details: " );
    t.move_pen( {0, 1} );
    t.pen_write( ex.what() );
    t.wait_for_enter();
  }
}
