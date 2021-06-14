#include "pch.hpp"

#include "ui/app.hpp"
#include "ui/states/user_selection.hpp"

namespace sj 
{
App::App() {
  current_state = new User_Selection{ terminal, db, *this };
  current_state->on_switch();

  try {
    db.set_folder( "database/" );
    db.load_from_folder();
  } catch( std::exception const& ex ) {
    terminal.clear_screen();
    terminal.set_pen_coords( {0,0} );
    std::cout << "Can't load the database. Details:\n";
    std::cout << ex.what() << std::endl;
    std::cout << "\n\nExiting.";
    exit = true;
  }
}

App::~App() {
  delete current_state;
  
  try {
    db.save_to_folder();
  } catch( std::exception const& ex ) {
    terminal.set_pen_coords( {0,0} );
    terminal.clear_screen();
    std::cout << "Can't save the database. Details: \n";
    std::cout << ex.what() << std::endl;
    std::cout << "\n\nExiting.";
  }
}

void App::request_quit() {
  exit = true;
}

void App::run() {
  while( !exit ) {
    terminal.update();

    if( auto* new_state = current_state->update();
        new_state ) {
       
      delete current_state;
      current_state = new_state;
      current_state->on_switch();
    }
  }
}
}
