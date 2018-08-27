#ifndef DRAT2ER_FORMULA_PARSER_H_
#define DRAT2ER_FORMULA_PARSER_H_

#include <string>
#include <utility>
#include <memory>

namespace drat2er
{

class Formula;
class Clause;

// A pair of two numbers: The number of variables, and the number of clauses
// of a given formula. This object is created when the header of the DIMCAS
// file is parsed.
struct FormulaProperties {
  FormulaProperties() : number_of_variables{0}, number_of_clauses{0} {}
  int number_of_variables;
  int number_of_clauses;
};

// Can parse a DIMACS file and return a Formula object created from the
// formula encoded in the DIMACS file.
class FormulaParser
{
 public:
  FormulaParser() {};

  // Parses the DIMACS file with the given file name and returns a pointer
  // to the resulting Formula object.
  std::unique_ptr<Formula> ParseFormula(const std::string& file_name);

  // Parses a clause line in a DIMACS file and returns a Clause object
  // corresponding to that line.
  Clause ParseClause(const std::string& clause_line);

 private:
  FormulaParser(const FormulaParser& other);
  FormulaParser& operator=(FormulaParser other);

  void ParseComment(const std::string& comment_line);
  FormulaProperties ParseHeader(const std::string& header_line);
};

} //namespace

#endif
