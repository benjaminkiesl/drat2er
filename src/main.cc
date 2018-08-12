#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
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

const string kDRATTrimPath = "/media/DATA/code/drat-trim/drat-trim";

//void process_proof(std::shared_ptr<LratParser> proof_parser,
//                   std::shared_ptr<LratParserObserver> proof_transformer,
//                   const std::string& input_file_path,
//                   const std::string& output_file_path){
//}

void eliminate_deletions(const std::string& input_file,
                         const std::string& output_file){

  std::ifstream input_stream {input_file, std::ifstream::in};
  std::ofstream output_stream {output_file};
  string proof_line;
  while(getline(input_stream, proof_line)) {
    if(proof_line.find(" d ") == string::npos){
      output_stream << proof_line << endl;
    }
  }
}

int main (int argc, char *argv[])
{
  string file_name = "hole20";
  //string file_name = "Urquhart-s5-b1.shuffled";
  if(argc > 1){
    file_name = argv[1];
  }

  const string kInputFormula = input_folder_name + "cnf/" + file_name + ".cnf";
  const string kInputDRAT = input_folder_name + "drat/" + file_name + ".drat";
  const string kOutputLRAT = output_folder_name + file_name + ".lrat";
  const string kOutputEDRUP = output_folder_name + file_name + ".edrup";
  const string kOutputERUP = output_folder_name + file_name + ".erup";
  const string kOutputER = output_folder_name + file_name + "_unrenamed.er";
  const string kOutputERRenamed = output_folder_name + file_name + ".er";
  const string kOutputERTrimmed = output_folder_name + file_name + "_trimmed.er";

  cout << "drat2er: Parsing Formula..." << endl;
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(kInputFormula);
  if(formula == nullptr){
    std::cerr << "File '" << kInputFormula << "' could not be opened." << endl;
    return 1;
  }
  int size_of_original_formula = formula->GetClauses().size();
  
  cout << "drat2er: Verifying DRAT proof and converting it to" 
          " LRAT format using drat-trim..." << endl;
  auto drat_trim_call = kDRATTrimPath + " " + kInputFormula + " " +
    kInputDRAT + " -b -L " + kOutputLRAT;
  system(drat_trim_call.c_str()); 
  
  LratParser lrat_parser{};
  
  ProofStatCollector stat_collector(formula);
  lrat_parser.RegisterObserver(&stat_collector);
  lrat_parser.ParseFile(kOutputLRAT);
  
  cout << "drat2er: Eliminating proper RAT additions..." << endl;
  RatEliminator rat_eliminator(kOutputEDRUP, formula, 
        stat_collector.GetMaxVariable(), stat_collector.GetMaxInstruction(),
        stat_collector.GetNumberOfProperRatAdditions());
  lrat_parser.RegisterObserver(&rat_eliminator);
  lrat_parser.ParseFile(kOutputLRAT);
  
  cout << "drat2er: Eliminating deletions..." << endl;
  eliminate_deletions(kOutputEDRUP, kOutputERUP);

  cout << "drat2er: Transforming RUPs to resolution chains..." << endl;
  std::shared_ptr<Formula> original_formula = 
    parser.ParseFormula(kInputFormula);

  RupToResolutionTransformer rup_to_resolution_transformer(original_formula);
  rup_to_resolution_transformer.Transform(kOutputERUP, kOutputER);

  cout << "drat2er: Renaming clause indices incrementally..." << endl;
  ProofStepRenamer incremental_proof_step_renamer(size_of_original_formula);
  incremental_proof_step_renamer.Transform(kOutputER, kOutputERRenamed);
  
  cout << "drat2er: Proof successfully transformed." << endl;

  //cout << "drat2er: Verifying final proof with drat-trim..." << endl;
  //auto drat_trim_verification_call = kDRATTrimPath + " " + 
  //  kInputFormula + " -b " + kOutputER + " " + kOutputERTrimmed;
  //system(drat_trim_verification_call.c_str());

  return 0;
}

