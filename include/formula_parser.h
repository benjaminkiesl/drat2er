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

#ifndef DRAT2ER_FORMULA_PARSER_H_
#define DRAT2ER_FORMULA_PARSER_H_

#include <string>
#include <utility>
#include <memory>

namespace drat2er
{

class Formula;
class Clause;


// Can parse a DIMACS file and return a Formula object created from the
// formula encoded in the DIMACS file.
class FormulaParser
{
 public:

  // Parses the DIMACS file with the given file name and returns a pointer
  // to the resulting Formula object.
  Formula ParseFormula(const std::string& file_name) const;

  // Parses a clause line in a DIMACS file and returns a Clause object
  // corresponding to that line.
  Clause ParseClause(const std::string& clause_line) const;

 private:

  // A pair of two numbers: The number of variables, and the number of clauses
  // of a given formula. This object is created when the header of the DIMCAS
  // file is parsed.
  struct FormulaProperties {
    FormulaProperties() : number_of_variables{0}, number_of_clauses{0} {}
    int number_of_variables;
    int number_of_clauses;
  };

  FormulaParser::FormulaProperties 
    ParseHeader(const std::string& header_line) const;

  void ParseComment(const std::string& comment_line) const;
};

} //namespace drat2er

#endif
