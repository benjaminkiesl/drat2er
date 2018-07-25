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

  ReplaceOldPivotByNew(rat, definition_clauses);
  DeleteClausesWithOldVariable(abs(rat.GetPivot()), definition_clauses);
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
  formula_->DeleteClauses(clause_indices);
  WriteDeletionToOutput(clause_indices, instruction_index);
} 

void RatEliminator::HandleComment(const string& comment_line){
  // do nothing
}

vector<Clause> RatEliminator::CorrespondingDefinition(const RatClause& rat,
                                                      const int new_variable){
  vector<Clause> definition{};

  if(rat.empty()){
    return definition;
  }

  definition.emplace_back(FirstDefinitionClause(rat, new_variable));
  definition.emplace_back(SecondDefinitionClause(rat, new_variable));
  auto third_block_of_definition_clauses = 
    ThirdBlockOfDefinitionClauses(rat, new_variable);
  definition.insert(definition.end(), 
                    third_block_of_definition_clauses.begin(),
                    third_block_of_definition_clauses.end());

  return definition;
}

Clause RatEliminator::FirstDefinitionClause(const RatClause& rat,
                                            const int new_variable){
  Clause clause;
  clause.SetIndex(rat.GetIndex());
  clause.AddLiteral(new_variable);
  for(auto it = rat.begin()+1; it != rat.end(); ++it){
    clause.AddLiteral(*it);
  }
  return clause;
}

Clause RatEliminator::SecondDefinitionClause(const RatClause& rat, 
                                             const int new_variable){
  Clause second_clause{new_variable, -rat.GetPivot()};
  second_clause.SetIndex(++max_instruction_);
  return second_clause;
}

vector<Clause> RatEliminator::ThirdBlockOfDefinitionClauses(
                                                    const RatClause& rat,
                                                    const int new_variable){
  vector<Clause> definition_clauses;
  for(auto it=rat.begin()+1; it != rat.end(); ++it){
    Clause negative_clause{-new_variable, rat.GetPivot(), -*it};
    negative_clause.SetIndex(++max_instruction_);
    definition_clauses.emplace_back(negative_clause);
  }
  return definition_clauses;
}

void RatEliminator::ReplaceOldPivotByNew(const RatClause& rat, 
                                         const vector<Clause>& definition){
  ReplacePositivePivot(rat, definition);
  ReplaceNegativePivot(rat, definition);
}

void RatEliminator::ReplacePositivePivot(const RatClause& rat,
                                         const vector<Clause>& definition){
  int new_literal = *definition.front().begin();
  for(auto& clause : formula_->Occurrences(rat.GetPivot())){
    RupClause rup;
    rup.SetIndex(++max_instruction_);
    for(auto literal : *clause){
      rup.AddLiteral(literal == rat.GetPivot() ? new_literal : literal);
    }
    rup.AddPositiveHint(clause->GetIndex());
    rup.AddPositiveHint(definition[1].GetIndex());
    WriteRupToOutput(rup);
    formula_->AddClause(rup);
  }
}  

void RatEliminator::ReplaceNegativePivot(const RatClause& rat,
                                         const vector<Clause>& definition){
  int new_literal = *definition.front().begin();
  for(auto& clause : formula_->Occurrences(-rat.GetPivot())){
    RupClause rup;
    rup.SetIndex(++max_instruction_);
    for(auto literal : *clause){
      rup.AddLiteral(literal == -rat.GetPivot() ? -new_literal : literal);
    }
    rup.AddPositiveHint(clause->GetIndex());
    for(int i = 2;i < definition.size();i++){
      rup.AddPositiveHint(definition[i].GetIndex());
    }
    for(auto hint : rat.GetPositiveHints()){
      rup.AddPositiveHint(hint);
    }
    if(rat.GetNegativeHints().find(clause->GetIndex()) 
        != rat.GetNegativeHints().end()){
      for(auto hint : rat.GetNegativeHints().at(clause->GetIndex())){
        rup.AddPositiveHint(hint);
      }
    }    
    WriteRupToOutput(rup);
    formula_->AddClause(rup);
  }
}  

void RatEliminator::DeleteClausesWithOldVariable(const int old_variable,
                                           const vector<Clause>& definition){
  vector<Clause> clauses_to_delete{};
  for(auto clause : formula_->Occurrences(old_variable)){
    clauses_to_delete.emplace_back(*clause);
  }
  for(auto clause : formula_->Occurrences(-old_variable)){
    clauses_to_delete.emplace_back(*clause);
  }
  for(auto clause : clauses_to_delete){
    formula_->DeleteClause(clause.GetIndex());
  }
  for(int i=1; i < definition.size(); i++){
    clauses_to_delete.emplace_back(definition[i]);
  }
  WriteDeletionToOutput(clauses_to_delete, max_instruction_);
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
  bool lrat = false;
  if(lrat){
    output_stream_ << rup.ToLrat() << endl;
  } else {
    output_stream_ << rup.ToDimacs() << endl;
  }
}

void RatEliminator::WriteDefinitionToOutput(const vector<Clause>& definition){
  bool lrat = false;
  if(lrat){
    for(auto clause : definition){
      output_stream_ << clause.GetIndex() << " e " << clause.ToDimacs() << endl;
    }
  } else {
    for(auto clause : definition){
      output_stream_ << clause.ToDimacs() << endl;
    }
  }
}

void RatEliminator::WriteDeletionToOutput(const vector<int>& clause_indices,
                                          int instruction_index){
  bool lrat = false;
  if(lrat){
    output_stream_ << instruction_index << ' ';
    output_stream_ << "d ";
    for(auto index : clause_indices){
      output_stream_ << index << " ";
    }
    output_stream_ << "0" << endl;
  } else {
    for(auto index : clause_indices){
      if(formula_->GetClause(index) != nullptr){
        output_stream_ << "d " << formula_->GetClause(index)->ToDimacs() << endl;
      }
    }
  }
}

void RatEliminator::WriteDeletionToOutput(const vector<Clause>& clauses,
                                          int instruction_index){
  bool lrat = false;
  if(lrat){
    output_stream_ << instruction_index << ' ';
    output_stream_ << "d ";
    for(auto clause : clauses){
      output_stream_ << clause.GetIndex() << " ";
    }
    output_stream_ << "0" << endl;
  } else {
    for(auto clause : clauses){
      output_stream_ << "d " << clause.ToDimacs() << endl;
    }
  }
}

} // namespace
