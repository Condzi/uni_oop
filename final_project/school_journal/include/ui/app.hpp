#pragma once

#include "states/user_selection.hpp"

namespace sj
{
class App final {
public:
  enum User_Type {
    None,
    Student,
    Instructor
  };

  App();
  ~App();

  void set_user_type( App::User_Type user_type_ );
  void set_user_id( Key id_ );

  [[nodiscard]]
  User_Type get_current_user_type() const;

  [[nodiscard]]
  Key get_current_user_id() const;

  void run();

private:
  Terminal terminal;
  Database db;
  bool exit = false;
  State* current_state;

  User_Type user_type = None;
  Key id = 0;

  State* make_state( State::Type type );
};
}
