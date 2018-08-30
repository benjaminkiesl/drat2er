// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.

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

int get_number_of_lines(const string& file_path)
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
