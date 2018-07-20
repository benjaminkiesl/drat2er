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
  void AddClause(std::unique_ptr<Clause> clause);
  std::shared_ptr<Clause> GetClause(const int clause_index) const;
  void DeleteClause(const int clause_index);
  std::vector<std::shared_ptr<Clause>>& Occurrences(const int literal);

  const auto& Clauses() {
    return clauses_;
  }

  const auto NumberOfVariables() {
    return occurrences_.size();
  }

 private:
  std::vector<std::shared_ptr<Clause>> clauses_;
  std::vector<std::vector<std::shared_ptr<Clause>>> occurrences_;
};

} // namespace

#endif
