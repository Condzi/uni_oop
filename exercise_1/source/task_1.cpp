#include <iostream>
#include <cinttypes>

// short for "signed 32 bit"
using s32 = std::int32_t;

void print_tree( std::int32_t height );

[[nodiscard]]
s32 get_input();

int main() {
  s32 input;

  std::cout << "----- Christmas Tree Printer 2021 -----\n";

  do {
    input = get_input();
    print_tree( input );
  } while( input != 0 );

  std::cout << "Shutting down...\n";
}


void print_tree( s32 height ) {
  for( s32 i = 1; i <= height; i++ ) {
    for( s32 j = height - 1; j >= i; j-- ) {
      std::cout << " ";
    }

    for( s32 j = 1; j <= i; j++ ) {
      std::cout << "\30 ";
    }
    std::cout << std::endl;
  }

  for( s32 i = 0; i < 2; i++ ) {
    for( s32 j = 0; j < height-2; j++ ) {
      std::cout << " ";
    }
    for( s32 j = 0; j < 3; j++ ) {
      std::wcout << (wchar_t)219;
    }
    std::cout << std::endl;
  }
}

std::int32_t get_input() {
  std::int32_t input;
  bool valid_input = false;

  std::cout << "\nSpecify the tree height (range is <5; 20>) or 0 for exit:\n> ";

  while( !valid_input ) {
    if( !(std::cin >> input) ) {
      std::cout << "Incorrect input. Try again.\n> ";
      std::cin.clear();
      std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
      continue;
    }
    if( (input < 5 || input > 20) && input != 0 ) {
      std::cout << "Input is not in the valid range: <5, 20>. Try again.\n> ";
      continue;
    }

    valid_input = true;
  }

  return input;
}
