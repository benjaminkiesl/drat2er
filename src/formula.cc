#include "formula.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include <cassert>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::find;
using std::cerr;
using std::endl;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_clauses, nullptr),
occurrences_{},
empty_occurrence_list_{}
{ }

void Formula::AddClauses(const vector<Clause>& clauses){
  for(auto& clause : clauses){
    AddClause(clause);
  }
}

void Formula::AddClause(const Clause& clause)
{
  assert(clause.GetIndex() != -1);
  auto new_clause = make_shared<Clause>(clause);

  if(new_clause->GetIndex() >= int(clauses_.size())){
    clauses_.resize(static_cast<int>((new_clause->GetIndex()+1)*1.3), nullptr);
  } else if(clauses_[new_clause->GetIndex()] != nullptr){
    cerr << "Formula::AddClause(): Clause with index " << 
      new_clause->GetIndex() << " is already contained.";
    return;
  }

  clauses_[new_clause->GetIndex()] = new_clause;

  for(auto literal : *new_clause) {
    occurrences_[literal].emplace_back(new_clause);
  }
}

shared_ptr<Clause> Formula::GetClause(const int clause_index) const
{
  if(clause_index < clauses_.size()){
    return clauses_[clause_index];
  }
  return nullptr;
}

const vector<shared_ptr<Clause>>& Formula::GetClauses() {
  return clauses_;
}

void Formula::DeleteClause(const int clause_index)
{
  if(clause_index >= clauses_.size() || clauses_[clause_index] == nullptr){
    return;
  }
  auto clause = clauses_[clause_index];
  for(auto literal : *clause){
    if(occurrences_.find(literal) != occurrences_.end()){
      occurrences_[literal].erase(find(occurrences_[literal].begin(), 
                                       occurrences_[literal].end(), clause));
    }
  }
  clauses_[clause_index] = nullptr;
}

void Formula::DeleteClauses(const vector<int>& clause_indices){
  for(auto clause_index : clause_indices){
    DeleteClause(clause_index);
  }
}

const vector<shared_ptr<Clause>>& Formula::Occurrences(int literal){
  if(occurrences_.find(literal) != occurrences_.end()){
    return occurrences_.at(literal);
  }
  return empty_occurrence_list_;
}

} // namespace
