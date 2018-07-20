#include "clause.h"

#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>

using std::string;
using std::stringstream;
using std::cout;
using std::endl;

namespace drat2er
{

Clause::Clause() : literals_{} { }

Clause::Clause(const Clause& other) : literals_(other.literals_) { }

Clause::Clause(Clause&& other) : Clause()
{
  swap(*this, other);
}

Clause& Clause::operator=(Clause other)
{
  swap(*this, other);
  return *this;
}

void Clause::AddLiteral(const int literal)
{
  literals_.emplace_back(literal);
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
  swap(lhs.literals_, rhs.literals_);
}

} // namespace
