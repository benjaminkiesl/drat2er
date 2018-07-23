#ifndef DRAT2ER_RAT_CLAUSE_H_
#define DRAT2ER_RAT_CLAUSE_H_

#include <vector>
#include <memory>
#include "clause.h"

namespace drat2er
{


class ResolutionPartner
{
 public:
  ResolutionPartner(int index);
  int GetIndex() const;
  std::vector<int>& Hints();

 private:
  int index_;
  std::vector<int> hints_;
};

class RatClause : public Clause
{
  friend void swap(RatClause& lhs, RatClause& rhs);
 public:
  RatClause() = default;
  RatClause(const RatClause& other) = default;
  RatClause(RatClause&& other);
  RatClause& operator=(RatClause other);
  int GetPivot() const;
  const std::vector<ResolutionPartner>& GetResolutionPartners() const;
  void AddResolutionPartner(ResolutionPartner resolution_partner);
 private:
  std::vector<ResolutionPartner> resolution_partners_;
};

} // namespace

#endif
