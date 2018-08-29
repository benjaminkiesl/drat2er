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

#ifndef DRAT2ER_RUP_CLAUSE_H_
#define DRAT2ER_RUP_CLAUSE_H_

#include <vector>
#include <memory>
#include <initializer_list>
#include "clause.h"

namespace drat2er
{

// A RupClause represents a RUP clause as given in an LRAT proof.
// It contains a list of positive hints. For the details of what these hints
// mean, consult the definition of the LRAT format, given in the paper
// "Efficient Certified RAT Verification" (CADE-26).
class RupClause : public Clause
{
  friend void swap(RupClause& lhs, RupClause& rhs);

 public:
  RupClause() = default;
  RupClause(std::initializer_list<int> literals);
  RupClause(const RupClause& other) = default;
  RupClause(RupClause&& other);
  RupClause& operator=(RupClause other);
  void AddPositiveHint(int hint);
  void ClearPositiveHints();
  const std::vector<int>& GetPositiveHints() const;
  virtual std::string ToLrat() const override;

 protected:
  std::vector<int> positive_hints_;
};

} // namespace drat2er

#endif
