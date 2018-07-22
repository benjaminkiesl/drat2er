#include "formula.h"
#include <memory>
#include <iostream>
#include <algorithm>

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::find;
using std::move;
using std::cout;
using std::cerr;
using std::endl;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_variables),
max_variable_{0},
occurrences_(2*number_of_variables,std::vector<shared_ptr<Clause>> {}) { }

void Formula::AddClauses(const std::vector<Clause>& clauses){
  for(auto clause : clauses){
    AddClause(clause);
  }
}

void Formula::AddClause(const Clause& clause)
{
  auto new_clause = make_shared<Clause>(clause);

  if(new_clause->GetMaxVariable() > MaxVariable()){
    max_variable_ = new_clause->GetMaxVariable();
    if(max_variable_ * 2ll >= occurrences_.size()){
      occurrences_.resize(static_cast<int>(max_variable_*2.2)); 
    }
  }

  if(new_clause->GetIndex() >= int(clauses_.size())){
    clauses_.resize(static_cast<int>((new_clause->GetIndex()+1)*1.2));
    clauses_[new_clause->GetIndex()] = new_clause;
  } else {
    clauses_.emplace_back(new_clause);
    new_clause->SetIndex(clauses_.size()-1);
  }

  for(auto literal : *new_clause) {
    Occurrences(literal).emplace_back(new_clause);
  }
}

std::shared_ptr<Clause> Formula::GetClause(const int clause_index) const
{
  return clauses_[clause_index];
}

void Formula::DeleteClause(const int clause_index)
{
  auto clause = clauses_[clause_index];
  for(auto literal : *clause){
    auto& occurrences = Occurrences(literal);
    occurrences.erase(find(occurrences.begin(), occurrences.end(), clause));
  }
  clauses_[clause_index] = nullptr;
}

void Formula::DeleteClauses(const std::vector<int>& clause_indices){
  for(auto clause_index : clause_indices){
    DeleteClause(clause_index);
  }
}

int Formula::MaxVariable(){
  return max_variable_;
}

vector<shared_ptr<Clause>>& Formula::Occurrences(int literal)
{
  int index = literal > 0 ? 2*literal - 2 : (-2)*literal - 1;
  if(index >= occurrences_.size()) {
    cerr << "Index is out of bounds [0," << occurrences_.size()-1 << "]: "
    << index << endl;
  }
  return occurrences_[index];
}

} // namespace
