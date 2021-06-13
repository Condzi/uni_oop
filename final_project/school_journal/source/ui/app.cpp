#include "pch.hpp"

#include "ui/app.hpp"
#include "ui/states/user_selection.hpp"

namespace sj 
{
App::App() {
  current_state = new User_Selection{ terminal, db, *this };
  current_state->on_switch();
  // @ToDo: This should be in a state...?
  // Also, prompt an error when failed to load.
  db.set_folder( "non_prod/" );
  // @ToDo: should be in a try/catch
  db.load_from_folder();
}

App::~App() {
  delete current_state;
  // @ToDo: should be in a try/catch
  db.save_to_folder();
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
