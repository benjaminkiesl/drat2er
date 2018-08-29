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

#include "drat_trim_interface.h"
#include <string>
#include <cstring>
#include <vector>

extern "C" {
  int run_drat_trim (int argc, char** argv);
}

using std::string;
using std::vector;

namespace drat2er {

namespace drat_trim {
  
int check_and_convert_to_lrat(string input_formula_path, 
                              string input_proof_path, 
                              string output_proof_path, bool is_verbose){
  vector<string> args;
  args.push_back("");
  args.push_back(input_formula_path);
  args.push_back(input_proof_path);
  if(is_verbose){
    args.push_back("-b");
  }
  args.push_back("-L");
  args.push_back(output_proof_path);

  vector<char*> args_c_strings;
  for(auto& argument : args){
    args_c_strings.push_back(&argument.front());
  }

  return run_drat_trim(args_c_strings.size(), args_c_strings.data());
}

} // namespace drat_trim

} // namespace drat2er
