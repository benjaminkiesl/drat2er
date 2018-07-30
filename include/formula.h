#ifndef DRAT2ER_FORMULA_H_
#define DRAT2ER_FORMULA_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "clause.h"

namespace drat2er
{

using OccurrenceList = std::vector<std::shared_ptr<Clause>>;
using WatchList = std::unordered_set<std::shared_ptr<Clause>>;

class Formula
{
 public:
  Formula(int number_of_variables, int number_of_clauses);
  void AddClause(const Clause& clause);
  void AddClauses(const std::vector<Clause>& clauses);
  std::shared_ptr<Clause> GetClause(const int clause_index) const;
  const std::unordered_map<int, std::shared_ptr<Clause>>& GetClauses();
  void DeleteClause(const int clause_index);
  void DeleteClauses(const std::vector<int>& clause_indices);
  const OccurrenceList& Occurrences(const int literal);
  WatchList& Watches(const int literal);

 private:
  std::unordered_map<int, std::shared_ptr<Clause>> clauses_;
  std::unordered_map<int, OccurrenceList> occurrences_;
  std::unordered_map<int, WatchList> watch_table_;
  const OccurrenceList empty_occurrence_list_;
  WatchList empty_watch_list_;
};

} // namespace

#endif
