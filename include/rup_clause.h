#ifndef DRAT2ER_RUP_CLAUSE_H_
#define DRAT2ER_RUP_CLAUSE_H_

#include <vector>
#include <memory>
#include <initializer_list>
#include "clause.h"

namespace drat2er
{

// A RupClause represents a RUP clause as given in an LRAT proof.
// It contains a list of positive hints. For the details of what these hints
// mean, consult the definition of the LRAT format, given in the paper
// "Efficient Certified RAT Verification" (CADE-26).
class RupClause : public Clause
{
  friend void swap(RupClause& lhs, RupClause& rhs);

 public:
  RupClause() = default;
  RupClause(std::initializer_list<int> literals);
  RupClause(const RupClause& other) = default;
  RupClause(RupClause&& other);
  RupClause& operator=(RupClause other);
  void AddPositiveHint(int hint);
  void ClearPositiveHints();
  const std::vector<int>& GetPositiveHints() const;
  virtual std::string ToLrat() const override;

 protected:
  std::vector<int> positive_hints_;
};

} // namespace

#endif
