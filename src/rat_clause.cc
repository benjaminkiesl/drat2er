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

#include "rat_clause.h"
#include <vector>
#include <map>
#include <initializer_list>

using std::vector;
using std::map;

namespace drat2er
{

void swap(RatClause& lhs, RatClause& rhs)
{
  using std::swap;
  swap(static_cast<Clause&>(lhs), static_cast<Clause&>(rhs));
  swap(lhs.positive_hints_, rhs.positive_hints_);
  swap(lhs.negative_hints_, rhs.negative_hints_);
}

RatClause::RatClause(std::initializer_list<int> literals) : Clause(literals)
{
}

RatClause::RatClause(RatClause&& other) : RatClause()
{
  swap(*this, other);
}

RatClause& RatClause::operator=(RatClause other)
{
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

void RatClause::AddPositiveHint(int hint)
{
  positive_hints_.emplace_back(hint);
}

const vector<int>& RatClause::GetPositiveHints() const
{
  return positive_hints_;
}

void RatClause::ClearPositiveHints()
{
  positive_hints_.clear();
}

void RatClause::AddNegativeHint(const int resolution_partner,
                                const vector<int>& hints)
{
  negative_hints_[resolution_partner] = hints;
}

const map<int, vector<int>>& RatClause::GetNegativeHints() const
{
  return negative_hints_;
}

void RatClause::ClearNegativeHints()
{
  negative_hints_.clear();
}

} // namespace drat2er
