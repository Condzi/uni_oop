#include "pch.hpp"

#include "ui/app.hpp"

namespace sj 
{
App::App() {
  current_state = make_state( State::Type::User_Selection );
  current_state->on_switch();
  // @ToDo: This should be in a state...?
  db.set_folder( "non_prod/" );
  db.load_from_folder();
}

App::~App() {
  delete current_state;
  db.save_to_folder();
}

void App::set_user_type( App::User_Type user_type_ ) {
  user_type = user_type_;
}

void App::set_user_id( Key id_ ) {
  id = id_;
}

App::User_Type App::get_current_user_type() const {
  return user_type;
}

Key App::get_current_user_id() const {
  return id;
}

void App::run() {
  while( !exit ) {
    terminal.update();

    if( auto new_state_id = current_state->update();
        new_state_id ) {

      /*
      delete current_state;
      current_state = make_state( *new_state_id );
      */
      current_state->on_switch();
    }
  }
}


State* App::make_state( State::Type type ) {
  switch( type ) {
    case State::Type::User_Selection: {
      return new User_Selection{ terminal, db, *this };
    }
    default: {
      SJ_THROW( "Unhandled make_state type" );
    }
  }
}
}
