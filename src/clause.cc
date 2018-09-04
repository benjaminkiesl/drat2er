// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "clause.h"
#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <initializer_list>

using std::vector;
using std::unordered_set;
using std::max;
using std::abs;

namespace drat2er
{

void swap(Clause& lhs, Clause& rhs)
{
  using std::swap;
  swap(lhs.index_, rhs.index_);
  swap(lhs.literals_, rhs.literals_);
}


Clause::Clause() : index_ {-1}, 
                   literals_ {} {}

Clause::Clause(std::initializer_list<int> literals) : Clause()
{
  for(auto literal : literals) {
    AddLiteral(literal);
  }
}

Clause::Clause(const Clause& other) : index_ {other.index_},
literals_(other.literals_) {}

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

void Clause::SetIndex(const int index)
{
  index_ = index;
}

vector<int>& Clause::GetLiterals()
{
  return literals_;
}

const vector<int>& Clause::GetLiteralsConst() const
{
  return literals_;
}

void Clause::SetLiterals(const vector<int>& literals)
{
  literals_.clear();
  for(auto literal : literals) {
    AddLiteral(literal);
  }
}

void Clause::AddLiteral(const int literal)
{
  literals_.emplace_back(literal);
}

bool Clause::IsUnit() const
{
  return literals_.size() == 1;
}

bool Clause::IsSubclauseOf(const Clause& other) const
{
  unordered_set<int> literals_of_clause(cbegin(), cend());
  unordered_set<int> literals_of_other(other.cbegin(), other.cend());
  for(auto literal : literals_of_clause) {
    if(literals_of_other.find(literal) == literals_of_other.end()) {
      return false;
    }
  }
  return true;
}

int Clause::GetMaxVariable() const
{
  int max_variable = 0;
  for(auto literal : literals_) {
    max_variable = max(max_variable, abs(literal));
  }
  return max_variable;
}

bool Clause::ContainsLiteral(const int literal) const
{
  return std::find(literals_.cbegin(), literals_.cend(), literal) 
         != literals_.cend();
}

Clause Resolve(const Clause& first, 
               const Clause& second,
               const int pivot_literal)
{
  assert(first.ContainsLiteral(pivot_literal));
  assert(second.ContainsLiteral(-pivot_literal));

  unordered_set<int> resolvent_literals;
  for(auto it = first.cbegin(); it != first.cend(); ++it) {
    if(*it != pivot_literal) {
      resolvent_literals.insert(*it);
    }
  }
  for(auto it = second.cbegin(); it != second.cend(); ++it) {
    if(*it != -pivot_literal) {
      resolvent_literals.insert(*it);
    }
  }
  Clause resolvent {};
  resolvent.GetLiterals().assign(resolvent_literals.begin(),
                                 resolvent_literals.end());
  return resolvent;
}

} // namespace drat2er
