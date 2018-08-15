#include "file_helper.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#ifdef WINDOWS
#include <direct.h>
#define get_current_dir _getcwd
#else
#include <unistd.h>
#define get_current_dir getcwd
#endif

using std::string;
using std::ifstream;

namespace drat2er
{

namespace file_helper
{

string get_current_working_directory()
{
  char c_current_path[FILENAME_MAX];

  if (!get_current_dir(c_current_path, sizeof(c_current_path))) {
    return "";
  }
  
  return c_current_path;
}

int get_number_of_lines(string file_path)
{
  ifstream input_file(file_path);
  if(input_file.is_open()) {
    return std::count(std::istreambuf_iterator<char>(input_file),
                      std::istreambuf_iterator<char>(), '\n');
  }
  return -1;
}

} // namespace file_helper

} // namespace drat2er
