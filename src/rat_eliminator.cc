#include "rat_eliminator.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include "formula.h"
#include "clause.h"
#include "lrat_parser.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"
#include "progress_bar.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::unordered_map;
using std::max;
using std::endl;
using std::cout;
using std::shared_ptr;

namespace drat2er
{

RatEliminator::RatEliminator(string output_file, shared_ptr<Formula> formula,
                             int max_variable, int max_instruction,
                             int number_of_proper_rats_overall,
                             bool output_lrat) : 
               formula_{formula},
               max_variable_{max_variable},
               max_instruction_{max_instruction},
               number_of_proper_rats_{0},
               number_of_proper_rats_overall_{number_of_proper_rats_overall},
               output_lrat_{output_lrat},
               output_stream_{output_file},
               old_to_new_literal_{},
               progress_bar_{}
               { }

void RatEliminator::ObserveProperRatAddition(const RatClause& unrenamed_rat){
  auto rat = RenameRat(unrenamed_rat);
  ReplaceByDefinitionRUPsAndDeletions(rat);
  PrintProgress(++number_of_proper_rats_);
}

void RatEliminator::ObserveRupAddition(const RupClause& unrenamed_rup){
  auto rup = RenameRup(unrenamed_rup);
  formula_->AddClause(rup);
  WriteRupToOutput(rup);
}

void RatEliminator::ObserveDeletion(const Deletion& unrenamed_deletion){
  auto deletion = RenameDeletion(unrenamed_deletion);
  formula_->DeleteClauses(deletion.GetClauseIndices());
  WriteDeletionToOutput(deletion);
} 

void RatEliminator::ObserveComment(const string& comment_line){
  // do nothing
}

void RatEliminator::ReplaceByDefinitionRUPsAndDeletions(const RatClause& rat){
  int new_variable = ++max_variable_;

  auto definition_clauses = CorrespondingDefinition(rat, new_variable);
  WriteDefinitionToOutput(definition_clauses);
  formula_->AddClause(definition_clauses.front());

  ReplaceOldPivotByNew(rat, definition_clauses);
  DeleteClausesWithOldVariable(abs(rat.GetPivot()), definition_clauses);
  UpdateLiteralRenaming(rat.GetPivot(), new_variable);
}

vector<Clause> RatEliminator::CorrespondingDefinition(const RatClause& rat,
                                              const int new_variable) {
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
                                            const int new_variable) const {
  Clause clause;
  clause.SetIndex(rat.GetIndex());
  clause.AddLiteral(new_variable);
  for(auto it = rat.cbegin()+1; it != rat.cend(); ++it){
    clause.AddLiteral(*it);
  }
  return clause;
}

Clause RatEliminator::SecondDefinitionClause(const RatClause& rat, 
                                             const int new_variable) {
  Clause second_clause{new_variable, -rat.GetPivot()};
  second_clause.SetIndex(++max_instruction_);
  return second_clause;
}

vector<Clause> RatEliminator::ThirdBlockOfDefinitionClauses(
                                                    const RatClause& rat,
                                                    const int new_variable){
  vector<Clause> definition_clauses;
  for(auto it=rat.cbegin()+1; it != rat.cend(); ++it){
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
  int new_literal = *definition.front().cbegin();
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
    UpdateClauseRenaming(clause->GetIndex(), rup.GetIndex());
  }
}  

void RatEliminator::ReplaceNegativePivot(const RatClause& rat,
                                         const vector<Clause>& definition){
  int new_literal = *definition.front().cbegin();
  for(auto& resolution_partner : formula_->Occurrences(-rat.GetPivot())){
    RupClause rup;
    rup.SetIndex(++max_instruction_);
    for(auto literal : *resolution_partner){
      rup.AddLiteral(literal == -rat.GetPivot() ? -new_literal : literal);
    }
    rup.AddPositiveHint(resolution_partner->GetIndex());
    for(int i = 2;i < definition.size();i++){
      rup.AddPositiveHint(definition[i].GetIndex());
    }
    for(auto hint : rat.GetPositiveHints()){
      rup.AddPositiveHint(hint);
    }
    if(rat.GetNegativeHints().find(resolution_partner->GetIndex()) 
        != rat.GetNegativeHints().end()){
      for(auto hint : rat.GetNegativeHints().at(
            resolution_partner->GetIndex())){
        rup.AddPositiveHint(hint);
      }
    }    
    WriteRupToOutput(rup);
    formula_->AddClause(rup);
    UpdateClauseRenaming(resolution_partner->GetIndex(), rup.GetIndex());
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

RupClause RatEliminator::RenameRup(const RupClause& rup) const {
  RupClause renamed_rup;
  renamed_rup.SetIndex(rup.GetIndex());
  for(auto literal : rup.GetLiteralsConst()){
    renamed_rup.AddLiteral(RenameLiteral(literal));
  }
  for(auto positive_hint : rup.GetPositiveHints()){
    renamed_rup.AddPositiveHint(RenameClauseIndex(positive_hint));
  }
  return renamed_rup;
}

RatClause RatEliminator::RenameRat(const RatClause& rat) const {
  RatClause renamed_rat;
  renamed_rat.SetIndex(rat.GetIndex());
  for(auto literal : rat.GetLiteralsConst()){
    renamed_rat.AddLiteral(RenameLiteral(literal));
  }
  for(auto positive_hint : rat.GetPositiveHints()){
    renamed_rat.AddPositiveHint(RenameClauseIndex(positive_hint));
  }
  for(auto negative_hint : rat.GetNegativeHints()){
    int resolution_partner = RenameClauseIndex(negative_hint.first);
    vector<int> hints_for_partner{};
    for(auto hint : negative_hint.second){
      hints_for_partner.emplace_back(RenameClauseIndex(hint));
    }
    renamed_rat.AddNegativeHint(resolution_partner, hints_for_partner);
  }

  return renamed_rat;
}

Deletion RatEliminator::RenameDeletion(const Deletion& deletion) const {
  Deletion renamed;
  renamed.SetIndex(deletion.GetIndex());
  for(auto clause_index : deletion.GetClauseIndices()){
    renamed.AddClauseIndex(RenameClauseIndex(clause_index));
  }
  return renamed;
}

int RatEliminator::RenameClauseIndex(const int clause_index) const {
  if(old_to_new_clause_.find(clause_index) != old_to_new_clause_.end()){
    return old_to_new_clause_.at(clause_index);
  }
  return clause_index;
}

int RatEliminator::RenameLiteral(const int literal) const {
  if(old_to_new_literal_.find(literal) != old_to_new_literal_.end()){
    return old_to_new_literal_.at(literal);
  }
  return literal;
}

void RatEliminator::UpdateLiteralRenaming(const int old_literal, 
                                          const int new_literal){
  UpdateMapping(old_to_new_literal_, new_to_old_literal_, 
                old_literal, new_literal);
}

void RatEliminator::UpdateClauseRenaming(const int old_index, 
                                         const int new_index){
  UpdateMapping(old_to_new_clause_, new_to_old_clause_, 
                old_index, new_index);
}

void RatEliminator::UpdateMapping(unordered_map<int,int>& mapping,
                          unordered_map<int,int>& inverse_mapping,
                          const int old_value, const int new_value){
  int original_value = old_value;
  if(inverse_mapping.find(old_value) != inverse_mapping.end()){
    original_value = inverse_mapping[old_value];
  } 
  mapping[original_value] = new_value;
  inverse_mapping[new_value] = original_value;
  mapping[-original_value] = -new_value;
  inverse_mapping[-new_value] = -original_value;
}


void RatEliminator::WriteRupToOutput(const RupClause& rup) {
  if(output_lrat_){
    output_stream_ << rup.ToLrat() << endl;
  } else {
    output_stream_ << rup.ToDimacs() << endl;
  }
}

void RatEliminator::WriteDefinitionToOutput(
                                      const vector<Clause>& definition){
  if(output_lrat_){
    for(auto clause : definition){
      output_stream_ << clause.GetIndex() << " e " 
        << clause.ToDimacs() << endl;
    }
  } else {
    for(auto clause : definition){
      output_stream_ << clause.ToDimacs() << endl;
    }
  }
}

void RatEliminator::WriteDeletionToOutput(const Deletion& deletion) {
  if(output_lrat_){
    output_stream_ << deletion.GetIndex() << ' ';
    output_stream_ << "d ";
    for(auto index : deletion.GetClauseIndices()){
      output_stream_ << index << " ";
    }
    output_stream_ << "0" << endl;
  } else {
    for(auto index : deletion.GetClauseIndices()){
      if(formula_->GetClause(index) != nullptr){
        output_stream_ << "d " << formula_->GetClause(index)->ToDimacs() << endl;
      }
    }
  }
}

void RatEliminator::WriteDeletionToOutput(const vector<Clause>& clauses,
                                          int instruction_index){
  if(output_lrat_){
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

void RatEliminator::PrintProgress(int number_of_proper_rats_eliminated){
  if(number_of_proper_rats_overall_ != 0){
    progress_bar_.PrintProgress(
      double(number_of_proper_rats_eliminated)/number_of_proper_rats_overall_);
  }
}


} // namespace
