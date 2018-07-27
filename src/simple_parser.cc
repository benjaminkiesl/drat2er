#include "simple_parser.h"
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "formula.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"

using std::string;
using std::shared_ptr;
using std::vector;
using std::unordered_set;
using std::cout;
using std::endl;
using std::find;
using std::find_if;

namespace drat2er {

SimpleParser::SimpleParser(shared_ptr<Formula> formula) : formula_(formula){}

void SimpleParser::HandleDeletion(const Deletion& deletion){
  formula_->DeleteClauses(deletion.GetClauseIndices());
  for(auto index : deletion.GetClauseIndices()){
    deleted_clauses_.insert(index);
  }
} 

void SimpleParser::HandleProperRatAddition(const RatClause& rat){
  if(rat.GetNegativeHints().size() != 
     formula_->Occurrences(-rat.GetPivot()).size()){
    cout << "Number of negative hints (" << rat.GetNegativeHints().size() <<
      ") does not equal number of negative occurrences (" <<
      formula_->Occurrences(-rat.GetPivot()).size() << ")." << endl;
  }
  for(auto clause : formula_->Occurrences(-rat.GetPivot())){
    auto hint_has_clause_index = [&](const auto& hint){ 
      return hint.first == clause->GetIndex(); 
    };
    if(find_if(rat.GetNegativeHints().begin(), rat.GetNegativeHints().end(),
               hint_has_clause_index) == rat.GetNegativeHints().end()){
      cout << "Clause " << clause->GetIndex() << " = '" << clause->ToLrat() <<
       "' is in occurrence list of " << -rat.GetPivot() << 
       " but not in the NegativeHints of '" << rat.ToLrat() << "'." << endl;
      if(deleted_clauses_.find(clause->GetIndex()) != deleted_clauses_.end()){
        cout << "Clause has been deleted." << endl;
      }
    }
  }
  for(auto hint : rat.GetNegativeHints()){
    if(formula_->GetClause(hint.first) == nullptr){
      cout << "Clause " << hint.first << " is in hints of "
        << rat.GetIndex() << " but not in the formula." << endl;
    }
  }
  formula_->AddClause(rat);
  added_clauses_.insert(rat.GetIndex());
}

void SimpleParser::HandleRupAddition(const RupClause& rup){
  formula_->AddClause(rup);
  added_clauses_.insert(rup.GetIndex());
}

void SimpleParser::HandleComment(const string& comment_line){
  // do nothing
}

}
