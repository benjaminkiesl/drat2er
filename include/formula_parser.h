#ifndef DRAT2ER_FORMULA_PARSER_H_
#define DRAT2ER_FORMULA_PARSER_H_

#include <string>
#include <utility>
#include <memory>
//#include "formula.h"

namespace drat2er
{

class Formula;
class Clause;

struct FormulaProperties {
  FormulaProperties() : number_of_variables{0}, number_of_clauses{0} {}
  int number_of_variables;
  int number_of_clauses;
};

class FormulaParser
{
 public:
  FormulaParser() {};
  std::unique_ptr<Formula> ParseFormula(const std::string& file_name);
  std::unique_ptr<Clause> ParseClause(const std::string& clause_line);

 private:
  FormulaParser(const FormulaParser& other);
  FormulaParser& operator=(FormulaParser other);

  void ParseComment(const std::string& comment_line);
  FormulaProperties ParseHeader(const std::string& header_line);
};

} //namespace

#endif
