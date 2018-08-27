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

}

} // namespace drat2er
