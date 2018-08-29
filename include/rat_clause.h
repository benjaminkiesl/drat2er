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

#ifndef DRAT2ER_RAT_CLAUSE_H_
#define DRAT2ER_RAT_CLAUSE_H_

#include <vector>
#include <map>
#include <memory>
#include <initializer_list>
#include "clause.h"

namespace drat2er
{

// Represents a RAT clause in a RAT proof. It contains a list of positive
// hints as well as of negative hints. For the details of what these hints
// mean, consult the definition of the LRAT format, given in the paper
// "Efficient Certified RAT Verification" (CADE-26).
class RatClause : public Clause
{
  friend void swap(RatClause& lhs, RatClause& rhs);

 public:
  RatClause() = default;
  RatClause(std::initializer_list<int> literals);
  RatClause(const RatClause& other) = default;
  RatClause(RatClause&& other);
  RatClause& operator=(RatClause other);
  int GetPivot() const;
  void AddPositiveHint(int hint);
  const std::vector<int>& GetPositiveHints() const;
  void ClearPositiveHints();
  void AddNegativeHint(int resolution_partner, const std::vector<int>& hints);
  const std::map<int, std::vector<int>>& GetNegativeHints() const; 
  void ClearNegativeHints();
  virtual std::string ToLrat() const override;

 private:
  std::vector<int> positive_hints_;
  std::map<int, std::vector<int>> negative_hints_;
};

} // namespace drat2er

#endif
