#include <fstream>
#include <iostream>
#include <string_view>
#include <filesystem>

void print_usage();
[[nodiscard]]
bool does_file_exist( std::string_view filename );
[[nodiscard]]
bool open_file( std::fstream* file, std::string_view path, std::ios_base::openmode flags );


int main() {
  if( __argc != 4 ) {
    print_usage();
    return 0;
  }

  std::string_view file_path_a, file_path_b, file_path_result;
  file_path_a = __argv[1];
  file_path_b = __argv[2];
  file_path_result = __argv[3];

  if( !does_file_exist( file_path_a ) ||
      !does_file_exist( file_path_b ) ) {
    return 1;
  }

  std::fstream file_a, file_b;
  std::fstream file_result;
  bool skip_one_file = false;
  std::fstream *file_to_copy = nullptr;

  if( !open_file( &file_a, file_path_a, std::ios::in ) ||
      !open_file( &file_b, file_path_b, std::ios::in ) ||
      !open_file( &file_result, file_path_result, std::ios::out ) ) {
    return 2;
  }

  std::string line_a, line_b;
  while( !file_a.eof() && !file_b.eof() ) {
    if( std::getline( file_a, line_a ).fail() ) {
      std::cout << "Failed to get the line from file \"" << file_path_a << "\"" << std::endl;
      return 3;
    }

    if( std::getline( file_b, line_b ).fail() ) {
      std::cout << "Failed to get the line from file \"" << file_path_b << "\"" << std::endl;
      return 3;
    }

    line_a += " " + line_b + "\n";

    if( !(file_result << line_a) ) {
      std::cout << "Failed to write to the result file \"" << file_path_result << "\"." << std::endl;
      return 4;
    }
    if( file_a.eof() && !file_b.eof() ) {
      skip_one_file = true;
      file_to_copy = &file_b;
    } else if( file_b.eof() && !file_a.eof() ) {
      skip_one_file = true;
      file_to_copy = &file_a;
    }
  }

  std::string line;
  if( skip_one_file ) {
    while( !file_to_copy->eof() ) {
      if( std::getline( *file_to_copy, line ).fail() ) {
        std::cout << "Failed to get the line from the longer file." << std::endl;
        return 5;
      }
      line += "\n";

      if( !(file_result << line) ) {
        std::cout << "Failed to write to the result file \"" << file_path_result << "\"." << std::endl;
        return 4;
      }
    }
  }

  std::cout << "Program executed nominally." << std::endl;
  return 0;
}

void print_usage() {
  char const* usage_str =
  R"(
----- Task 2 -----
---> About
 This program concatenates the corresponding lines of the
 input files to the output file.
---> Usage example
 program file_to_copy_A file_to_copy_B result_file
)";

  std::cout << usage_str << std::endl;
}

bool does_file_exist( std::string_view filename ) {
  if( !std::filesystem::exists( filename ) ) {
    std::cout << "File \"" << filename << "\" not found." << std::endl;
    return false;
  }
  return true;
}

bool open_file( std::fstream *file, std::string_view path, std::ios_base::openmode flags ) {
  file->open( path, flags );

  if( file->fail() ) {
    std::cout << "Can't open file \"" << path << "\"." << std::endl;
    return false;
  }
  return true;
}
