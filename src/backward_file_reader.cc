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

#include "backward_file_reader.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

using std::string;

namespace drat2er
{

BackwardFileReader::BackwardFileReader(string file_path)
  : input_file_stream_(file_path, std::ios::ate),
    file_position_(input_file_stream_.tellg())
{ 
  file_position_ -= 2; // get rid of the last new line
}

bool BackwardFileReader::GetLine(string& line)
{
  if(file_position_ < 0){
    return false;
  }

  line = "";
  char next_character = 0;

  while(file_position_ >= 0 && next_character != '\n'){
    input_file_stream_.seekg(file_position_--);
    input_file_stream_.get(next_character);
    line.push_back(next_character);
  }
  
  if(next_character == '\n'){
    line.pop_back();
  }
  
  std::reverse(line.begin(), line.end());

  return true;
}

} // namespace drat2er
