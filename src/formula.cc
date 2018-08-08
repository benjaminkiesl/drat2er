#include "formula.h"
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>
#include <limits>
#include "clause.h"
#include "rup_clause.h"
#include "watch.h"

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::priority_queue;
using std::find;
using std::copy_if;
using std::for_each;
using std::back_inserter;
using std::cout;
using std::cerr;
using std::endl;

namespace drat2er
{

Formula::Formula(int number_of_variables, int number_of_clauses) :
clauses_(number_of_clauses),
unit_clauses_ {},
              occurrences_(2*number_of_variables),
              watch_table_(2*number_of_variables),
              assignment_ {},
              conflict_{nullptr},
              resolution_graph_{},
              empty_occurrence_list_ {},
              next_clause_index_ {std::numeric_limits<int>::max()}
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


bool Formula::Propagate()
{
  int propagation_step = 1;
  conflict_ = nullptr;
  while(conflict_ == nullptr && !unit_clauses_.empty()) {
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
        resolution_graph_[clause].emplace_back(unit_clause, literal, 
                                               propagation_step);
        conflict_ = clause;
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
          resolution_graph_[clause].emplace_back(unit_clause, literal, 
                                                 propagation_step);
          if(TruthValue(other_watched_literal) == kFalse) {
            conflict_ = clause;
          } else {
            unit_clauses_.emplace_back(clause);
          }
        } else if(TruthValue(*it_unfalsified_unwatched_literal) == kTrue) {
          watch.SetBlockingLiteral(*it_unfalsified_unwatched_literal);
        } else {
          std::swap(*it_unfalsified_unwatched_literal, 
                    clause->GetLiterals()[1]);
          Watches(clause->GetLiterals()[1]).emplace_back(watch);
          resolution_graph_[clause].emplace_back(unit_clause, literal, 
                                                 propagation_step);
        }
      }
    }
    propagation_step++;
  }
  return conflict_ == nullptr;
}

int Formula::GetUnusedClauseIndex() {
  while(clauses_.find(next_clause_index_) != clauses_.end()){
    next_clause_index_--;
  }
  return next_clause_index_;
}

bool operator<(const Reason& lhs, const Reason& rhs){
  return lhs.propagation_step < rhs.propagation_step;
}

unique_ptr<RupClause> Formula::DeriveSubsumingClause(const Clause& rup){
  unordered_set<shared_ptr<Clause>> marked_clauses;

  for(auto literal : rup.GetLiteralsConst()){
    Clause negated_unit{-literal};
    negated_unit.SetIndex(GetUnusedClauseIndex());
    AddClause(negated_unit);
    marked_clauses.insert(GetClause(negated_unit.GetIndex()));
  } 

  if(!Propagate()){
    priority_queue<Reason> reason_queue;

    RupClause subsuming_rup;
    subsuming_rup.SetIndex(rup.GetIndex());
    subsuming_rup.SetLiterals(conflict_->GetLiterals());
    subsuming_rup.AddPositiveHint(conflict_->GetIndex());
    for_each(resolution_graph_[conflict_].begin(),
             resolution_graph_[conflict_].end(), 
             [&](const auto& clause){ reason_queue.push(clause); });
    
    while(!reason_queue.empty()){
      auto current = reason_queue.top();
      reason_queue.pop();
      if(marked_clauses.find(current.clause) == marked_clauses.end()){
        marked_clauses.insert(current.clause);
        subsuming_rup.SetLiterals(Resolve(subsuming_rup, 
                                          *current.clause, 
                                          current.literal).GetLiterals());
        subsuming_rup.AddPositiveHint(current.clause->GetIndex());
        for_each(resolution_graph_[current.clause].begin(),
                 resolution_graph_[current.clause].end(), 
                 [&](const auto& reason){ reason_queue.push(reason); });
      }
    }
    return make_unique<RupClause>(subsuming_rup);
  } else {
    return nullptr;
  }
}

} // namespace
