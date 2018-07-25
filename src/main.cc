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

using std::string;
using std::shared_ptr;
using std::system;
using std::cout;
using std::endl;

using namespace drat2er;

const string folder_name = "/home/benjamin/Documents/drat2er/";
const string temp_folder = "temp/";

const string kDRATTrimPath = "/media/DATA/code/drat-trim/drat-trim";
//const string kOutputLEDRUP = folder_name + file_name + ".ledrup";
//const string kOutputLERUP = folder_name + file_name + ".lerup";
//const string kOutputER = folder_name + file_name + ".er";

int main (int argc, char *argv[])
{
  string file_name = "hole20";
  if(argc > 1){
    file_name = argv[1];
  }
  const string kInputFormula = folder_name + file_name + ".cnf";
  const string kInputDRAT = folder_name + file_name + ".drat";
  const string kOutputLRAT = folder_name + temp_folder + file_name + ".lrat";
  const string kOutputEDRUP = folder_name + temp_folder + file_name + ".edrup";
  const string kOutputERUP = folder_name + temp_folder + file_name + ".erup";

  cout << "DRAT2ER: Parsing Formula..." << endl;
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(kInputFormula);
  if(formula == nullptr){
    std::cerr << "File '" << kInputFormula << "' could not be opened." << endl;
    return 1;
  }

  cout << "DRAT2ER: Converting DRAT proof to LRAT format using drat-trim..." << endl;
  auto drat_trim_call = kDRATTrimPath + " " + kInputFormula + " " +
    kInputDRAT + " -b -L " + kOutputLRAT;
  system(drat_trim_call.c_str()); 

  LratParser lrat_parser{};

  auto stat_collector = std::make_shared<ProofStatCollector>(formula);
  lrat_parser.RegisterObserver(stat_collector);
  lrat_parser.ParseFile(kOutputLRAT);

  cout << "DRAT2ER: Eliminating proper RATs..." << endl;
  auto rat_eliminator = 
    std::make_shared<RatEliminator>(kOutputEDRUP, formula, 
        stat_collector->GetMaxVariable(), stat_collector->GetMaxInstruction());
  lrat_parser.RegisterObserver(rat_eliminator);
  lrat_parser.ParseFile(kOutputLRAT);

  cout << "DRAT2ER: Eliminating deletions..." << endl;
  //auto deletion_eliminator = 
  //  std::make_shared<DeletionEliminator>(kOutputERUP);
  //lrat_parser.RegisterObserver(deletion_eliminator);
  //lrat_parser.ParseFile(kOutputEDRUP);
  auto sed_call = "sed '/^d.*/d' " + kOutputEDRUP + " > " + kOutputERUP;
  system(sed_call.c_str());
  
  cout << "DRAT2ER: Verifying proof with drat-trim..." << endl;
  auto drat_trim_check_call = kDRATTrimPath + " " + kInputFormula 
    + " -b " + kOutputERUP;
  system(drat_trim_check_call.c_str());

  cout << "DRAT2ER: Finished." << endl;
  return 0;
}

