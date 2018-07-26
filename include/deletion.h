#ifndef DRAT2ER_DELETION_H_
#define DRAT2ER_DELETION_H_

#include <vector>

namespace drat2er {

class Deletion{
  public:
    int GetIndex() const;
    void SetIndex(int index);
    void AddClauseIndex(int clause_index);
    void RemoveClauseIndex(int clause_index);
    const std::vector<int>& GetClauseIndices() const;
  private:
    std::vector<int> clause_indices_;
    int index_;
};

}

#endif
