#include "pch.hpp"

#include <iostream>
#include <Windows.h>

int main() {
  HANDLE terminal_input = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE terminal_output = GetStdHandle(STD_OUTPUT_HANDLE);

  constexpr int max_term_height = 256;
  constexpr int max_term_width = 256;
  constexpr int buffer_size = 256 + 16*max_term_width*max_term_height;

  char* buffer = new char[buffer_size];
  memset( buffer, 0, buffer_size );

  bool running = true;
  while(running) {
    CONSOLE_SCREEN_BUFFER_INFO terminal;
    GetConsoleScreenBufferInfo(terminal_output, &terminal);
    int terminal_width = terminal.srWindow.Right - terminal.srWindow.Left;
    int terminal_height = terminal.srWindow.Bottom - terminal.srWindow.Top;

    //WriteConsoleA( terminal_output, buffer, terminal_width*terminal_height, 0, 0);

    while(WaitForSingleObject(terminal_input, 0) == WAIT_OBJECT_0) {
      INPUT_RECORD record;
      DWORD record_count = 0;

      ReadConsoleInput(terminal_input, &record, 1, &record_count);

      if(record_count) {
        if(record.EventType == KEY_EVENT &&
          record.Event.KeyEvent.bKeyDown &&
          record.Event.KeyEvent.wRepeatCount == 1) {

          if(record.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
            running = false;
          }
          if(record.Event.KeyEvent.wVirtualKeyCode == VK_UP ) {
            std::cout << "Up" << std::endl;
          }

        }
      }
    }
  }
}
