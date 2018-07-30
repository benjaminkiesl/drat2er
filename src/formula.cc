#include "formula.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cassert>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::unordered_map;
using std::find;
using std::cerr;
using std::endl;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_clauses),
occurrences_(2*number_of_variables),
watch_table_(2*number_of_variables),
empty_occurrence_list_{},
empty_watch_list_{}
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
  clauses_[new_clause->GetIndex()] = new_clause;

  for(auto literal : *new_clause) {
    occurrences_[literal].emplace_back(new_clause);
  }

  
}

shared_ptr<Clause> Formula::GetClause(const int clause_index) const
{
  if(clauses_.find(clause_index) != clauses_.end()){
    return clauses_.at(clause_index);
  }
  return nullptr;
}

const unordered_map<int, shared_ptr<Clause>>& Formula::GetClauses() {
  return clauses_;
}

void Formula::DeleteClause(const int clause_index)
{
  if(clauses_.find(clause_index) == clauses_.end()){
    return;
  }

  auto clause = clauses_[clause_index];
  for(auto literal : *clause){
    if(occurrences_.find(literal) != occurrences_.end()){
      occurrences_[literal].erase(find(occurrences_[literal].begin(), 
                                       occurrences_[literal].end(), clause));
    }
  }

  clauses_.erase(clause_index);
}

void Formula::DeleteClauses(const vector<int>& clause_indices){
  for(auto clause_index : clause_indices){
    DeleteClause(clause_index);
  }
}

const OccurrenceList& Formula::Occurrences(const int literal){
  if(occurrences_.find(literal) != occurrences_.end()){
    return occurrences_.at(literal);
  }
  return empty_occurrence_list_;
}

WatchList& Formula::Watches(const int literal){
  if(occurrences_.find(literal) != occurrences_.end()){
    return watch_table_.at(literal);
  }
  return empty_watch_list_;
}

} // namespace
