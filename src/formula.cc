#include "formula.h"
#include <memory>
#include <vector>
#include <unordered_set>
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
using std::unordered_set;
using std::unordered_map;
using std::find;
using std::copy_if;
using std::back_inserter;
using std::cout;
using std::cerr;
using std::endl;
using std::swap;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_clauses),
unit_clauses_ {},
              occurrences_(2*number_of_variables),
              watch_table_(2*number_of_variables),
              assignment_ {},
              empty_occurrence_list_ {}
{ }

void Formula::AddClauses(const vector<Clause>& clauses)
{
  for(auto& clause : clauses) {
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

  if(new_clause->GetLiterals().size() >= 1) {
    Watches(new_clause->GetLiterals()[0]).emplace_back(new_clause);
    if(new_clause->GetLiterals().size() >= 2) {
      Watches(new_clause->GetLiterals()[1]).emplace_back(new_clause);
    } else {
      unit_clauses_.emplace_back(new_clause);
    }
  }

}

shared_ptr<Clause> Formula::GetClause(const int clause_index) const
{
  if(clauses_.find(clause_index) != clauses_.end()) {
    return clauses_.at(clause_index);
  }
  return nullptr;
}

const unordered_map<int, shared_ptr<Clause>>& Formula::GetClauses()
{
  return clauses_;
}

void Formula::DeleteClause(const int clause_index)
{
  if(clauses_.find(clause_index) == clauses_.end()) {
    return;
  }

  auto clause = clauses_[clause_index];

  for(auto literal : *clause) {
    DeleteClauseFromOccurrenceList(clause, literal);
  }

  int i=0;
  auto literal_iterator = clause->begin();
  while(literal_iterator != clause->end() && i < 2) {
    DeleteClauseFromWatchList(clause, *literal_iterator);
    ++literal_iterator;
    ++i;
  }

  clauses_.erase(clause_index);
}

void Formula::DeleteClauses(const vector<int>& clause_indices)
{
  for(auto clause_index : clause_indices) {
    DeleteClause(clause_index);
  }
}

const OccurrenceList& Formula::Occurrences(const int literal)
{
  if(occurrences_.find(literal) != occurrences_.end()) {
    return occurrences_.at(literal);
  }
  return empty_occurrence_list_;
}

void Formula::DeleteClauseFromOccurrenceList(shared_ptr<Clause> clause,
    const int literal)
{
  if(occurrences_.find(literal) != occurrences_.end()) {
    auto iterator_to_clause = find(occurrences_[literal].begin(),
                                   occurrences_[literal].end(), clause);
    if(iterator_to_clause != occurrences_[literal].end()) {
      occurrences_[literal].erase(iterator_to_clause);
    }
  }
}

WatchList& Formula::Watches(const int literal)
{
  return watch_table_[literal];
}

void Formula::DeleteClauseFromWatchList(shared_ptr<Clause> clause,
                                        const int literal)
{
  if(watch_table_.find(literal) != watch_table_.end()) {
    auto iterator_to_watch = find_if(watch_table_[literal].begin(),
                                     watch_table_[literal].end(),
    [&] (auto watch) {
      return watch.GetClause() == clause;
    });
    if(iterator_to_watch != watch_table_[literal].end()) {
      watch_table_[literal].erase(iterator_to_watch);
    }
  }
}

inline int Sign(const int number)
{
  return (number > 0) - (0 > number);
}

int Formula::TruthValue(const int literal)
{
  return Sign(literal) * assignment_[abs(literal)];
}

void Formula::Satisfy(const int literal)
{
  assignment_[abs(literal)] = Sign(literal);
}

void Formula::Falsify(const int literal)
{
  assignment_[abs(literal)] = -Sign(literal);
}

void Formula::Unassign(const int literal)
{
  assignment_[abs(literal)] = 0;
}

auto Formula::IteratorToUnfalsifiedUnwatchedLiteral(Clause& clause)
{
  for(auto it = clause.begin()+2; it != clause.end(); ++it) {
    if(TruthValue(*it) >= kUnassigned) {
      return it;
    }
  }
  return clause.end();
}

Clause Resolve(const Clause& first, const Clause& second, const int pivot){
  unordered_set<int> resolvent_literals;
  for(auto it = first.cbegin(); it != first.cend(); ++it){
    if(*it != pivot){
      resolvent_literals.insert(*it);
    }
  }
  for(auto it = second.cbegin(); it != second.cend(); ++it){
    if(*it != -pivot){
      resolvent_literals.insert(*it);
    }
  }
  Clause resolvent{};
  resolvent.GetLiterals().assign(resolvent_literals.begin(), 
                                 resolvent_literals.end());
  return resolvent;
}

struct Reason {
  Reason(shared_ptr<Clause> clause=nullptr, int pivot=0) : clause(clause), 
                                                           pivot(pivot){}
  shared_ptr<Clause> clause;
  int pivot;
};

bool Formula::Propagate()
{
  unordered_map<shared_ptr<Clause>, Reason> reason;

  shared_ptr<Clause> conflict = nullptr;
  while(conflict == nullptr && !unit_clauses_.empty()) {
    auto unit_clause = unit_clauses_.front();
    auto literal = -unit_clause->GetLiterals().front();
    unit_clauses_.pop_front();
    Falsify(literal);
    auto& watches = Watches(literal);
    for(auto& watch : watches) {
      auto clause = watch.GetClause();
      if(watch.GetBlockingLiteral() != 0) {
        continue;
      } else if(clause->size() == 1) {
        conflict = clause;
        reason[conflict] = Reason(unit_clause, literal);
        break;
      }

      SwapLiteralToSecondPosition(literal, *clause);
      auto other_watched_literal = clause->GetLiterals().front();

      if(TruthValue(other_watched_literal) == kTrue) {
        watch.SetBlockingLiteral(other_watched_literal);
      } else {
        auto it_unfalsified_unwatched_literal =
            IteratorToUnfalsifiedUnwatchedLiteral(*clause);

        if(it_unfalsified_unwatched_literal == clause->end()) {
          if(TruthValue(other_watched_literal) == kFalse) {
            conflict = clause;
            reason[conflict] = Reason(unit_clause,literal);
          } else {
            unit_clauses_.emplace_back(clause);
            reason[clause] = Reason(unit_clause,literal);
          }
        } else if(TruthValue(*it_unfalsified_unwatched_literal) == kTrue) {
          watch.SetBlockingLiteral(*it_unfalsified_unwatched_literal);
        } else {
          swap(*it_unfalsified_unwatched_literal, clause->GetLiterals()[1]);
          Watches(clause->GetLiterals()[1]).emplace_back(watch);
        }
      }
    }
  }
  
  if(conflict != nullptr){
    auto current = conflict;
    Clause resolvent = *conflict;
    while(reason[current].clause != nullptr){
      cout << resolvent << " o ";
      int pivot = reason[current].pivot;
      current = reason[current].clause;
      resolvent = Resolve(resolvent, *current, pivot);
      cout << *current << " : " << resolvent << endl; 
    }
    cout << endl;
  }
  return conflict == nullptr;
}

} // namespace
