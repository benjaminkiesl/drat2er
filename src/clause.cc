#include "clause.h"

#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <initializer_list>

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::max;
using std::max_element;
using std::abs;

namespace drat2er
{

Clause::Clause() : index_ {-1}, literals_ {}, max_variable_ {0} { }

Clause::Clause(Clause&& other) : Clause()
{
  swap(*this, other);
}

Clause& Clause::operator=(Clause other)
{
  swap(*this, other);
  return *this;
}

int Clause::GetIndex() const
{
  return index_;
}

void Clause::SetIndex(int index)
{
  index_ = index;
}

void Clause::SetLiterals(const std::vector<int>& literals)
{
  literals_.clear();
  max_variable_ = 0;
for(auto literal : literals) {
    AddLiteral(literal);
  }
}

void Clause::AddLiteral(const int literal)
{
  literals_.emplace_back(literal);
  max_variable_ = max(max_variable_, abs(literal));
}

int Clause::GetMaxVariable() const
{
  return max_variable_;
}

bool Clause::ContainsLiteral(const int literal) const
{
  return find(literals_.begin(), literals_.end(), literal) != literals_.end();
}

string Clause::ToDimacs() const
{
  stringstream ss;
  for(auto literal : literals_) {
    ss << literal << ' ';
  }
  ss << '0';
  return ss.str();
}

string Clause::ToLrat() const
{
  stringstream ss;
  ss << index_ << ' ' << ToDimacs();
  return ss.str();
}

void swap(Clause& lhs, Clause& rhs)
{
  using std::swap;
  swap(lhs.index_, rhs.index_);
  swap(lhs.literals_, rhs.literals_);
  swap(lhs.max_variable_, rhs.max_variable_);
}

bool operator==(const Clause& lhs, const Clause& rhs){
  return lhs.literals_ == rhs.literals_;
}

} // namespace
