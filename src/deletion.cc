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

#include "deletion.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::find;

namespace drat2er {

int Deletion::GetIndex() const {
  return index_;
}

void Deletion::SetIndex(int index){
  index_ = index;
}

void Deletion::AddClauseIndex(int clause_index){
  clause_indices_.emplace_back(clause_index);
}

void Deletion::RemoveClauseIndex(int clause_index){
  clause_indices_.erase(
      find(clause_indices_.begin(), clause_indices_.end(), clause_index));
}

const vector<int>& Deletion::GetClauseIndices() const{
  return clause_indices_;
}

} // namespace drat2er

