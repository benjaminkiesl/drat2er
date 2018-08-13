#ifndef DRAT2ER_RUP_TO_RESOLUTION_TRANSFORMER_H_
#define DRAT2ER_RUP_TO_RESOLUTION_TRANSFORMER_H_

#include <memory>
#include <iostream>
#include "proof_transformer.h"

namespace drat2er
{

class Formula;
class RatClause;
class RupClause;
class Deletion;

class RupToResolutionTransformer : public ProofTransformer
{

 public:
  RupToResolutionTransformer(std::shared_ptr<Formula> formula);

 protected:
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleExtension(const Clause& extension) override;

 private:
  std::shared_ptr<Formula> formula_;
  bool found_empty_clause_;
};

} // namespace

#endif
