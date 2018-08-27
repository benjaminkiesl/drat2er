#ifndef DRAT2ER_WATCH_H_
#define DRAT2ER_WATCH_H_

#include <memory>
#include "clause.h"

namespace drat2er
{

// Represents the watch object required for efficient unit propagation
// within the Formula class.
class Watch
{
 public:
  Watch();
  Watch(std::shared_ptr<Clause>, int blocking_literal=0);
  std::shared_ptr<Clause> GetClause();
  int GetBlockingLiteral();
  void SetBlockingLiteral(const int blocking_literal);
 private:
  std::shared_ptr<Clause> clause_;
  int blocking_literal_;
};

} // namespace

#endif
