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

#include "watch.h"
#include <memory>
#include "clause.h"

using std::shared_ptr;

namespace drat2er
{

Watch::Watch() : clause_(nullptr), 
                 blocking_literal_{0} { }

Watch::Watch(shared_ptr<Clause> clause, const int blocking_literal) 
  : clause_(clause),
    blocking_literal_{blocking_literal} { }

shared_ptr<Clause> Watch::GetClause()
{
  return clause_;
}

int Watch::GetBlockingLiteral() const
{
  return blocking_literal_;
}

void Watch::SetBlockingLiteral(const int blocking_literal)
{
  blocking_literal_ = blocking_literal;
}

} // namespace drat2er
