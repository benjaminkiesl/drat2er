#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <cstdio>
#include "CLI11.hpp"
#include "formula.h"
#include "clause.h"
#include "formula_parser.h"
#include "rat_eliminator.h"
#include "rup_to_resolution_transformer.h"
#include "proof_step_renamer.h"
#include "lrat_parser.h"
#include "proof_stat_collector.h"
#include "simple_parser.h"
#include "file_helper.h"


using std::string;
using std::shared_ptr;
using std::system;
using std::cout;
using std::cerr;
using std::endl;

using namespace drat2er;

const string input_folder_name = "/media/important/Dropbox/papers/bc_rat/code/";
const string output_folder_name = "/home/benjamin/Documents/drat2er/temp/";
const string kOutputLRAT = "temp.lrat";
const string kOutputEDRUP = "temp.edrup";
const string kOutputERUP = "temp.erup";
const string kOutputER = "temp_unrenamed.er";
//const string kDRATTrimPath = "/media/important/code/drat2er/build/bin/drat-trim";
const string kDRATTrimPath = file_helper::get_current_working_directory() 
                             + "/drat-trim";

//void eliminate_deletions(const std::string& input_file,
//                         std::ostream& output_stream,
//                         bool is_verbose){
//  if(is_verbose){
//    cout << "c drat2er: Eliminating deletions." << endl;
//  }
//  std::ifstream input_stream {input_file, std::ifstream::in};
//  string proof_line;
//  while(getline(input_stream, proof_line)) {
//    if(proof_line.find(" d ") == string::npos){
//      output_stream << proof_line << endl;
//    }
//  }
//}
//
//void eliminate_deletions(const std::string& input_file,
//                         const std::string& output_file,
//                         bool is_verbose){
//  std::ofstream output_stream{output_file};
//  eliminate_deletions(input_file, output_stream, is_verbose);
//}

int PerformTransformation(const std::string& input_formula_file,
                          const std::string& input_proof_file,
                          std::ostream& output_stream,
                          bool is_output_drat, bool is_verbose){

  cout << "c drat2er: Parsing Formula..." << endl;
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(input_formula_file);
  if(formula == nullptr){
    cerr << "Error: File '" << input_formula_file << "' could not be opened." 
      << endl;
    return 1;
  }
  int size_of_original_formula = formula->GetClauses().size();
  
  cout << "c drat2er: Verifying DRAT proof and converting it to" 
          " LRAT format using drat-trim." << endl;
  auto drat_trim_call = kDRATTrimPath + " " + input_formula_file + " " +
    input_proof_file + (is_verbose ? " -b" : " ") + "-L " + kOutputLRAT;
  int status = system(drat_trim_call.c_str()); 
  if(WEXITSTATUS(status) != 0){
    cerr << "Error: Could not parse the input proof with drat-trim." << endl;
    return 1;
  }
  
  LratParser lrat_parser{};
  
  ProofStatCollector stat_collector(formula);
  lrat_parser.RegisterObserver(&stat_collector);
  lrat_parser.ParseFile(kOutputLRAT);
  
  RatEliminator rat_eliminator(formula, stat_collector.GetMaxVariable(),
                               stat_collector.GetMaxInstruction(), is_verbose);
  rat_eliminator.Transform(kOutputLRAT, kOutputEDRUP);
  
  //eliminate_deletions(kOutputEDRUP, kOutputERUP, is_verbose);

  std::shared_ptr<Formula> original_formula = 
    parser.ParseFormula(input_formula_file);

  RupToResolutionTransformer 
    rup_to_resolution_transformer(original_formula, 
                                  is_output_drat, is_verbose);
  rup_to_resolution_transformer.Transform(kOutputERUP, kOutputER);

  ProofStepRenamer 
    incremental_proof_step_renamer(size_of_original_formula, is_verbose);
  incremental_proof_step_renamer.Transform(kOutputER, output_stream);

  std::remove(kOutputLRAT.c_str());
  std::remove(kOutputEDRUP.c_str());
  std::remove(kOutputERUP.c_str());
  std::remove(kOutputER.c_str());
  
  cout << "c drat2er: Proof successfully transformed." << endl;
  return 0;
}

int PerformTransformation(const std::string& input_formula_file,
                          const std::string& input_proof_file,
                          const std::string& output_proof_file,
                          bool is_output_drat, bool is_verbose){
  if(output_proof_file != "") {
    std::ofstream output_stream{output_proof_file};
    return PerformTransformation(input_formula_file, input_proof_file,
                                 output_stream, is_output_drat, is_verbose);
  } else {
    return PerformTransformation(input_formula_file, input_proof_file,
                                 cout, is_output_drat, is_verbose);
  }
}

int main (int argc, char *argv[])
{
  CLI::App app{
    "drat2er transforms DRAT proofs into extended-resolution proofs.\n"
    "It takes as input a propositional formula (specified in the DIMCAS\n" 
    "format) together with a DRAT proof, and outputs an extended-resolution\n" 
    "proof of the formula in either the TRACECHECK format or the DRAT\n" 
    "format. The description of this transformation can be found in the\n" 
    "paper \"Extended Resolution Simulates DRAT\" (IJCAR 2018). If no\n" 
    "output file is specified, the output is written to the standard output.\n" 
  };

  std::string input_formula_path = "";
  app.add_option("input_formula", input_formula_path, 
      "Path to a DIMCAS file of the input formula."
      )->required()->check(CLI::ExistingFile);
  
  std::string input_proof_path = "";
  app.add_option("input_proof", input_proof_path, 
      "Path to a DRAT file of the input proof."
      )->required()->check(CLI::ExistingFile);
  
  std::string output_file_path = "";
  app.add_option("output_proof", output_file_path, 
      "Path for the output proof.", false)->type_name("FILE");

  bool is_verbose = false;
  app.add_flag("-v,--verbose", is_verbose, 
      "Print information about the progress.");

  std::string output_format = "tracecheck";
  app.add_set("-f,--format", output_format, {"drat", "tracecheck"}, 
      "Format of the output proof (default: tracecheck).");
  bool is_output_drat = output_format == "drat";

  CLI11_PARSE(app, argc, argv);

  return PerformTransformation(input_formula_path, input_proof_path,
                               output_file_path, is_output_drat, is_verbose);
}
