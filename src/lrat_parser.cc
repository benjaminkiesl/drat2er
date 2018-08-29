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

#include "lrat_parser.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

using std::string;
using std::stringstream;
using std::vector;
using std::shared_ptr;
using std::ifstream;
using std::cout;
using std::endl;

namespace drat2er
{

void LratParser::ParseFile(const string& proof_file_path){
  if(observer_ == nullptr){
    return;
  }
  ifstream input_stream {proof_file_path, ifstream::in};
  string proof_line;
  while(getline(input_stream, proof_line)) {
    if(IsDeletion(proof_line)){
      observer_->ObserveDeletion(ParseDeletion(proof_line));
    } else if(IsExtension(proof_line)){
      observer_->ObserveExtension(ParseExtension(proof_line));
    } else if(IsProperRatAddition(proof_line)){
      observer_->ObserveProperRatAddition(ParseProperRat(proof_line));
    } else if(IsComment(proof_line)){
      observer_->ObserveComment(proof_line);
    } else {
      observer_->ObserveRupAddition(ParseRup(proof_line));
    }
  }
}

void LratParser::RegisterObserver(LratParserObserver* observer){
  observer_ = observer;
}

bool LratParser::IsProperRatAddition(const string& proof_line)
{
  if(proof_line.front() == '0' || ContainsNoLiterals(proof_line)){
    return false;
  }

  auto index_of_0 = proof_line.find(" 0 ");
  if(index_of_0 != string::npos && index_of_0 + 2 < proof_line.size()) {
    auto index_of_first_nonspace_after_0 =
        proof_line.find_first_not_of(" ", index_of_0 + 2);
    return index_of_first_nonspace_after_0 != string::npos &&
          (proof_line[index_of_first_nonspace_after_0] == '0' ||
           proof_line.find("-", index_of_0 + 2) != string::npos);
  }
  return false;
}

bool LratParser::IsDeletion(const string& proof_line)
{
  return proof_line.find('d') != string::npos;
}

bool LratParser::IsExtension(const string& proof_line)
{
  return proof_line.find('e') != string::npos;
}

bool LratParser::IsComment(const string& proof_line)
{
  return proof_line.front() == 'c';
}

bool LratParser::ContainsNoLiterals(const string& proof_line){
  int index_of_first_space = proof_line.find(' ');
  if(index_of_first_space != string::npos){
    int index_of_first_number_after_0 = 
      proof_line.find_first_not_of(' ', index_of_first_space + 1);
    if(index_of_first_number_after_0 != string::npos){
      return proof_line[index_of_first_number_after_0] == '0';
    }
  }
  return false;
}

Deletion LratParser::ParseDeletion(const string& proof_line)
{
  assert(IsDeletion(proof_line));
  Deletion deletion;
  stringstream line_stream {proof_line};
  int instruction_index = 0;
  line_stream >> instruction_index;
  deletion.SetIndex(instruction_index);

  char d_symbol;
  line_stream >> d_symbol;

  int token;
  line_stream >> token;
  while(token != 0) {
    deletion.AddClauseIndex(token);
    line_stream >> token;
  }
  return deletion;
}

RupClause LratParser::ParseRup(const string& proof_line)
{
  stringstream line_stream {proof_line};
  RupClause rup; 
  ParseClausePart(rup, line_stream);
  int hint = 0;
  line_stream >> hint;
  while(hint != 0){
    rup.AddPositiveHint(hint);
    line_stream >> hint;
  }
  return rup;
}

RatClause LratParser::ParseProperRat(const string& proof_line)
{
  assert(IsProperRatAddition(proof_line));
  stringstream line_stream {proof_line};
  RatClause rat{};
  ParseClausePart(rat, line_stream);

  // Parse positive hints partners
  int token = 0;
  line_stream >> token;
  while(token > 0){
    rat.AddPositiveHint(token);
    line_stream >> token;
  }

  // Parse negative hints
  while(token != 0) {
    int resolution_partner = -token;
    vector<int> hints{};
    line_stream >> token;
    while(token > 0) {
      hints.emplace_back(token);
      line_stream >> token;
    }
    rat.AddNegativeHint(resolution_partner, hints);
  }

  return rat;
}

string LratParser::RemoveE(const string& proof_line){
  assert(IsExtension(proof_line));
  int index_of_first_space = proof_line.find(" ");
  int index_of_e = proof_line.find("e"); 
  int index_of_first_literal = proof_line.find_first_not_of(" ", index_of_e+1); 
  return proof_line.substr(0, index_of_first_space + 1) + 
    proof_line.substr(index_of_first_literal);
}

Clause LratParser::ParseExtension(const string& proof_line){
  assert(IsExtension(proof_line));
  Clause clause;
  stringstream line_stream{RemoveE(proof_line)};
  ParseClausePart(clause, line_stream);
  return clause;
}

void LratParser::ParseClausePart(Clause& clause, stringstream& line_stream)
{
  int token;
  line_stream >> token;
  clause.SetIndex(token);
  line_stream >> token;
  while(token != 0) {
    clause.AddLiteral(token);
    line_stream >> token;
  }
}

} // namespace drat2er
