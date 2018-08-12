#include "rup_clause.h"
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <initializer_list>
#include "clause.h"

using std::string;
using std::stringstream;
using std::initializer_list;
using std::endl;

namespace drat2er {

void swap(RupClause& lhs, RupClause& rhs){
  using std::swap;
  swap(static_cast<Clause&>(lhs), static_cast<Clause&>(rhs));
  swap(lhs.positive_hints_, rhs.positive_hints_);
}

RupClause::RupClause(initializer_list<int> literals) : Clause(literals) {
}

RupClause::RupClause(RupClause&& other) : RupClause(){
  swap(*this, other);
}

RupClause& RupClause::operator=(RupClause other){
  swap(*this, other);
  return *this;
}

void RupClause::AddPositiveHint(int hint){
  positive_hints_.emplace_back(hint);
}

void RupClause::ClearPositiveHints() {
  positive_hints_.clear();
}

const std::vector<int>& RupClause::GetPositiveHints() const{
  return positive_hints_;
}

string RupClause::ToLrat() const
{
  stringstream ss;
  ss << GetIndex() << ' ' << Clause::ToDimacs() << ' ';
  for(auto hint : positive_hints_){
    ss << hint << ' ';
  }
  ss << '0';
  return ss.str();
}

} // namespace
