#pragma once

#include "state.hpp"

namespace sj
{
class Basic_Menu_State : public State {
public:
  Basic_Menu_State( Terminal& terminal_, Database& database_, App& app_, s32 options_count_ );
  virtual ~Basic_Menu_State() = default;

protected:
  static inline constexpr s32 MAX_OPTIONS_COUNT = 16;
  std::string title;
  // [0, options_count)
  s32 const options_count;
  s32 current_option = 0;
  std::array<std::string, MAX_OPTIONS_COUNT> options_labels;

  char cursor_char = '>';

  void display_options();
  void display_cursor();
  // Returns:
  //	-1 on Terminal::Input::Escape,
  //	 0 when nothing was pressed
  //     1 on Terminal::Input::Enter
  s32 update_input();
  
  template <typename T>
  void ask_for_input( std::string const& label, T& data );

  void prompt_error( std::string const& label );

private:
  static inline constexpr Coords TITLE_POS{ 10, 2 };
  static inline constexpr Coords OPTIONS_POS{ TITLE_POS.x - 4, TITLE_POS.y + 2 };
  static inline constexpr Coords CURSOR_STARTING_POS{ OPTIONS_POS.x - 2, OPTIONS_POS.y };
  Coords const INPUT_POS;
  Coords const ERROR_POS;

  bool cursor_needs_redrawing = true;
};

template <typename T>
void Basic_Menu_State::ask_for_input( std::string const& label, 
                                      T& data ) {
  terminal.set_pen_coords( INPUT_POS );
  terminal.pen_write( label );

  if( !(std::cin >> data) ) {
    prompt_error( "Invalid input. Try again." );
    display_options();
    display_cursor();
    ask_for_input( label, data );
  }
}
}