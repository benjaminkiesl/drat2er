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

void RatEliminator::HandleProperRatAddition(const RatClause& rat){
  int new_variable = ++max_variable_;
  auto definition_clauses = CorrespondingDefinition(rat, new_variable);
  WriteDefinitionToOutput(definition_clauses);
  formula_->AddClause(definition_clauses.front());

  ReplaceOldLiteralByNew(rat, new_variable, 
                         definition_clauses.front().GetIndex(), 
                         definition_clauses.back().GetIndex());

  DeleteClausesWithOldVariable(abs(rat.GetPivot()));
}

void RatEliminator::HandleRupAddition(const RupClause& rup){
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

  if(rat.GetClause().size() == 0){
    return definition;
  }

  Clause first_clause;
  first_clause.SetIndex(++max_instruction_);
  first_clause.AddLiteral(new_variable);
  for(auto it = rat.GetClause().begin()+1; it != rat.GetClause().end(); ++it){
    first_clause.AddLiteral(ActualLiteral(*it));
  }
  definition.emplace_back(first_clause);

  Clause second_clause;
  second_clause.SetIndex(++max_instruction_);
  second_clause.AddLiteral(new_variable); 
  second_clause.AddLiteral(ActualLiteral(-rat.GetPivot()));
  definition.emplace_back(second_clause);

  for(auto it=rat.GetClause().begin()+1; it != rat.GetClause().end(); ++it){
    Clause negative_clause;
    negative_clause.SetIndex(++max_instruction_);
    negative_clause.AddLiteral(-new_variable);
    negative_clause.AddLiteral(ActualLiteral(rat.GetPivot()));
    negative_clause.AddLiteral(-ActualLiteral(*it));
    definition.emplace_back(negative_clause);
  }

  old_to_new_literal_[rat.GetPivot()] = new_variable;
  old_to_new_literal_[-rat.GetPivot()] = -new_variable;

  return definition;
}

void RatEliminator::ReplaceOldLiteralByNew(const RatClause& rat, 
                                           const int new_literal,
                                           const int index_of_first_extension,
                                           const int index_of_last_extension){
  auto& pivot_occurrences = 
    formula_->Occurrences(ActualLiteral(rat.GetPivot()));
  auto& negated_pivot_occurrences = 
    formula_->Occurrences(-ActualLiteral(rat.GetPivot()));

  for(auto& clause : pivot_occurrences){
    RupClause rup;
  }

  for(auto& clause : negated_pivot_occurrences){
  }
}

void RatEliminator::DeleteClausesWithOldVariable(const int old_variable){
  vector<int> clauses_to_delete{};
  auto positive_occurrences = 
    formula_->Occurrences(ActualLiteral(old_variable));
  auto negative_occurrences = 
    formula_->Occurrences(ActualLiteral(-old_variable));
  for(auto clause : positive_occurrences){
    formula_->DeleteClause(clause->GetIndex());
    clauses_to_delete.emplace_back(clause->GetIndex());
  }
  for(auto clause : negative_occurrences){
    formula_->DeleteClause(clause->GetIndex());
    clauses_to_delete.emplace_back(clause->GetIndex());
  }
  WriteDeletionToOutput(clauses_to_delete, ++max_instruction_);
}

int RatEliminator::ActualLiteral(int literal) {
  if(old_to_new_literal_.find(literal) != old_to_new_literal_.end()){
    return old_to_new_literal_[literal];
  }
  return literal;
}

void RatEliminator::WriteRupToOutput(const RupClause& rup){
  output_stream_ << rup.GetIndex() << ' ' << string(rup.GetClause()) << ' ';
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
