#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include "formula.h"
#include "clause.h"
#include "formula_parser.h"
#include "rat_eliminator.h"
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

int main (int argc, char *argv[])
{
  //string file_name = "hole20";
  string file_name = "Urquhart-s5-b1.shuffled";
  if(argc > 1){
    file_name = argv[1];
  }
  const string kInputFormula = input_folder_name + "cnf/" + file_name + ".cnf";
  const string kInputDRAT = input_folder_name + "drat/" + file_name + ".drat";
  const string kOutputLRAT = output_folder_name + file_name + ".lrat";
  const string kOutputEDRUP = output_folder_name + file_name + ".edrup";
  const string kOutputERUP = output_folder_name + file_name + ".erup";

  //cout << "drat2er: Parsing Formula..." << endl;
  //FormulaParser parser {};
  //std::shared_ptr<Formula> formula = parser.ParseFormula(kInputFormula);
  //if(formula == nullptr){
  //  std::cerr << "File '" << kInputFormula << "' could not be opened." << endl;
  //  return 1;
  //}
  //
  //cout << "drat2er: Converting DRAT proof to LRAT format using drat-trim..." << endl;
  //auto drat_trim_call = kDRATTrimPath + " " + kInputFormula + " " +
  //  kInputDRAT + " -b -L " + kOutputLRAT;
  //system(drat_trim_call.c_str()); 
  //
  //LratParser lrat_parser{};
  //
  //auto simple_parser = std::make_shared<SimpleParser>(formula);
  //lrat_parser.RegisterObserver(simple_parser);
  //lrat_parser.ParseFile(kOutputLRAT);

  cout << "drat2er: Parsing Formula..." << endl;
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(kInputFormula);
  if(formula == nullptr){
    std::cerr << "File '" << kInputFormula << "' could not be opened." << endl;
    return 1;
  }
  
  cout << "drat2er: Converting DRAT proof to LRAT format using drat-trim..." << endl;
  auto drat_trim_call = kDRATTrimPath + " " + kInputFormula + " " +
    kInputDRAT + " -b -L " + kOutputLRAT;
  system(drat_trim_call.c_str()); 
  
  LratParser lrat_parser{};
  
  auto stat_collector = std::make_shared<ProofStatCollector>(formula);
  lrat_parser.RegisterObserver(stat_collector);
  lrat_parser.ParseFile(kOutputLRAT);
  
  cout << "drat2er: Eliminating proper RATs..." << endl;
  auto rat_eliminator = 
    std::make_shared<RatEliminator>(kOutputEDRUP, formula, 
        stat_collector->GetMaxVariable(), stat_collector->GetMaxInstruction(),
        stat_collector->GetNumberOfProperRatAdditions());
  lrat_parser.RegisterObserver(rat_eliminator);
  lrat_parser.ParseFile(kOutputLRAT);
  
  cout << "drat2er: Eliminating deletions..." << endl;
  auto sed_call = "sed '/^d.*/d' " + kOutputEDRUP + " > " + kOutputERUP;
  system(sed_call.c_str());
  
  cout << "drat2er: Verifying proof with drat-trim..." << endl;
  auto drat_trim_check_call = kDRATTrimPath + " " + kInputFormula 
    + " " + kOutputERUP + " -b";
  system(drat_trim_check_call.c_str());
  
  cout << "drat2er: Finished." << endl;
  return 0;
}

