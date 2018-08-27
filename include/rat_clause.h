#ifndef DRAT2ER_RAT_CLAUSE_H_
#define DRAT2ER_RAT_CLAUSE_H_

#include <vector>
#include <map>
#include <memory>
#include <initializer_list>
#include "clause.h"

namespace drat2er
{

// Represents a RAT clause in a RAT proof. It contains a list of positive
// hints as well as of negative hints. For the details of what these hints
// mean, consult the definition of the LRAT format, given in the paper
// "Efficient Certified RAT Verification" (CADE-26).
class RatClause : public Clause
{
  friend void swap(RatClause& lhs, RatClause& rhs);

 public:
  RatClause() = default;
  RatClause(std::initializer_list<int> literals);
  RatClause(const RatClause& other) = default;
  RatClause(RatClause&& other);
  RatClause& operator=(RatClause other);
  int GetPivot() const;
  void AddPositiveHint(int hint);
  const std::vector<int>& GetPositiveHints() const;
  void ClearPositiveHints();
  void AddNegativeHint(int resolution_partner, const std::vector<int>& hints);
  const std::map<int, std::vector<int>>& GetNegativeHints() const; 
  void ClearNegativeHints();
  virtual std::string ToLrat() const override;

 private:
  std::vector<int> positive_hints_;
  std::map<int, std::vector<int>> negative_hints_;
};

} // namespace

#endif
