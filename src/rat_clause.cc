#include "rat_clause.h"
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include "clause.h"

using std::vector;
using std::string;
using std::stringstream;
using std::map;
using std::cout;
using std::endl;

namespace drat2er
{

void swap(RatClause& lhs, RatClause& rhs){
  using std::swap;
  swap(static_cast<Clause&>(lhs), static_cast<Clause&>(rhs));
  swap(lhs.hints_, rhs.hints_);
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

void RatClause::AddHint(int resolution_partner, const vector<int>& hints){
  hints_[resolution_partner] = hints;
}

const map<int, vector<int>>& RatClause::GetHints() const {
  return hints_;
}

string RatClause::ToLrat() const
{
  stringstream ss;
  ss << Clause::ToLrat() << ' ';
  for(auto hint : hints_){
    ss << -hint.first << ' ';
    for(auto sub_hint : hint.second){
      ss << sub_hint << ' ';
    }
  }
  ss << '0';
  return ss.str();
}

} // namespace
