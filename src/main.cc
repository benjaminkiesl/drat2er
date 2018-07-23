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

using std::string;
using std::shared_ptr;
using std::system;
using std::cout;
using std::endl;

using namespace drat2er;

const string kDratTrimPath = "/media/DATA/code/drat-trim/drat-trim";
//const string kInputFormula = "/media/DATA/Dropbox/papers/bc_rat/code/cnf/hole20.cnf";
const string kInputFormula = "/home/benjamin/Documents/drat2er/test.cnf";
const string kInputDratProof = "/media/DATA/Dropbox/papers/bc_rat/code/Cook/hole20.rat";
//const string kLratProof = "/home/benjamin/Documents/drat2er/temp.lrat";
const string kLratProof = "/home/benjamin/Documents/drat2er/test.lrat";
const string kOutputErProof = "/home/benjamin/Documents/drat2er/test.er";

int main()
{
  FormulaParser parser {};
  std::shared_ptr<Formula> formula = parser.ParseFormula(kInputFormula);

  auto drat_trim_call = kDratTrimPath + " " + kInputFormula + " " +
    kInputDratProof + " -L " + kLratProof;
  //system(drat_trim_call.c_str()); 

  LratParser lrat_parser{};

  auto stat_collector = std::make_shared<ProofStatCollector>(formula);
  lrat_parser.RegisterObserver(stat_collector);
  lrat_parser.ParseFile(kLratProof);

  auto rat_eliminator = std::make_shared<RatEliminator>(kOutputErProof, formula,
      stat_collector->GetMaxVariable(), stat_collector->GetMaxInstruction());
  lrat_parser.RegisterObserver(rat_eliminator);
  lrat_parser.ParseFile(kLratProof);

  cout << "DRAT2ER finished succcessfully." << endl;
  return 0;
}

