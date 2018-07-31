#include "formula.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>
#include "clause.h"
#include "watch.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::unordered_map;
using std::find;
using std::cerr;
using std::endl;
using std::swap;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_clauses),
unit_clauses_{},
occurrences_(2*number_of_variables),
watch_table_(2*number_of_variables),
assignment_{},
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
  clauses_[new_clause->GetIndex()] = new_clause;

  for(auto literal : *new_clause) {
    occurrences_[literal].emplace_back(new_clause);
  }

  if(new_clause->GetLiterals().size() >= 1){
    Watches(new_clause->GetLiterals()[0]).emplace_back(new_clause);
    if(new_clause->GetLiterals().size() >= 2){
      Watches(new_clause->GetLiterals()[1]).emplace_back(new_clause);
    } else {
      unit_clauses_.emplace_back(new_clause);
    }
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
    DeleteClauseFromOccurrenceList(clause, literal);
  }

  int i=0; auto literal_iterator = clause->begin();
  while(literal_iterator != clause->end() && i < 2){
    DeleteClauseFromWatchList(clause, *literal_iterator);
    ++literal_iterator; ++i;
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

void Formula::DeleteClauseFromOccurrenceList(shared_ptr<Clause> clause, 
                                            const int literal){
  if(occurrences_.find(literal) != occurrences_.end()){
    auto iterator_to_clause = find(occurrences_[literal].begin(), 
                                   occurrences_[literal].end(), clause);
    if(iterator_to_clause != occurrences_[literal].end()){
      occurrences_[literal].erase(iterator_to_clause);
    }
  }
}

WatchList& Formula::Watches(const int literal){
  return watch_table_[literal];
}

void Formula::DeleteClauseFromWatchList(shared_ptr<Clause> clause, 
                                        const int literal){
  if(watch_table_.find(literal) != watch_table_.end()){
    auto iterator_to_watch = find_if(watch_table_[literal].begin(),
                                     watch_table_[literal].end(),
                                     [&] (auto watch) { 
                                       return watch.GetClause() == clause; 
                                     });
    if(iterator_to_watch != watch_table_[literal].end()){
      watch_table_[literal].erase(iterator_to_watch);
    }
  }
}

inline int sign(const int number) {
  return (number > 0) - (0 > number);
}

int Formula::GetTruthValue(const int literal){
  return sign(literal) * assignment_[abs(literal)];
}

void Formula::Satisfy(const int literal){
  assignment_[abs(literal)] = sign(literal);
}

void Formula::Falsify(const int literal){
  assignment_[abs(literal)] = -sign(literal);
}

void Formula::Unassign(const int literal){
  assignment_[abs(literal)] = 0;
}

bool Formula::Propagate() {
  shared_ptr<Clause> conflict = nullptr;
  while(conflict == nullptr && !unit_clauses_.empty()){
    auto literal = -unit_clauses_.front()->GetLiterals().front();
    Falsify(literal);
    auto& watches = Watches(literal);
    for(auto& watch : watches){
      auto clause = watch.GetClause();
      if(clause->IsUnit()){
        conflict = clause;
        break;
      }
      if(clause->GetLiterals()[1] != literal){
        swap(clause->GetLiterals()[0], clause->GetLiterals()[1]);
      }

    }
  }
  return conflict == nullptr;
}

} // namespace
