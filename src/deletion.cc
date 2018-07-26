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

}

