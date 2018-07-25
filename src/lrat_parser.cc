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

using std::string;
using std::stringstream;
using std::vector;
using std::shared_ptr;
using std::ifstream;
using std::cout;
using std::endl;

namespace drat2er
{

struct Deletion{
  vector<int> clause_indices;
  int instruction_index;
};

void LratParser::ParseFile(const string& proof_file_path){
  ifstream input_stream {proof_file_path, ifstream::in};
  string proof_line;
  while(getline(input_stream, proof_line)) {
    if(LratParser::IsDeletion(proof_line)){
      auto deletion = ParseDeletion(proof_line);
      observer_->HandleDeletion(deletion.clause_indices,
                                deletion.instruction_index);
    } else if(IsProperRatAddition(proof_line)){
      observer_->HandleProperRatAddition(ParseProperRat(proof_line));
    } else if(IsComment(proof_line)){
      observer_->HandleComment(proof_line);
    } else {
      observer_->HandleRupAddition(ParseRup(proof_line));
    }
  }
}

void LratParser::RegisterObserver(shared_ptr<LratParserObserver> observer){
  observer_ = observer;
}

bool LratParser::IsProperRatAddition(const string& proof_line)
{
  if(proof_line.front() == '0'){
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

bool LratParser::IsComment(const string& proof_line)
{
  return proof_line.front() == 'c';
}

Deletion LratParser::ParseDeletion(const string& proof_line)
{
  assert(IsDeletion(proof_line));
  Deletion deletion;
  stringstream line_stream {proof_line};
  line_stream >> deletion.instruction_index;

  char d_symbol;
  line_stream >> d_symbol;

  int token;
  line_stream >> token;
  while(token != 0) {
    deletion.clause_indices.emplace_back(token);
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

  // Parse resolution partners
  int token = 0;
  line_stream >> token;
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

} // namespace
