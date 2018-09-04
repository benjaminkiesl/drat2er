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

#include "rup_clause.h"
#include <vector>
#include <initializer_list>
#include "clause.h"

using std::vector;

namespace drat2er
{

void swap(RupClause& lhs, RupClause& rhs)
{
  using std::swap;
  swap(static_cast<Clause&>(lhs), static_cast<Clause&>(rhs));
  swap(lhs.positive_hints_, rhs.positive_hints_);
}

RupClause::RupClause(std::initializer_list<int> literals) : Clause(literals)
{
}

RupClause::RupClause(RupClause&& other) : RupClause()
{
  swap(*this, other);
}

RupClause& RupClause::operator=(RupClause other)
{
  swap(*this, other);
  return *this;
}

void RupClause::AddPositiveHint(const int hint)
{
  positive_hints_.emplace_back(hint);
}

void RupClause::ClearPositiveHints()
{
  positive_hints_.clear();
}

const vector<int>& RupClause::GetPositiveHints() const
{
  return positive_hints_;
}

} // namespace drat2er
