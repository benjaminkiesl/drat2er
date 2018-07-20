#include "rat_eliminator.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include "formula.h"
#include "clause.h"
#include "rat_clause.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::vector;
using std::endl;
using std::cout;

namespace drat2er
{

unique_ptr<RatClause> ParseRat(const string& proof_line){
  stringstream line_stream {proof_line};
  int token;
  line_stream >> token;
  auto rat = make_unique<RatClause>(token);

  // Parse literals
  line_stream >> token;
  while(token != 0) {
    rat->AddLiteral(token);
    line_stream >> token;
  }

  // Parse resolution partners
  line_stream >> token;
  while(token != 0){
    ResolutionPartner resolution_partner{-token};
    line_stream >> token;
    while(token > 0){
      resolution_partner.Hints().emplace_back(token);
      line_stream >> token;
    }
    rat->AddResolutionPartner(resolution_partner);
  }

  return rat;
}

bool IsProperRatAddition(const string& proof_line){
  auto index_of_0 = proof_line.find(" 0 ");
  if(index_of_0 != string::npos && index_of_0 + 2 < proof_line.size()){ 
    auto index_of_first_nonspace_after_0 = 
      proof_line.find_first_not_of(" ", index_of_0 + 2);
    return index_of_first_nonspace_after_0 != string::npos &&
      proof_line[index_of_first_nonspace_after_0] == '-';
  }
  return false;
}


bool IsComment(const string& proof_line){
  return proof_line.front() == 'c';
}

vector<unique_ptr<Clause>> GetCorrespondingDefinition(const RatClause& rat,
                                                      const int new_variable){
  if(rat.GetClause()->size() == 0){
    return vector<unique_ptr<Clause>>{};
  }

  vector<unique_ptr<Clause>> definition{};

  int pivot = rat.GetPivot();

  auto first_clause = make_unique<Clause>(*rat.GetClause());  
  (*first_clause)[0] = new_variable;
  definition.push_back(move(first_clause));

  auto second_clause = make_unique<Clause>();
  second_clause->AddLiteral(new_variable);
  second_clause->AddLiteral(-pivot);
  definition.push_back(move(second_clause));

  for(int i=1; i < rat.GetClause()->size(); i++){
    auto negative_clause = make_unique<Clause>(); 
    negative_clause->AddLiteral(-new_variable);
    negative_clause->AddLiteral(pivot);
    negative_clause->AddLiteral(-(*rat.GetClause())[i]);
    definition.push_back(move(negative_clause));
  }

  return definition;
}

void RatEliminator::Apply(Formula& formula, const string& input_proof_file, 
                          const string& output_proof_file){
  ifstream input_stream {input_proof_file, ifstream::in};
  ofstream output_stream {output_proof_file};
  string proof_line;
  while(getline(input_stream, proof_line)) {
    if(IsComment(proof_line)){
      continue;
    }
    if(IsProperRatAddition(proof_line)){
      auto rat_clause = ParseRat(proof_line);
      //int old_literal, new_literal;
      //AddDefinitionsForRatClause(*rat_clause, formula);
      //ReplaceOldLiteralByNew(old_literal, new_literal);
      //DeleteClausesWithOldLiteral(old_literal);
      //RenameLiteralInRemainingProof(old_literal, new_literal);
      output_stream << proof_line << endl;
    } else {
      output_stream << proof_line << endl;
    }
  }
}

void RatEliminator::AddDefinitionsForRatClause(const RatClause& clause, 
    Formula& formula){
}

void RatEliminator::ReplaceOldLiteralByNew(const int old_literal, 
                                           const int new_literal){
}

void RatEliminator::DeleteClausesWithOldLiteral(const int old_literal){
}

void RatEliminator::RenameLiteralInRemainingProof(const int old_literal, 
                                    const int new_literal){
}

} // namespace
