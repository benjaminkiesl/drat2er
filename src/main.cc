#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include "CLI11.hpp"
#include "formula.h"
#include "clause.h"
#include "formula_parser.h"
#include "rat_eliminator.h"
#include "rup_to_resolution_transformer.h"
#include "proof_step_renamer.h"
#include "lrat_parser.h"
#include "proof_stat_collector.h"
#include "deletion_eliminator.h"
#include "simple_parser.h"

using std::string;
using std::shared_ptr;
using std::system;
using std::cout;
using std::endl;

using namespace drat2er;

const string input_folder_name = "/media/DATA/Dropbox/papers/bc_rat/code/";
const string output_folder_name = "/home/benjamin/Documents/drat2er/temp/";

const string kDRATTrimPath = "/media/DATA/code/drat2er/build/bin/drat-trim";

void eliminate_deletions(const std::string& input_file,
                         std::ostream& output_stream){
  std::ifstream input_stream {input_file, std::ifstream::in};
  string proof_line;
  while(getline(input_stream, proof_line)) {
    if(proof_line.find(" d ") == string::npos){
      output_stream << proof_line << endl;
    }
  }
}

void eliminate_deletions(const std::string& input_file,
                         const std::string& output_file){
  std::ofstream output_stream{output_file};
  eliminate_deletions(input_file, output_stream);
}


int PerformTransformation(const std::string& input_formula_file,
                          const std::string& input_proof_file,
                          std::ostream& output_stream){

  const string kOutputLRAT = output_folder_name + "temp.lrat";
  const string kOutputEDRUP = output_folder_name + "temp.edrup";
  const string kOutputERUP = output_folder_name + "temp.erup";
  const string kOutputER = output_folder_name + "temp_unrenamed.er";

  cout << "c drat2er: Parsing Formula..." << endl;
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(input_formula_file);
  if(formula == nullptr){
    std::cerr << "File '" << input_formula_file << "' could not be opened." 
      << endl;
    return 1;
  }
  int size_of_original_formula = formula->GetClauses().size();
  
  cout << "c drat2er: Verifying DRAT proof and converting it to" 
          " LRAT format using drat-trim..." << endl;
  auto drat_trim_call = kDRATTrimPath + " " + input_formula_file + " " +
    input_proof_file + " -b -L " + kOutputLRAT;
  system(drat_trim_call.c_str()); 
  
  LratParser lrat_parser{};
  
  ProofStatCollector stat_collector(formula);
  lrat_parser.RegisterObserver(&stat_collector);
  lrat_parser.ParseFile(kOutputLRAT);
  
  cout << "c drat2er: Eliminating proper RAT additions..." << endl;
  RatEliminator rat_eliminator(formula, stat_collector.GetMaxVariable(),
                                        stat_collector.GetMaxInstruction());
  rat_eliminator.Transform(kOutputLRAT, kOutputEDRUP);
  
  cout << "c drat2er: Eliminating deletions..." << endl;
  eliminate_deletions(kOutputEDRUP, kOutputERUP);

  cout << "c drat2er: Transforming RUPs to resolution chains..." << endl;
  std::shared_ptr<Formula> original_formula = 
    parser.ParseFormula(input_formula_file);

  RupToResolutionTransformer rup_to_resolution_transformer(original_formula);
  rup_to_resolution_transformer.Transform(kOutputERUP, kOutputER);

  cout << "c drat2er: Renaming clause indices incrementally..." << endl;
  ProofStepRenamer incremental_proof_step_renamer(size_of_original_formula);
  incremental_proof_step_renamer.Transform(kOutputER, output_stream);
  
  cout << "c drat2er: Proof successfully transformed." << endl;
  return 0;
}

int main (int argc, char *argv[])
{
  const string kInputFormula = input_folder_name + "cnf/hole20.cnf";
  const string kInputDRAT = input_folder_name + "drat/hole20.drat";
  const string kOutputERRenamed = output_folder_name + "hole20.er";

  int return_code = PerformTransformation(kInputFormula, 
                                          kInputDRAT, 
                                          cout);

  return return_code;
}

