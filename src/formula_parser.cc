#include "formula_parser.h"

#include <fstream>
#include <utility>
#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>
#include <cassert>
#include "formula.h"
#include "clause.h"

using std::string;
using std::stoi;
using std::stringstream;
using std::getline;
using std::ifstream;
using std::unique_ptr;
using std::make_unique;
using std::cout;
using std::cerr;
using std::endl;

namespace drat2er
{

unique_ptr<Formula> FormulaParser::ParseFormula(
    const string& file_name)
{
  ifstream file_stream {file_name, ifstream::in};

  if(file_stream.fail()) {
    return nullptr;
  }

  string line;

  while(getline(file_stream, line) && line.front() != 'p') {
    if(line.front() == 'c') {
      ParseComment(line);
    }
  }

  FormulaProperties formula_properties;
  if(line.front() == 'p') {
    formula_properties = ParseHeader(line);
  }

  auto formula = make_unique<Formula>(formula_properties.number_of_variables,
                                      formula_properties.number_of_clauses);

  int clause_index = 1;
  while(getline(file_stream, line)) {
    if(line.front() == 'c') {
      ParseComment(line);
    } else {
      auto clause = ParseClause(line);
      clause.SetIndex(clause_index++);
      formula->AddClause(clause);
    }
  }

  return formula;
}

void FormulaParser::ParseComment(const string& comment_line)
{
  // Do nothing
}

FormulaProperties FormulaParser::ParseHeader(const string& header_line)
{
  assert(header_line.front() == 'p');
  stringstream header_stream {header_line};
  string token;
  header_stream >> token; // 'p'
  header_stream >> token;
  if(header_stream.fail() || token != "cnf"){
    cerr << "Parsing error: Could not parse header of formula." << endl;
  }

  FormulaProperties formula_properties;

  header_stream >> formula_properties.number_of_variables;
  if(header_stream.fail()){
    cerr << "Parsing error: Could not parse number of variables in header "
            "of formula." << endl;
  }

  header_stream >> formula_properties.number_of_clauses;
  if(header_stream.fail()){
    cerr << "Parsing error: Could not parse number of clauses in header "
            "of formula." << endl;
  }
  //for(int i=1; getline(header_stream, token, ' '); i++) {
  //  if(i==3) {
  //    formula_properties.number_of_variables = stoi(token);
  //  } else if(i==4) {
  //    formula_properties.number_of_clauses = stoi(token);
  //  }
  //}
  return formula_properties;
}

Clause FormulaParser::ParseClause(const string& clause_line)
{
  Clause clause;
  stringstream line_stream {clause_line};
  int literal = 0;
  line_stream >> literal;
  while(!line_stream.fail() && literal != 0) {
    clause.AddLiteral(literal);
    line_stream >> literal;
  }

  if(line_stream.fail()){
    cerr << "Parsing error: Could not parse clause in formula." << endl;
    // TODO: maybe throw exception;
  }

  return clause;
}

} // namespace
