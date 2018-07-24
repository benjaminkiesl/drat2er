#ifndef DRAT2ER_RUP_CLAUSE_H_
#define DRAT2ER_RUP_CLAUSE_H_

#include <vector>
#include <memory>
#include "clause.h"

namespace drat2er
{

class RupClause : public Clause
{
  friend void swap(RupClause& lhs, RupClause& rhs);

 public:
  RupClause() = default;
  RupClause(const RupClause& other) = default;
  RupClause(RupClause&& other);
  RupClause& operator=(RupClause other);
  void AddPositiveHint(int hint);
  const std::vector<int>& GetPositiveHints() const;
  virtual std::string ToLrat() const override;

 protected:
  std::vector<int> positive_hints_;
};

} // namespace

#endif
