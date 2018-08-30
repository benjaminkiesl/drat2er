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

#include <iostream>
#include <fstream>
#include <ostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "CLI11.hpp"
#include "formula.h"
#include "formula_parser.h"
#include "rat_eliminator.h"
#include "rup_to_resolution_transformer.h"
#include "proof_step_renamer.h"
#include "lrat_parser.h"
#include "proof_stat_collector.h"
#include "drat_trim_interface.h"

using std::string;
using std::shared_ptr;
using std::cout;
using std::cerr;
using std::endl;

using namespace drat2er;

// File names for temp files used during the transformation.
const string kTempFileLRAT = "temp.lrat";
const string kTempFileERUP = "temp.erup";
const string kTempFileER = "temp_unrenamed.er";

// Takes as input the path to a DIMACS file, parses the file and creates
// a formula object. Returns a pointer to the formula object.
auto ParseFormula(const string& formula_file, bool is_verbose)
{
  cout << "c drat2er: Parsing input formula..." << endl;
  FormulaParser parser {};
  auto formula = parser.ParseFormula(formula_file);
  if(formula == nullptr) {
    throw std::runtime_error(
        "Error: File '" + formula_file + "' could not be opened.");
  }
  return formula;
}

// Takes a DRAT proof and calls drat-trim to turn the DRAT proof into an
// LRAT proof. The resulting proof is written to the file 'output_proof_file'
void TransformDRATToLRAT(const string& input_formula_file,
                         const string& input_proof_file,
                         const string& output_proof_file,
                         bool is_verbose)
{
  cout << "c drat2er: Verifying DRAT proof and converting it to"
       " LRAT format using drat-trim." << endl;
  if(drat_trim::check_and_convert_to_lrat(input_formula_file, input_proof_file,
                                          output_proof_file, is_verbose)) {
    throw std::runtime_error(
        "Could not parse the input proof with drat-trim.");
  }
}

// Takes as input a formula and an LRAT proof and replaces all proper RAT
// additions in the proof with extensions and RUP additions. In the course of
// this, deletions are also eliminated from the LRAT proof.
void EliminateProperRATs(shared_ptr<Formula> formula,
                         const string& input_proof_file,
                         const string& output_proof_file, bool is_verbose)
{
  LratParser lrat_parser {};
  ProofStatCollector stat_collector(formula);
  lrat_parser.RegisterObserver(&stat_collector);
  lrat_parser.ParseFile(input_proof_file);

  RatEliminator rat_eliminator(formula, stat_collector.GetMaxVariable(),
                               stat_collector.GetMaxInstruction(), is_verbose);
  rat_eliminator.Transform(input_proof_file, output_proof_file);
  std::remove(input_proof_file.c_str());
}

// Takes as input an ERUP proof (i.e., a proof containing only extensions
// and RUP additions) and transforms all RUP additions into resolution chains.
void TransformRUPsToResolutions(const string& input_formula_file,
                                const string& input_proof_file,
                                const string& output_proof_file,
                                bool is_output_drat, bool is_verbose)
{
  shared_ptr<Formula> original_formula =
      ParseFormula(input_formula_file, is_verbose);
  RupToResolutionTransformer
  rup_to_resolution_transformer(original_formula,
                                is_output_drat, is_verbose);
  rup_to_resolution_transformer.Transform(input_proof_file, output_proof_file);
  std::remove(input_proof_file.c_str());
}

// Takes as input a TRACECHECK proof (input_proof_file) whose proof steps are
// not necessarily numbered incrementally and renames the proof steps such
// that the steps in the resulting proof (output_proof_file) are numbered
// incrementally.
void RenameProofStepsIncrementally(const string& input_proof_file,
                                   const string& output_proof_file,
                                   int size_of_original_formula,
                                   bool is_verbose)
{
  auto write_to_standard_output = output_proof_file == "";
  ProofStepRenamer proof_step_renamer(size_of_original_formula,
                                      is_verbose && !write_to_standard_output);
  if(write_to_standard_output) {
    proof_step_renamer.Transform(input_proof_file, cout);
  } else {
    std::ofstream output_file_stream(output_proof_file);
    proof_step_renamer.Transform(input_proof_file, output_file_stream);
  }
  std::remove(input_proof_file.c_str());
}

// Takes as input a DIMACS file (input_formula_file) and a DRAT proof file
// (input_proof_file) and transforms the DRAT proof into an extended-resolution
// proof. The resulting proof is written to the path 'output_file'.
// The transformation is described in the paper "Extended Resolution Simulates
// DRAT" (IJCAR 2018).
void TransformDRATToExtendedResolution(const string& input_formula_file,
                                       const string& input_proof_file,
                                       const string& output_file,
                                       bool is_output_drat, bool is_verbose)
{
  TransformDRATToLRAT(input_formula_file, input_proof_file,
                      kTempFileLRAT, is_verbose);

  shared_ptr<Formula> formula = ParseFormula(input_formula_file, is_verbose);
  auto size_of_original_formula = formula->GetNumberOfClauses();

  EliminateProperRATs(formula, kTempFileLRAT, kTempFileERUP, is_verbose);

  TransformRUPsToResolutions(input_formula_file, kTempFileERUP, kTempFileER,
                             is_output_drat, is_verbose);

  if(!is_output_drat) {
    RenameProofStepsIncrementally(kTempFileER, output_file,
                                  size_of_original_formula, is_verbose);
  }
}

int main (int argc, char *argv[])
{
  CLI::App app {
    "drat2er transforms DRAT proofs into extended-resolution proofs.\n"
    "It takes as input a propositional formula (specified in the DIMCAS\n"
    "format) together with a DRAT proof, and outputs an extended-resolution\n"
    "proof of the formula in either the TRACECHECK format or the DRAT\n"
    "format. The description of this transformation can be found in the\n"
    "paper \"Extended Resolution Simulates DRAT\" (IJCAR 2018). If no\n"
    "output file is specified, the output is written to the standard output.\n"
  };

  string input_formula_path = "";
  app.add_option("input_formula", input_formula_path,
                 "Path to a DIMCAS file of the input formula."
                )->required()->check(CLI::ExistingFile);

  string input_proof_path = "";
  app.add_option("input_proof", input_proof_path,
                 "Path to a DRAT file of the input proof."
                )->required()->check(CLI::ExistingFile);

  string output_file_path = "";
  app.add_option("output_proof", output_file_path,
                 "Path for the output proof.", false)->type_name("FILE");

  bool is_verbose = false;
  app.add_flag("-v,--verbose", is_verbose,
               "Print information about the progress.");

  string output_format = "tracecheck";
  app.add_set("-f,--format", output_format, {"drat", "tracecheck"},
              "Format of the output proof (default: tracecheck).");
  bool is_output_drat = output_format == "drat";

  CLI11_PARSE(app, argc, argv);

  try {
    TransformDRATToExtendedResolution(input_formula_path, input_proof_path,
                                      output_file_path, is_output_drat, is_verbose);
    cout << "c drat2er: Proof successfully transformed." << endl;
    return 0;
  } catch(const std::exception& ex) {
    cerr << "Error: " << ex.what() << endl;
    return 1;
  }
}
