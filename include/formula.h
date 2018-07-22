#ifndef DRAT2ER_FORMULA_H_
#define DRAT2ER_FORMULA_H_

#include <memory>
#include <vector>
#include "clause.h"

namespace drat2er
{

class Formula
{
 public:
  Formula(int number_of_variables, int number_of_clauses);
  void AddClause(const Clause& clause);
  void AddClauses(const std::vector<Clause>& clauses);
  std::shared_ptr<Clause> GetClause(const int clause_index) const;
  void DeleteClause(const int clause_index);
  void DeleteClauses(const std::vector<int>& clause_indices);
  int MaxVariable();
  std::vector<std::shared_ptr<Clause>>& Occurrences(const int literal);

  const auto& Clauses() {
    return clauses_;
  }

 private:
  std::vector<std::shared_ptr<Clause>> clauses_;
  std::vector<std::vector<std::shared_ptr<Clause>>> occurrences_;
  int max_variable_;
};

} // namespace

#endif
