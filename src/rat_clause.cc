#include "rat_clause.h"
#include <vector>
#include <memory>
#include <iostream>
#include "clause.h"

using std::vector;
using std::cout;
using std::endl;

namespace drat2er
{

ResolutionPartner::ResolutionPartner(int index) : index_ {index} {}

int ResolutionPartner::GetIndex() const
{
  return index_;
}

vector<int>& ResolutionPartner::Hints()
{
  return hints_;
}

void swap(RatClause& lhs, RatClause& rhs){
  using std::swap;
  swap(static_cast<Clause&>(lhs), static_cast<Clause&>(rhs));
  swap(lhs.resolution_partners_, rhs.resolution_partners_);
}

RatClause::RatClause(RatClause&& other) : RatClause(){
  swap(*this, other);
}

RatClause& RatClause::operator=(RatClause other){
  swap(*this, other);
  return *this;
}

int RatClause::GetPivot() const
{
  if(literals_.size() != 0) {
    return *(literals_.begin());
  }
  return 0;
}

const vector<ResolutionPartner>& RatClause::GetResolutionPartners() const
{
  return resolution_partners_;
}

void RatClause::AddResolutionPartner(ResolutionPartner resolution_partner)
{
  resolution_partners_.emplace_back(resolution_partner);
}

} // namespace
