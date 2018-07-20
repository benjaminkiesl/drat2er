#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include "formula.h"
#include "clause.h"
#include "formula_parser.h"
#include "rat_eliminator.h"

using std::string;
using std::unique_ptr;
using std::system;
using std::cout;
using std::endl;

using namespace drat2er;

const string kDratTrimPath = "/media/DATA/code/drat-trim/drat-trim";
const string kInputFormula = "/home/benjamin/Documents/drat2er/hole20.cnf";
const string kInputDratProof = "/home/benjamin/Documents/drat2er/hole20.rat";
const string kLratProof = "/home/benjamin/Documents/drat2er/hole20.lrat";
const string kOutputErProof = "/home/benjamin/Documents/drat2er/hole20.er";

int main()
{
  FormulaParser parser {};
  auto formula = parser.ParseFormula(kInputFormula);
  auto drat_trim_call = kDratTrimPath + " " + kInputFormula + " " +
    kInputDratProof + " -L " + kLratProof;
  //system(drat_trim_call.c_str()); 
  RatEliminator eliminator{};
  eliminator.Apply(*formula, kLratProof, kOutputErProof);
  cout << "DRAT2ER finished succcessfully." << endl;
  return 0;
}

