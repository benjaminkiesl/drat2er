#ifndef DRAT2ER_RAT_CLAUSE_H_
#define DRAT2ER_RAT_CLAUSE_H_

#include <vector>
#include <map>
#include <memory>
#include "clause.h"

namespace drat2er
{

class RatClause : public Clause
{
  friend void swap(RatClause& lhs, RatClause& rhs);

 public:
  RatClause() = default;
  RatClause(const RatClause& other) = default;
  RatClause(RatClause&& other);
  RatClause& operator=(RatClause other);
  int GetPivot() const;
  void AddHint(int resolution_partner, const std::vector<int>& hints);
  const std::map<int, std::vector<int>>& GetHints() const; 
  virtual std::string ToLrat() const override;

 private:
  std::map<int, std::vector<int>> hints_;
};

} // namespace

#endif
