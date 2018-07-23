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

Clause::Clause() : index_{-1}, literals_{}, max_variable_{0} { }

Clause::Clause(Clause&& other) : Clause()
{
  swap(*this, other);
}

Clause& Clause::operator=(Clause other)
{
  swap(*this, other);
  return *this;
}

int Clause::GetIndex() const {
  return index_;
}

void Clause::SetIndex(int index){
  index_ = index;
}

void Clause::SetLiterals(const std::vector<int>& literals){
  literals_.clear();
  max_variable_ = 0;
  for(auto literal : literals){
    AddLiteral(literal);
  }
}

void Clause::AddLiteral(const int literal)
{
  literals_.emplace_back(literal);
  max_variable_ = max(max_variable_, abs(literal));
}

int Clause::GetMaxVariable() const {
  return max_variable_;
}

bool Clause::ContainsLiteral(const int literal) const
{
  return find(literals_.begin(), literals_.end(), literal) != literals_.end();
}

Clause::operator string() const
{
  stringstream clause_stream;
  for(auto literal : literals_){
    clause_stream << literal << " ";
  }
  clause_stream << "0";
  return clause_stream.str();
}

void swap(Clause& lhs, Clause& rhs)
{
  using std::swap;
  swap(lhs.index_, rhs.index_);
  swap(lhs.literals_, rhs.literals_);
  swap(lhs.max_variable_, rhs.max_variable_);
}

} // namespace
