#ifndef DRAT2ER_FORMULA_H_
#define DRAT2ER_FORMULA_H_

#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include "watch.h"

namespace drat2er
{

const int kFalse = -1; 
const int kUnassigned = 0; 
const int kTrue = 1;

class Clause;
class RupClause;

struct Reason {
  Reason(std::shared_ptr<Clause> clause=nullptr, 
         int literal=0, 
         int propagation_step = 0) : clause(clause), 
                                     literal(literal),
                                     propagation_step(propagation_step){}
  std::shared_ptr<Clause> clause;
  int literal;
  int propagation_step;
};

bool operator<(const Reason& lhs, const Reason& rhs);

using OccurrenceList = std::vector<std::shared_ptr<Clause>>;
using WatchList = std::vector<Watch>;

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
  void DeleteClauseFromOccurrenceList(std::shared_ptr<Clause> clause, 
                                      const int literal);
  WatchList& Watches(const int literal);
  void DeleteClauseFromWatchList(std::shared_ptr<Clause> clause, 
                                 const int literal);
  bool Propagate();
  std::unique_ptr<RupClause> DeriveSubsumingClause(const Clause& rup);
  int TruthValue(const int literal);
  void Satisfy(const int literal);
  void Falsify(const int literal);
  void Unassign(const int literal);

 private:
  auto IteratorToUnfalsifiedUnwatchedLiteral(Clause& clause);
  int GetUnusedClauseIndex();

  std::unordered_map<int, std::shared_ptr<Clause>> clauses_;
  std::deque<std::shared_ptr<Clause>> unit_clauses_;
  std::unordered_map<int, OccurrenceList> occurrences_;
  std::unordered_map<int, WatchList> watch_table_;
  std::unordered_map<int, int> assignment_;
  std::shared_ptr<Clause> conflict_;
  std::unordered_map<std::shared_ptr<Clause>, std::vector<Reason>> 
    resolution_graph_;
  const OccurrenceList empty_occurrence_list_;
  int next_clause_index_;
};

} // namespace

#endif
