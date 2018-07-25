#ifndef DRAT2ER_FORMULA_H_
#define DRAT2ER_FORMULA_H_

#include <memory>
#include <vector>
#include <unordered_map>
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
  const std::vector<std::shared_ptr<Clause>>& GetClauses();
  void DeleteClause(const int clause_index);
  void DeleteClauses(const std::vector<int>& clause_indices);
  const std::vector<std::shared_ptr<Clause>>& Occurrences(const int literal);

 private:
  std::vector<std::shared_ptr<Clause>> clauses_;
  std::unordered_map<int, std::vector<std::shared_ptr<Clause>>> occurrences_;
  const std::vector<std::shared_ptr<Clause>> empty_occurrence_list_;
};

} // namespace

#endif
