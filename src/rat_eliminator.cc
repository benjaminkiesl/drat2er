#include "rat_eliminator.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>
#include "formula.h"
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "lrat_parser.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::max;
using std::endl;
using std::cout;
using std::shared_ptr;

namespace drat2er
{

RatEliminator::RatEliminator(string output_file, shared_ptr<Formula> formula,
                             int max_variable, int max_instruction) : 
                             formula_{formula},
                             max_variable_{max_variable},
                             max_instruction_{max_instruction},
                             output_stream_{output_file},
                             old_to_new_literal_{}
                             { }

void RatEliminator::HandleProperRatAddition(const RatClause& unrenamed_rat){
  RatClause rat(unrenamed_rat);
  ApplyRenaming(rat);
  int new_variable = ++max_variable_;

  auto definition_clauses = CorrespondingDefinition(rat, new_variable);
  WriteDefinitionToOutput(definition_clauses);
  formula_->AddClause(definition_clauses.front());

  ReplaceOldLiteralByNew(rat, definition_clauses);
  DeleteClausesWithOldVariable(abs(rat.GetPivot()));
  UpdateRenaming(rat.GetPivot(), new_variable);
}

void RatEliminator::HandleRupAddition(const RupClause& unrenamed_rup){
  RupClause rup(unrenamed_rup);
  ApplyRenaming(rup);
  formula_->AddClause(rup);
  WriteRupToOutput(rup);
}

void RatEliminator::HandleDeletion(const vector<int>& clause_indices,
                                   int instruction_index){
  WriteDeletionToOutput(clause_indices, instruction_index);
} 

void RatEliminator::HandleComment(const string& comment_line){
  // do nothing
}

vector<Clause> RatEliminator::CorrespondingDefinition(const RatClause& rat,
                                                      const int new_variable){
  vector<Clause> definition{};

  if(rat.size() == 0){
    return definition;
  }

  Clause first_clause;
  first_clause.SetIndex(rat.GetIndex());
  first_clause.AddLiteral(new_variable);
  for(auto it = rat.begin()+1; it != rat.end(); ++it){
    first_clause.AddLiteral(*it);
  }
  definition.emplace_back(first_clause);

  Clause second_clause;
  second_clause.SetIndex(++max_instruction_);
  second_clause.AddLiteral(new_variable); 
  second_clause.AddLiteral(-rat.GetPivot());
  definition.emplace_back(second_clause);

  for(auto it=rat.begin()+1; it != rat.end(); ++it){
    Clause negative_clause;
    negative_clause.SetIndex(++max_instruction_);
    negative_clause.AddLiteral(-new_variable);
    negative_clause.AddLiteral(rat.GetPivot());
    negative_clause.AddLiteral(-*it);
    definition.emplace_back(negative_clause);
  }

  return definition;
}

void RatEliminator::ReplaceOldLiteralByNew(const RatClause& rat, 
                                           const vector<Clause>& definition){
  int new_literal = *definition.front().begin();
  for(auto& clause : formula_->Occurrences(rat.GetPivot())){
    RupClause rup;
    rup.SetIndex(++max_instruction_);
    for(auto literal : *clause){
      rup.AddLiteral(literal == rat.GetPivot() ? new_literal : literal);
    }
    rup.AddHint(clause->GetIndex());
    rup.AddHint(definition[1].GetIndex());
    WriteRupToOutput(rup);
    formula_->AddClause(rup);
  }

  for(auto& clause : formula_->Occurrences(-rat.GetPivot())){
    RupClause rup;
    rup.SetIndex(++max_instruction_);
    for(auto literal : *clause){
      rup.AddLiteral(literal == -rat.GetPivot() ? -new_literal : literal);
    }
    rup.AddHint(clause->GetIndex());
    for(int i = 2;i < definition.size();i++){
      rup.AddHint(definition[i].GetIndex());
    }
    // TODO: Add missing hints
    WriteRupToOutput(rup);
    formula_->AddClause(rup);
  }
}

void RatEliminator::DeleteClausesWithOldVariable(const int old_variable){
  vector<int> clauses_to_delete{};
  for(auto clause : formula_->Occurrences(old_variable)){
    formula_->DeleteClause(clause->GetIndex());
    clauses_to_delete.emplace_back(clause->GetIndex());
  }
  for(auto clause : formula_->Occurrences(-old_variable)){
    formula_->DeleteClause(clause->GetIndex());
    clauses_to_delete.emplace_back(clause->GetIndex());
  }
  WriteDeletionToOutput(clauses_to_delete, ++max_instruction_);
}

int RatEliminator::Rename(const int literal) {
  if(old_to_new_literal_.find(literal) != old_to_new_literal_.end()){
    return old_to_new_literal_[literal];
  }
  return literal;
}

void RatEliminator::ApplyRenaming(Clause& clause){
  vector<int> renamed_literals;
  for(auto literal : clause){
    renamed_literals.emplace_back(Rename(literal));
  }
  clause.SetLiterals(renamed_literals);
}

void RatEliminator::UpdateRenaming(int old_literal, int new_literal){
  int original_literal = old_literal;
  if(new_to_old_literal_.find(old_literal) != new_to_old_literal_.end()){
    original_literal = new_to_old_literal_[old_literal];
  } 
  old_to_new_literal_[original_literal] = new_literal;
  new_to_old_literal_[new_literal] = original_literal;
  old_to_new_literal_[-original_literal] = -new_literal;
  new_to_old_literal_[-new_literal] = -original_literal;
}

void RatEliminator::WriteRupToOutput(const RupClause& rup){
  output_stream_ << rup.GetIndex() << ' ' << string(rup) << ' ';
  for(auto hint : rup.GetHints()){
    output_stream_ << hint << ' ';
  }
  output_stream_ << '0' << endl;
}

void RatEliminator::WriteDefinitionToOutput(const vector<Clause>& definition){
  for(auto clause : definition){
    output_stream_ << clause.GetIndex() << " e " << string(clause) << endl;
  }
}

void RatEliminator::WriteDeletionToOutput(const vector<int>& clause_indices,
                                          int instruction_index){
  output_stream_ << instruction_index << " d ";
  for(auto index : clause_indices){
    output_stream_ << index << " ";
  }
  output_stream_ << "0" << endl;
}


} // namespace
