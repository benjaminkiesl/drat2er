#ifndef DRAT2ER_FORMULA_H_
#define DRAT2ER_FORMULA_H_

#include <memory>
#include <vector>
#include <deque>
#include <stack>
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

// Represents a reason why a clause has become a unit clause
// during unit propagation. Reasons are used to obtain a resolution
// derivation after unit propagation has derived a conflict.
struct Reason {
  Reason(std::shared_ptr<Clause> clause=nullptr, 
         int literal=0) : clause(clause), 
                          literal(literal) {}
  std::shared_ptr<Clause> clause;
  int literal;
};

using OccurrenceList = std::vector<std::shared_ptr<Clause>>;
using WatchList = std::vector<Watch>;

// Represents a propostional formula in CNF. Can also maintain a truth
// assignment to its variables and perform unit propagation using the member
// function Propagate(). A resolution proof can be returned if unit 
// propagation derives a conflict.
class Formula
{

 public:
  Formula(int number_of_variables = 0, int number_of_clauses = 0);

  // Adds a clause to the formula.
  void AddClause(const Clause& clause);

  // Adds a sequence of clauses to the formula.
  void AddClauses(const std::vector<Clause>& clauses);

  // Returns the clause with the given index and nullptr if this clause
  // is not contained.
  std::shared_ptr<Clause> GetClause(const int clause_index) const;

  // Returns the number of clauses contained in the formula.
  size_t GetNumberOfClauses() const;

  // Deletes the clause with the given index if it is contained in the formula. 
  // Otherwise does nothing.
  void DeleteClause(const int clause_index);

  // Deletes the clauses with the given indices if they are contained in the
  // formula.
  void DeleteClauses(const std::vector<int>& clause_indices);

  // Returns a list of all clauses in which the given literal is contained.
  const OccurrenceList& Occurrences(const int literal);

  // Deletes the given clause from the occurrence list of the given literal.
  void DeleteClauseFromOccurrenceList(std::shared_ptr<Clause> clause, 
                                      const int literal);

  // Returns the list of watches. Watches are required for efficient unit
  // propagation.
  WatchList& Watches(const int literal);

  // Deletes the given clause from the watch list of the given literal.
  void DeleteClauseFromWatchList(std::shared_ptr<Clause> clause, 
                                 const int literal);

  // Performs unit propagation on the current formula.
  bool Propagate();

  // Takes a RUP clause and derives (via resolution) a clause that subsumes
  // this RUP clause. The result is a pointer to a RUP clause whose positive
  // hints encode the resolution chain via which the RUP can be obtained
  // from the clauses in the formula.
  std::unique_ptr<RupClause> DeriveSubsumingClause(const Clause& rup);

  // Returns the truth value of the given literal.
  int TruthValue(const int literal);

  // Sets the truth value of the given literal to TRUE.
  void Satisfy(const int literal);
  
  // Sets the truth value of the given literal to FALSE.
  void Falsify(const int literal);

  // Sets the truth value of the given literal to UNASSIGNED.
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
  std::stack<Reason> resolution_stack_;
  const OccurrenceList empty_occurrence_list_;
  int next_clause_index_;
};

} // namespace

#endif
