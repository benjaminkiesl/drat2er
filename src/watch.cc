#include "watch.h"
#include <memory>
#include "clause.h"

using std::shared_ptr;

namespace drat2er
{

Watch::Watch() : clause_(nullptr),
                 blocking_literal_(0) { } 

Watch::Watch(shared_ptr<Clause> clause, int blocking_literal) :
                                    clause_(clause),
                                    blocking_literal_(blocking_literal) { }

shared_ptr<Clause> Watch::GetClause()
{
  return clause_;
}

int Watch::GetBlockingLiteral()
{
  return blocking_literal_;
}

void Watch::SetBlockingLiteral(const int blocking_literal){
  blocking_literal_ = blocking_literal;
}

} // namespace
