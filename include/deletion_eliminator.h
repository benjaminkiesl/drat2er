#ifndef DRAT2ER_DELETION_ELIMINATOR_H_
#define DRAT2ER_DELETION_ELIMINATOR_H_

#include <string>
#include "proof_transformer.h"

namespace drat2er
{

class DeletionEliminator : public ProofTransformer
{

 protected:
  virtual void HandleDeletion(const Deletion& deletion) override;
  virtual void HandleProperRatAddition(const RatClause& rat) override;
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleComment(const std::string& comment_line) override;
  virtual void HandleExtension(const Clause& clause) override;

};

} // namespace

#endif
