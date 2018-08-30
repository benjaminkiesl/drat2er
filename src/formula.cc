// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "formula.h"
#include <memory>
#include <vector>
#include <unordered_map>
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
using std::unordered_map;
using std::find;
using std::copy_if;
using std::for_each;
using std::back_inserter;
using std::cout;
using std::cerr;
using std::endl;

namespace drat2er
{

Reason::Reason(std::shared_ptr<Clause> clause, const int literal) 
        : clause(clause), 
          literal(literal) {}

Formula::Formula(const int number_of_variables, const int number_of_clauses) 
          : clauses_(number_of_clauses),
            unit_clauses_ {},
            occurrences_(2*number_of_variables),
            watch_table_(2*number_of_variables),
            assignment_ {},
            conflict_ {nullptr},
            resolution_stack_ {},
            empty_occurrence_list_ {},
            next_clause_index_ {std::numeric_limits<int>::max()} {}

Formula::Formula(const Formula& other) 
    : clauses_(other.clauses_.size()), 
      unit_clauses_{},
      assignment_{other.assignment_},
      conflict_ {nullptr},
      resolution_stack_ {},
      empty_occurrence_list_{},
      next_clause_index_{other.next_clause_index_}
{
  for(auto index_clause : other.clauses_){
    AddClause(*index_clause.second);
  }
}


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

  if(!new_clause->empty()) {
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

size_t Formula::GetNumberOfClauses() const
{
  return clauses_.size();
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

const OccurrenceList& Formula::Occurrences(const int literal) const
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

int Formula::TruthValue(const int literal) const
{
  if(assignment_.find(abs(literal)) != assignment_.end()){
    return Sign(literal) * assignment_.at(abs(literal));
  }
  return kUnassigned;
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
  assignment_[abs(literal)] = kUnassigned;
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

bool Formula::Propagate()
{
  conflict_ = nullptr;
  while(conflict_ == nullptr && !unit_clauses_.empty()) {
    auto unit_clause = unit_clauses_.back();
    auto literal = -unit_clause->GetLiterals().front();
    unit_clauses_.pop_back();
    if(TruthValue(literal) == kFalse) {
      continue;
    }
    assert(TruthValue(literal) == kUnassigned);
    resolution_stack_.emplace(unit_clause, literal);
    Falsify(literal);
    auto& watches = Watches(literal);
    for(auto& watch : watches) {
      auto clause = watch.GetClause();
      if(TruthValue(watch.GetBlockingLiteral()) != 0) {
        continue;
      } else if(clause->size() == 1) {
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
          if(TruthValue(other_watched_literal) == kFalse) {
            conflict_ = clause;
          } else {
            unit_clauses_.emplace_front(clause);
          }
        } else if(TruthValue(*it_unfalsified_unwatched_literal) == kTrue) {
          watch.SetBlockingLiteral(*it_unfalsified_unwatched_literal);
        } else {
          std::swap(*it_unfalsified_unwatched_literal,
                    clause->GetLiterals()[1]);
          Watches(clause->GetLiterals()[1]).emplace_back(watch);
        }
      }
    }
  }
  return conflict_ == nullptr;
}

int Formula::GetUnusedClauseIndex()
{
  while(clauses_.find(next_clause_index_) != clauses_.end()) {
    next_clause_index_--;
  }
  return next_clause_index_;
}

unique_ptr<RupClause> Formula::DeriveSubsumingClause(const Clause& rup)
{
  for(auto literal : rup.GetLiteralsConst()) {
    Clause negated_unit {-literal};
    negated_unit.SetIndex(GetUnusedClauseIndex());
    AddClause(negated_unit);
  }

  if(!Propagate()) {
    RupClause subsuming_rup;
    subsuming_rup.SetIndex(rup.GetIndex());
    subsuming_rup.SetLiterals(conflict_->GetLiterals());
    subsuming_rup.AddPositiveHint(conflict_->GetIndex());

    while(!resolution_stack_.empty()) {
      auto current = resolution_stack_.top();
      resolution_stack_.pop();
      if(!rup.ContainsLiteral(current.literal) &&
      subsuming_rup.ContainsLiteral(current.literal)) {
        subsuming_rup.SetLiterals(Resolve(subsuming_rup,
                                          *current.clause,
                                          current.literal).GetLiterals());

        subsuming_rup.AddPositiveHint(current.clause->GetIndex());
      }
    }
    return make_unique<RupClause>(subsuming_rup);
  } else {
    return nullptr;
  }
}

} // namespace
