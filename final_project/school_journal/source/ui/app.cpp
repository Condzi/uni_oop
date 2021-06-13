#include "pch.hpp"

#include "ui/app.hpp"

namespace sj 
{
App::App() {
  current_state = make_state( State::Type::User_Selection );
  current_state->on_switch();
  // @ToDo: This should be in a state...?
  // db.set_folder( "non_prod/" );
  // db.load_from_folder();
}

App::~App() {
  delete current_state;
}

void App::run() {
  while( !exit ) {
    terminal.update();

    if( auto new_state_id = current_state->update();
        new_state_id ) {
      
      terminal.set_title( "New state id request: " + std::to_string   ( *new_state_id ) );
      /*
      delete current_state;
      current_state = make_state( *new_state_id );
      current_state->on_switch();
      */
    }
  }
}


State* App::make_state( State::Type type ) {
  switch( type ) {
    case State::Type::User_Selection: {
      return new User_Selection{ terminal, db };
    }
    default: {
      SJ_THROW( "Unhandled make_state type" );
    }
  }
}
}
